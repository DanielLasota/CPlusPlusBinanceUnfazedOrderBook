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
        .def_readonly("rolling_trade_statistics",
              &MS::rollingTradeStatistics,
              py::return_value_policy::reference_internal,
              "Statystyki obrotu w oknie")
        .def_readonly("rolling_difference_depth_statistics",
                  &MS::rollingDifferenceDepthStatistics,
                  py::return_value_policy::reference_internal,
                  "Statystyki głębokości w oknie")
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

    svc.def("calculate_rsi",                                        &SingleVariableCounter::calculateRSI,                                                   py::arg("rolling_statistics_data"), py::arg("startWindowTimeSeconds"), py::arg("windowTimeSeconds"));
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
            bool,
            Market
            >(),
            py::arg("timestamp_of_receive"),
            py::arg("symbol"),
            py::arg("price"),
            py::arg("quantity"),
            py::arg("is_buyer_market_maker"),
            py::arg("is_last"),
            py::arg("market")
        )
        .def_readwrite("timestamp_of_receive",    &TradeEntry::timestampOfReceive)
        .def_readwrite("symbol",                  &TradeEntry::symbol)
        .def_readwrite("price",                   &TradeEntry::price)
        .def_readwrite("quantity",                &TradeEntry::quantity)
        .def_readwrite("is_buyer_market_maker",   &TradeEntry::isBuyerMarketMaker)
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
                "is_last",
                "market"
            };
        })
    ;

    // ----- RollingTradeStatistics -----
    py::class_<RollingTradeStatistics>(m, "RollingTradeStatistics")
        .def(py::init<>())
        .def("update",
             &RollingTradeStatistics::update,
             py::arg("trade_entry"),
             "Dodaje nowy TradeEntry do statystyk")
        .def("buy_trade_count",
             &RollingTradeStatistics::buyTradeCount,
             py::arg("windowTimeSeconds"),
             "Liczba kupna w oknie [s]")
        .def("sell_trade_count",
             &RollingTradeStatistics::sellTradeCount,
             py::arg("windowTimeSeconds"),
             "Liczba sprzedaży w oknie [s]")
        .def("buy_trade_volume",
             &RollingTradeStatistics::buyTradeVolume,
             py::arg("windowTimeSeconds"),
             "Wolumen kupna w oknie [s]")
        .def("sell_trade_volume",
             &RollingTradeStatistics::sellTradeVolume,
             py::arg("windowTimeSeconds"),
             "Wolumen sprzedaży w oknie [s]")
        .def("price_difference",
             &RollingTradeStatistics::priceDifference,
             py::arg("windowTimeSeconds"),
             "Różnica ceny w oknie [s]")
        .def("oldest_price",
             &RollingTradeStatistics::oldestPrice,
             py::arg("windowTimeSeconds"),
             "Najstarsza cena w oknie [s]")
        .def("simple_moving_average",
             &RollingTradeStatistics::simpleMovingAverage,
             py::arg("windowTimeSeconds"),
             "Prosta średnia ruchoma ceny w oknie [s]")
        ;

    // ----- RollingDifferenceDepthStatistics -----
    py::class_<RollingDifferenceDepthStatistics>(m, "RollingDifferenceDepthStatistics")
        .def(py::init<>())
        .def("update",
             &RollingDifferenceDepthStatistics::update,
             py::arg("depth_entry"),
             "Dodaje nowy DifferenceDepthEntry do statystyk")
        .def("bid_difference_depth_entry_count",
             &RollingDifferenceDepthStatistics::bidDifferenceDepthEntryCount,
             py::arg("windowTimeSeconds"),
             "Liczba bid‐entry w oknie [s]")
        .def("ask_difference_depth_entry_count",
             &RollingDifferenceDepthStatistics::askDifferenceDepthEntryCount,
             py::arg("windowTimeSeconds"),
             "Liczba ask‐entry w oknie [s]")
        ;

    // ----- OrderBookMetricsEntry -----
    py::class_<OrderBookMetricsEntry>(m, "OrderBookMetricsEntry")
        #define METRIC(name, ctype) \
            .def_readonly(#name, &OrderBookMetricsEntry::name)
        #include "detail/metrics_list.def"
        #undef METRIC

        .def("__str__", [](const OrderBookMetricsEntry &entry) {
            std::ostringstream oss;

            auto append_field = [&](auto const& field, const char* nm) {
                using T = std::decay_t<decltype(field)>;
                oss << nm << ": ";
                if constexpr (std::is_same_v<T, bool>) {
                    oss << (field ? 1 : 0);
                } else if constexpr (std::is_same_v<T, uint8_t>) {
                    oss << static_cast<int>(field);
                } else {
                    oss << field;
                }
                oss << " ";
            };

            #define METRIC(name, ctype) append_field(entry.name, #name);
            #include "detail/metrics_list.def"
            #undef METRIC

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
