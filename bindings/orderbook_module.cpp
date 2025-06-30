#include <iomanip>
#include <iostream>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SingleVariableCounter.h"
#include "MarketState.h"
#include "TradeEntry.h"
#include "DifferenceDepthEntry.h"
#include "GlobalMarketState.h"
#include "OrderBook.h"
#include "OrderBookMetricsEntry.h"
#include "OrderBookMetrics.h"
#include "OrderBookSessionSimulator.h"
#include "OrderBookMetricsCalculator.h"
#include "enums/Market.h"

namespace py = pybind11;
using MS = MarketState;

PYBIND11_MODULE(cpp_binance_orderbook, m) {
    // std::cout << std::fixed << std::setprecision(5);

    // ----- OrderbookSessionSimulator -----
    py::class_<OrderBookSessionSimulator>(m, "OrderBookSessionSimulator")
        .def(py::init<>())
        .def("compute_backtest", &OrderBookSessionSimulator::computeBacktest,
             py::arg("csv_path"), py::arg("variables"), py::arg("python_callback") = py::none(),
             "Uruchamia backtest; zwraca void")
        .def("compute_variables", &OrderBookSessionSimulator::computeVariables,
             py::arg("csv_path"), py::arg("variables"),
             "Oblicza metryki z pliku CSV i zwraca OrderBookMetrics")

        .def("compute_variables_numpy",
             &OrderBookSessionSimulator::computeVariablesNumPy,
             py::arg("csv_path"), py::arg("variables"),
             "compute_variables_numpy(csv_path, variables) -> dict of numpy arrays")
        .def("compute_backtest_numpy",
             &OrderBookSessionSimulator::computeBacktestNumPy,
             py::arg("csv_path"), py::arg("variables"), py::arg("python_callback") = py::none(),
             "compute_backtest_numpy(csv_path, variables[, python_callback]) -> dict of numpy arrays")

        .def("compute_final_depth_snapshot", &OrderBookSessionSimulator::computeFinalDepthSnapshot,
             py::arg("csv_path"),
             "Zwraca FinalOrderBookSnapshot")
        ;

    // ----- GlobalMarketState -----
    py::class_<GlobalMarketState>(m, "GlobalMarketState")
        .def(py::init<MetricMask>(), py::arg("mask"),
             "Tworzy GlobalMarketState z podaną maską zmiennych")
        .def("update",
             &GlobalMarketState::update,
             py::arg("entry"),
             "Aktualizuje stan rynkowy na podstawie DifferenceDepthEntry lub TradeEntry")
        .def("count_market_state_metrics_by_entry",
             &GlobalMarketState::countMarketStateMetricsByEntry,
             py::arg("entry"),
             "Stara wersja: liczy metryki dla podanego DecodedEntry")
        .def("count_market_state_metrics",
             &GlobalMarketState::countMarketStateMetrics,
             py::arg("symbol"), py::arg("market"),
             "Stara wersja: liczy metryki dla podanego DecodedEntry")
        .def("get_market_state",
             &GlobalMarketState::getMarketState,
             py::arg("symbol"), py::arg("market"),
             py::return_value_policy::reference_internal,
             "Zwraca obiekt MarketState dla danego symbolu i rynku")
            .def("get_market_state_count", &GlobalMarketState::getMarketStateCount,
                 "Liczba przechowywanych (symbol, market)")
            .def("__len__", &GlobalMarketState::getMarketStateCount)
            .def("get_market_state_list", &GlobalMarketState::getMarketStateList,
                 "Zwraca listę (symbol, market) dostępnych w GlobalMarketState")
        ;


    // ----- MarketState -----
    py::class_<MS>(m, "MarketState")
        .def(py::init<>(), "Tworzy nowy MarketState")
        .def_readonly("order_book", &MS::orderBook, py::return_value_policy::reference_internal)
        .def_readonly("rolling_statistics_data", &MS::rollingStatisticsData, py::return_value_policy::reference_internal)
        .def("update",
                 &MS::update,
                 py::arg("entry"),
                 "Aktualizuje stan rynkowy na podstawie DifferenceDepthEntry lub TradeEntry")
        .def("update_orderbook",
             &MS::updateOrderBook,
             py::call_guard<py::gil_scoped_release>(),
             py::arg("timestamp_of_receive"),
             py::arg("price"),
             py::arg("quantity"),
             py::arg("is_ask"),
             "Bardzo szybka aktualizacja DifferenceDepthEntry")
        .def("update_trade_registry",
             &MS::updateTradeRegistry,
             py::call_guard<py::gil_scoped_release>(),
             py::arg("timestamp_of_receive"),
             py::arg("price"),
             py::arg("quantity"),
             py::arg("is_buyer_market_maker"),
             "Bardzo szybka aktualizacja TradeEntry")
        .def_property_readonly(
            "has_last_trade",
            &MS::getHasLastTrade,
            "Czy jest w pamięci ostatni TradeEntry?"
        )
        .def_property_readonly(
            "last_trade",
            &MS::getLastTrade,
            py::return_value_policy::reference_internal,
            "Ostatni TradeEntry (rzuca, jeśli nie ma żadnego)"
        )
        .def_property_readonly(
            "last_timestamp_of_receive",
            &MS::getLastTimestampOfReceive,
            "Ostatni użyty timestampOfReceive"
        )
        .def("print_order_book",
             [](MS &self) {
                 self.orderBook.printOrderBook();
             },
             "Wypisuje stan orderbooka")
        .def_property_readonly(
            "symbol",
            &MS::getSymbol,
            "Zwraca symbol tej MarketState"
        )
        .def_property_readonly(
            "market",
            &MS::getMarket,
            "Zwraca market tej MarketState"
        )
        .def("do_nothing",
            &MS::doNothing,
            "xD it does nothing"
            )
        ;

    // ----- OrderBook -----
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("print_order_book",                    &OrderBook::printOrderBook)
        .def("asks",                                &OrderBook::getAsks, "Return list of all ask levels in the book, in linked-list order")
        .def("bids",                                &OrderBook::getBids, "Return list of all bid levels in the book, in linked-list order")
        .def("ask_count",                           &OrderBook::askCount, "Number of ask levels in the book")
        .def("bid_count",                           &OrderBook::bidCount, "Number of bid levels in the book")
        .def("sum_ask_quantity",                    &OrderBook::sumAskQuantity, "Total quantity on the ask side")
        .def("sum_bid_quantity",                    &OrderBook::sumBidQuantity, "Total quantity on the bid side")
        .def("best_ask_price",                      &OrderBook::bestAskPrice, "Price of the best (lowest) ask")
        .def("best_bid_price",                      &OrderBook::bestBidPrice, "Price of the best (highest) bid")
        .def("best_ask_quantity",                   &OrderBook::bestAskQuantity, "Quantity available at the best ask")
        .def("best_bid_quantity",                   &OrderBook::bestBidQuantity, "Quantity available at the best bid")
        .def("second_ask_price",                    &OrderBook::secondAskPrice, "Price of the second-best ask")
        .def("second_bid_price",                    &OrderBook::secondBidPrice, "Price of the second-best bid")

        .def("cumulative_quantity_of_top_n_asks",   &OrderBook::cumulativeQuantityOfTopNAsks, "Price of the second-best bid")
        .def("cumulative_quantity_of_top_n_bids",   &OrderBook::cumulativeQuantityOfTopNBids, "Price of the second-best bid")
        .def("sum_of_price_times_quantity",         &OrderBook::sumOfPriceTimesQuantity, "Price of the second-best bid")
        .def("best_nth_ask_price",                  &OrderBook::bestNthAskPrice, "Price of the second-best bid")
        .def("best_nth_bid_price",                  &OrderBook::bestNthBidPrice, "Price of the second-best bid")

        .def("update",
             &OrderBook::update,
             py::arg("entry"),
             "Apply a single DifferenceDepthEntry update to this order book")
        ;

    // ----- SingleVariableCounter -----
    auto svc = m.def_submodule("single_variable_counter", "Compute single-variable order book metrics");
    svc.def("calculate_best_ask_price",                             &SingleVariableCounter::calculateBestAskPrice,                                          py::arg("order_book"));
    svc.def("calculate_best_bid_price",                             &SingleVariableCounter::calculateBestBidPrice,                                          py::arg("order_book"));
    svc.def("calculate_mid_price",                                  &SingleVariableCounter::calculateMidPrice,                                              py::arg("order_book"));
    svc.def("calculate_best_volume_imbalance",                      &SingleVariableCounter::calculateBestVolumeImbalance,                                   py::arg("order_book"));
    svc.def("calculate_best_depth_volume_ratio",                    &SingleVariableCounter::calculateBestDepthVolumeRatio,                                  py::arg("order_book"));
    svc.def("calculate_best_n_price_levels_volume_imbalance",       &SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance,                       py::arg("order_book"), py::arg("nPriceLevels"));
    svc.def("calculate_volume_imbalance",                           &SingleVariableCounter::calculateVolumeImbalance,                                       py::arg("order_book"));
    svc.def("calculate_queue_imbalance",                            &SingleVariableCounter::calculateQueueImbalance,                                        py::arg("order_book"));
    svc.def("calculate_gap",                                        &SingleVariableCounter::calculateGap,                                                   py::arg("order_book"));
    svc.def("calculate_vwap_deviation",                             &SingleVariableCounter::calculateVwapDeviation,                                         py::arg("order_book"));
    svc.def("calculate_is_aggressor_ask", [](const TradeEntry &t){ return SingleVariableCounter::calculateIsAggressorAsk(&t); },                    py::arg("trade_entry"));
    svc.def("calculate_simplified_slope_imbalance",                 &SingleVariableCounter::calculateSimplifiedSlopeImbalance,                              py::arg("order_book"));

    svc.def("calculate_trade_count_imbalance",                      &SingleVariableCounter::calculateTradeCountImbalance,                                   py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));
    svc.def("calculate_cumulative_delta",                           &SingleVariableCounter::calculateCumulativeDelta,                                       py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));
    svc.def("calculate_price_difference",                           &SingleVariableCounter::calculatePriceDifference,                                       py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));
    svc.def("calculate_rate_of_return",                             &SingleVariableCounter::calculateRateOfReturn,                                          py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));
    svc.def("calculate_difference_depth_volatility_imbalance",      &SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance,                    py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));

    svc.def("calculate_rsi",                                        &SingleVariableCounter::calculateRSI,                                                   py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));
    svc.def("calculate_stoch_rsi",                                  &SingleVariableCounter::calculateStochRSI,                                              py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));
    svc.def("calculate_macd",                                       &SingleVariableCounter::calculateMacd,                                                  py::arg("rolling_statistics_data"), py::arg("windowTimeSeconds"));

    // ----- DifferenceDepthEntry (DifferenceDepthEntry) -----
    py::class_<DifferenceDepthEntry>(m, "DifferenceDepthEntry")
        .def(py::init<>())
        .def(py::init<
            int64_t,
            Symbol,
            bool,
            double,
            double,
            bool,
            Market
            >(),
            py::arg("timestamp_of_receive"),
            py::arg("symbol"),
            py::arg("is_ask"),
            py::arg("price"),
            py::arg("quantity"),
            py::arg("is_last"),
            py::arg("market")
        )
        .def_readwrite("timestamp_of_receive", &DifferenceDepthEntry::timestampOfReceive)
        .def_readwrite("symbol", &DifferenceDepthEntry::symbol)
        .def_readwrite("is_ask", &DifferenceDepthEntry::isAsk)
        .def_readwrite("price", &DifferenceDepthEntry::price)
        .def_readwrite("quantity", &DifferenceDepthEntry::quantity)
        .def_readwrite("is_last", &DifferenceDepthEntry::isLast)
        .def_readwrite("market", &DifferenceDepthEntry::market)
        .def("__str__", [](const DifferenceDepthEntry &entry) {
            std::ostringstream oss;
            // oss << std::fixed << std::setprecision(5);
            oss
            << "TimestampOfReceive: " << entry.timestampOfReceive << " "
            << "Symbol: " << entry.symbol << " "
            << "IsAsk: " << entry.isAsk << " "
            << "Price: " << entry.price << " "
            << "Quantity: " << entry.quantity << " "
            << "IsLast: " << entry.isLast << " "
            << "Market_: " << entry.market << " ";
            return oss.str();
        })
        .def("to_list", [](const DifferenceDepthEntry &e) {
            py::list v;
            v.append(e.timestampOfReceive);
            v.append(e.symbol);
            v.append(e.isAsk ? 1 : 0);
            v.append(e.price);
            v.append(e.quantity);
            v.append(e.isLast ? 1 : 0);
            v.append(e.market);
            return v;
        })
        .def_static("field_names", []() {
            return std::vector<std::string>{
                "timestamp_of_receive",
                "symbol",
                "is_ask",
                "price",
                "quantity",
                "is_last",
                "market"
            };
        });

    // ----- TradeEntry -----
    py::class_<TradeEntry>(m, "TradeEntry")
        .def(py::init<>())
        .def(py::init<
            int64_t,
            Symbol,
            double,
            double,
            bool,
            const std::string&,
            const std::string&,
            bool,
            Market
            >(),
            py::arg("timestamp_of_receive"),
            py::arg("symbol"),
            py::arg("price"),
            py::arg("quantity"),
            py::arg("is_buyer_market_maker"),
            py::arg("m_unknown_parameter"),
            py::arg("x_unknown_parameter"),
            py::arg("is_last"),
            py::arg("market")
        )
        .def_readwrite("timestamp_of_receive",    &TradeEntry::timestampOfReceive)
        .def_readwrite("symbol",                  &TradeEntry::symbol)
        .def_readwrite("price",                   &TradeEntry::price)
        .def_readwrite("quantity",                &TradeEntry::quantity)
        .def_readwrite("is_buyer_market_maker",   &TradeEntry::isBuyerMarketMaker)
        .def_readwrite("m_unknown_parameter",     &TradeEntry::mUnknownParameter)
        .def_readwrite("x_unknown_parameter",     &TradeEntry::xUnknownParameter)
        .def_readwrite("is_last",                 &TradeEntry::isLast)
        .def_readwrite("market",                  &TradeEntry::market)
        .def("__str__", [](const TradeEntry &entry) {
            std::ostringstream oss;
            // oss << std::fixed << std::setprecision(5);
            oss
            << "TimestampOfReceive: " << entry.timestampOfReceive << " "
            << "Symbol: " << entry.symbol << " "
            << "Price: " << entry.price << " "
            << "Quantity: " << entry.quantity << " "
            << "IsBuyerMarketMaker: " << entry.isBuyerMarketMaker << " "
            << "IsLast: " << entry.isLast << " "
            << "Market_: " << entry.market << " ";
            return oss.str();
        })
        .def("to_list", [](const TradeEntry &e) {
            py::list v;
            v.append(e.timestampOfReceive);
            v.append(e.symbol);
            v.append(e.price);
            v.append(e.quantity);
            v.append(e.isBuyerMarketMaker ? 1 : 0);
            v.append(e.mUnknownParameter);
            v.append(e.xUnknownParameter);
            v.append(e.isLast ? 1 : 0);
            v.append(e.market);
            return v;
        })
        .def_static("field_names", []() {
            return std::vector<std::string>{
                "timestamp_of_receive",
                "symbol",
                "price",
                "quantity",
                "is_buyer_market_maker",
                "m_unknown_parameter",
                "x_unknown_parameter",
                "is_last",
                "market"
            };
        })
    ;

    py::class_<RollingStatisticsData>(m, "RollingStatisticsData")
    .def(py::init<>())
    .def("bid_difference_depth_entry_count",
         &RollingStatisticsData::bidDifferenceDepthEntryCount,
         py::arg("windowTimeSeconds"),
         "Liczba bid DifferenceDepthEntry w oknie")
    .def("ask_difference_depth_entry_count",
         &RollingStatisticsData::askDifferenceDepthEntryCount,
         py::arg("windowTimeSeconds"),
         "Liczba ask DifferenceDepthEntry w oknie")
    .def("buy_trade_count",
         &RollingStatisticsData::buyTradeCount,
         py::arg("windowTimeSeconds"),
         "Liczba buy TradeEntry w oknie")
    .def("sell_trade_count",
         &RollingStatisticsData::sellTradeCount,
         py::arg("windowTimeSeconds"),
         "Liczba sell TradeEntry w oknie")
    .def("buy_trade_volume",
         &RollingStatisticsData::buyTradeVolume,
         py::arg("windowTimeSeconds"),
         "Wolumen buy TradeEntry w oknie")
    .def("sell_trade_volume",
         &RollingStatisticsData::sellTradeVolume,
         py::arg("windowTimeSeconds"),
         "Wolumen sell TradeEntry w oknie")
    .def("price_difference",
         &RollingStatisticsData::priceDifference,
         py::arg("windowTimeSeconds"),
         "Różnica ceny w oknie")
    .def("oldest_price",
         &RollingStatisticsData::oldestPrice,
         py::arg("windowTimeSeconds"),
         "Najstarsza cena w oknie")
    .def("simple_moving_average",
         &RollingStatisticsData::simpleMovingAverage,
         py::arg("windowTimeSeconds"),
         "Prosta średnia ruchoma ceny w oknie")
    ;

    // ----- OrderBookMetricsEntry -----
    py::class_<OrderBookMetricsEntry>(m, "OrderBookMetricsEntry")
        .def_readonly("timestampOfReceive",             &OrderBookMetricsEntry::timestampOfReceive)
        .def_readonly("market",                         &OrderBookMetricsEntry::market)
        .def_readonly("symbol",                         &OrderBookMetricsEntry::symbol)
        .def_readonly("bestAskPrice",                   &OrderBookMetricsEntry::bestAskPrice)
        .def_readonly("bestBidPrice",                   &OrderBookMetricsEntry::bestBidPrice)
        .def_readonly("midPrice",                       &OrderBookMetricsEntry::midPrice)
        .def_readonly("bestVolumeImbalance",            &OrderBookMetricsEntry::bestVolumeImbalance)
        .def_readonly("bestVolumeRatio",                &OrderBookMetricsEntry::bestVolumeRatio)
        .def_readonly("bestTwoVolumeImbalance",         &OrderBookMetricsEntry::bestTwoVolumeImbalance)
        .def_readonly("bestThreeVolumeImbalance",       &OrderBookMetricsEntry::bestThreeVolumeImbalance)
        .def_readonly("bestFiveVolumeImbalance",        &OrderBookMetricsEntry::bestFiveVolumeImbalance)
        .def_readonly("volumeImbalance",                &OrderBookMetricsEntry::volumeImbalance)
        .def_readonly("queueImbalance",                 &OrderBookMetricsEntry::queueImbalance)
        .def_readonly("gap",                            &OrderBookMetricsEntry::gap)
        .def_readonly("isAggressorAsk",                 &OrderBookMetricsEntry::isAggressorAsk)
        .def_readonly("vwapDeviation",                  &OrderBookMetricsEntry::vwapDeviation)
        .def_readonly("simplifiedSlopeImbalance",       &OrderBookMetricsEntry::simplifiedSlopeImbalance)

        .def_readonly("tradeCountImbalance1Seconds",    &OrderBookMetricsEntry::tradeCountImbalance1Seconds)
        .def_readonly("tradeCountImbalance3Seconds",    &OrderBookMetricsEntry::tradeCountImbalance3Seconds)
        .def_readonly("tradeCountImbalance5Seconds",    &OrderBookMetricsEntry::tradeCountImbalance5Seconds)
        .def_readonly("tradeCountImbalance10Seconds",   &OrderBookMetricsEntry::tradeCountImbalance10Seconds)
        .def_readonly("tradeCountImbalance15Seconds",   &OrderBookMetricsEntry::tradeCountImbalance15Seconds)
        .def_readonly("tradeCountImbalance30Seconds",   &OrderBookMetricsEntry::tradeCountImbalance30Seconds)
        .def_readonly("tradeCountImbalance60Seconds",   &OrderBookMetricsEntry::tradeCountImbalance60Seconds)

        .def_readonly("cumulativeDelta1Seconds",        &OrderBookMetricsEntry::cumulativeDelta1Seconds)
        .def_readonly("cumulativeDelta3Seconds",        &OrderBookMetricsEntry::cumulativeDelta3Seconds)
        .def_readonly("cumulativeDelta5Seconds",        &OrderBookMetricsEntry::cumulativeDelta5Seconds)
        .def_readonly("cumulativeDelta10Seconds",       &OrderBookMetricsEntry::cumulativeDelta10Seconds)
        .def_readonly("cumulativeDelta15Seconds",       &OrderBookMetricsEntry::cumulativeDelta15Seconds)
        .def_readonly("cumulativeDelta30Seconds",       &OrderBookMetricsEntry::cumulativeDelta30Seconds)
        .def_readonly("cumulativeDelta60Seconds",       &OrderBookMetricsEntry::cumulativeDelta60Seconds)

        .def_readonly("priceDifference1Seconds",        &OrderBookMetricsEntry::priceDifference1Seconds)
        .def_readonly("priceDifference3Seconds",        &OrderBookMetricsEntry::priceDifference3Seconds)
        .def_readonly("priceDifference5Seconds",        &OrderBookMetricsEntry::priceDifference5Seconds)
        .def_readonly("priceDifference10Seconds",       &OrderBookMetricsEntry::priceDifference10Seconds)
        .def_readonly("priceDifference15Seconds",       &OrderBookMetricsEntry::priceDifference15Seconds)
        .def_readonly("priceDifference30Seconds",       &OrderBookMetricsEntry::priceDifference30Seconds)
        .def_readonly("priceDifference60Seconds",       &OrderBookMetricsEntry::priceDifference60Seconds)

        .def_readonly("rateOfReturn1Seconds",           &OrderBookMetricsEntry::rateOfReturn1Seconds)
        .def_readonly("rateOfReturn3Seconds",           &OrderBookMetricsEntry::rateOfReturn3Seconds)
        .def_readonly("rateOfReturn5Seconds",           &OrderBookMetricsEntry::rateOfReturn5Seconds)
        .def_readonly("rateOfReturn10Seconds",          &OrderBookMetricsEntry::rateOfReturn10Seconds)
        .def_readonly("rateOfReturn15Seconds",          &OrderBookMetricsEntry::rateOfReturn15Seconds)
        .def_readonly("rateOfReturn30Seconds",          &OrderBookMetricsEntry::rateOfReturn30Seconds)
        .def_readonly("rateOfReturn60Seconds",          &OrderBookMetricsEntry::rateOfReturn60Seconds)

        .def_readonly("differenceDepthVolatilityImbalance1Seconds",           &OrderBookMetricsEntry::differenceDepthVolatilityImbalance1Seconds)
        .def_readonly("differenceDepthVolatilityImbalance3Seconds",           &OrderBookMetricsEntry::differenceDepthVolatilityImbalance3Seconds)
        .def_readonly("differenceDepthVolatilityImbalance5Seconds",           &OrderBookMetricsEntry::differenceDepthVolatilityImbalance5Seconds)
        .def_readonly("differenceDepthVolatilityImbalance10Seconds",          &OrderBookMetricsEntry::differenceDepthVolatilityImbalance10Seconds)
        .def_readonly("differenceDepthVolatilityImbalance15Seconds",          &OrderBookMetricsEntry::differenceDepthVolatilityImbalance15Seconds)
        .def_readonly("differenceDepthVolatilityImbalance30Seconds",          &OrderBookMetricsEntry::differenceDepthVolatilityImbalance30Seconds)
        .def_readonly("differenceDepthVolatilityImbalance60Seconds",          &OrderBookMetricsEntry::differenceDepthVolatilityImbalance60Seconds)

        .def_readonly("rsi5Seconds",                                          &OrderBookMetricsEntry::rsi5Seconds)
        .def_readonly("stochRsi5Seconds",                                     &OrderBookMetricsEntry::stochRsi5Seconds)
        .def_readonly("macd2Seconds",                                         &OrderBookMetricsEntry::macd2Seconds)

        .def("__str__", [](const OrderBookMetricsEntry &entry) {
            std::ostringstream oss;
            // oss << std::fixed << std::setprecision(5);
            oss
            << "timestampOfReceive: " << entry.timestampOfReceive << " "
            << "market: " << static_cast<int>(entry.market) << " "
            << "symbol: " << static_cast<int>(entry.symbol) << " "
            << "bestAskPrice: " << entry.bestAskPrice << " "
            << "bestBidPrice: " << entry.bestBidPrice << " "
            << "midPrice: " << entry.midPrice << " "
            << "bestVolumeImbalance: " << entry.bestVolumeImbalance << " "
            << "bestVolumeRatio: " << entry.bestVolumeRatio << " "
            << "bestTwoVolumeImbalance: " << entry.bestTwoVolumeImbalance << " "
            << "bestThreeVolumeImbalance: " << entry.bestThreeVolumeImbalance << " "
            << "bestFiveVolumeImbalance: " << entry.bestFiveVolumeImbalance << " "
            << "volumeImbalance: " << entry.volumeImbalance << " "
            << "queueImbalance: " << entry.queueImbalance << " "
            << "gap: " << entry.gap << " "
            << "isAggressorAsk: " << entry.isAggressorAsk << " "
            << "vwapDeviation: " << entry.vwapDeviation << " "
            << "simplifiedSlopeImbalance: " << entry.simplifiedSlopeImbalance << " "

            << "tradeCountImbalance1Seconds: " << entry.tradeCountImbalance1Seconds << " "
            << "tradeCountImbalance3Seconds: " << entry.tradeCountImbalance3Seconds << " "
            << "tradeCountImbalance5Seconds: " << entry.tradeCountImbalance5Seconds << " "
            << "tradeCountImbalance10Seconds: " << entry.tradeCountImbalance10Seconds << " "
            << "tradeCountImbalance15Seconds: " << entry.tradeCountImbalance15Seconds << " "
            << "tradeCountImbalance30Seconds: " << entry.tradeCountImbalance30Seconds << " "
            << "tradeCountImbalance60Seconds: " << entry.tradeCountImbalance60Seconds << " "

            << "cumulativeDelta1Seconds: " << entry.cumulativeDelta1Seconds << " "
            << "cumulativeDelta3Seconds: " << entry.cumulativeDelta3Seconds << " "
            << "cumulativeDelta5Seconds: " << entry.cumulativeDelta5Seconds << " "
            << "cumulativeDelta10Seconds: " << entry.cumulativeDelta10Seconds << " "
            << "cumulativeDelta15Seconds: " << entry.cumulativeDelta15Seconds << " "
            << "cumulativeDelta30Seconds: " << entry.cumulativeDelta30Seconds << " "
            << "cumulativeDelta60Seconds: " << entry.cumulativeDelta60Seconds

            << " priceDifference1Seconds: " << entry.priceDifference1Seconds
            << " priceDifference3Seconds: " << entry.priceDifference3Seconds
            << " priceDifference5Seconds: " << entry.priceDifference5Seconds
            << " priceDifference10Seconds: " << entry.priceDifference10Seconds
            << " priceDifference15Seconds: " << entry.priceDifference15Seconds
            << " priceDifference30Seconds: " << entry.priceDifference30Seconds
            << " priceDifference60Seconds: " << entry.priceDifference60Seconds

            << " rateOfReturn1Seconds: "  << entry.rateOfReturn1Seconds
            << " rateOfReturn3Seconds: "  << entry.rateOfReturn3Seconds
            << " rateOfReturn5Seconds: "  << entry.rateOfReturn5Seconds
            << " rateOfReturn10Seconds: " << entry.rateOfReturn10Seconds
            << " rateOfReturn15Seconds: " << entry.rateOfReturn15Seconds
            << " rateOfReturn30Seconds: " << entry.rateOfReturn30Seconds
            << " rateOfReturn60Seconds: " << entry.rateOfReturn60Seconds

            << " differenceDepthVolatilityImbalance1Seconds: "  << entry.differenceDepthVolatilityImbalance1Seconds
            << " differenceDepthVolatilityImbalance3Seconds: "  << entry.differenceDepthVolatilityImbalance3Seconds
            << " differenceDepthVolatilityImbalance5Seconds: "  << entry.differenceDepthVolatilityImbalance5Seconds
            << " differenceDepthVolatilityImbalance10Seconds: " << entry.differenceDepthVolatilityImbalance10Seconds
            << " differenceDepthVolatilityImbalance15Seconds: " << entry.differenceDepthVolatilityImbalance15Seconds
            << " differenceDepthVolatilityImbalance30Seconds: " << entry.differenceDepthVolatilityImbalance30Seconds
            << " differenceDepthVolatilityImbalance60Seconds: " << entry.differenceDepthVolatilityImbalance60Seconds

            << " rsi5Seconds: "                                 << entry.rsi5Seconds
            << " stochRsi5Seconds: "                            << entry.stochRsi5Seconds
            << " macd2Seconds: "                                << entry.macd2Seconds;
            return oss.str();
        });

    // ----- OrderBookMetrics -----
    py::class_<OrderBookMetrics>(m, "OrderBookMetrics")
        .def(py::init<const std::vector<std::string>&>(), py::arg("variables"))
        .def("to_csv", &OrderBookMetrics::toCSV, py::arg("path"),
             "Zapisuje metryki do pliku CSV")
        .def_property_readonly("entries", &OrderBookMetrics::entries,
             "Lista wpisów OrderBookMetricsEntry");
        // .def_property_readonly("variables", [](const OrderBookMetrics &self) {
        //      return self.variables_;
        // }, "Lista nazw metryk");

    m.def("parse_mask", &parseMask, py::arg("variables"),
      "Parsuje listę nazw zmiennych na bitową MetricMask");

    // ----- OrderBookMetricsCalculator -----
    py::class_<OrderBookMetricsCalculator>(m, "OrderBookMetricsCalculator")
        .def(py::init<MetricMask>(), py::arg("mask"))
        .def("count_market_state_metrics",
             &OrderBookMetricsCalculator::countMarketStateMetrics,
             py::arg("market_state"),
             "Oblicza OrderBookMetricsEntry dla podanego MarketState i maski");

    // ----- Market -----
    py::enum_<Market>(m, "Market")
        .value("SPOT",          Market::SPOT)
        .value("USD_M_FUTURES", Market::USD_M_FUTURES)
        .value("COIN_M_FUTURES",Market::COIN_M_FUTURES)
        .value("UNKNOWN",       Market::UNKNOWN)
        .export_values();

    py::enum_<Symbol>(m, "Symbol")
        .value("UNKNOWN",   Symbol::UNKNOWN)
        .value("BTCUSDT",   Symbol::BTCUSDT)
        .value("ETHUSDT",   Symbol::ETHUSDT)
        .value("BNBUSDT",   Symbol::BNBUSDT)
        .value("SOLUSDT",   Symbol::SOLUSDT)
        .value("XRPUSDT",   Symbol::XRPUSDT)
        .value("DOGEUSDT",  Symbol::DOGEUSDT)
        .value("ADAUSDT",   Symbol::ADAUSDT)
        .value("SHIBUSDT",  Symbol::SHIBUSDT)
        .value("LTCUSDT",   Symbol::LTCUSDT)
        .value("AVAXUSDT",  Symbol::AVAXUSDT)
        .value("TRXUSDT",   Symbol::TRXUSDT)
        .value("DOTUSDT",   Symbol::DOTUSDT)
        .value("BCHUSDT",   Symbol::BCHUSDT)
        .value("SUIUSDT",   Symbol::SUIUSDT)
        .export_values();

    py::enum_<StreamType>(m, "StreamType")
        .value("DIFFERENCE_DEPTH_STREAM", StreamType::DIFFERENCE_DEPTH_STREAM)
        .value("TRADE_STREAM",            StreamType::TRADE_STREAM)
        .value("DEPTH_SNAPSHOT",          StreamType::DEPTH_SNAPSHOT)
        .value("FINAL_DEPTH_SNAPSHOT",    StreamType::FINAL_DEPTH_SNAPSHOT)
        .export_values();

}
