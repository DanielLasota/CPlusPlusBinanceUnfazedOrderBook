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
    std::cout << std::fixed << std::setprecision(5);

    // ----- OrderbookSessionSimulator -----
    py::class_<OrderBookSessionSimulator>(m, "OrderBookSessionSimulator")
        .def(py::init<>())
        .def("compute_backtest", &OrderBookSessionSimulator::computeBacktest,
             py::arg("csv_path"), py::arg("variables"), py::arg("python_callback") = py::none(),
             "Uruchamia backtest; zwraca void")
        .def("compute_final_depth_snapshot", &OrderBookSessionSimulator::computeFinalDepthSnapshot,
             py::arg("csv_path"),
             "Zwraca FinalOrderBookSnapshot")
        .def("compute_variables", &OrderBookSessionSimulator::computeVariables,
             py::arg("csv_path"), py::arg("variables"),
             "Oblicza metryki z pliku CSV i zwraca OrderBookMetrics")
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
             py::arg("symbol"), py::arg("market_str"),
             py::return_value_policy::reference_internal,
             "Zwraca obiekt MarketState dla danego symbolu i rynku")
            .def("market_state_count", &GlobalMarketState::marketStateCount,
                 "Liczba przechowywanych (symbol, market)")
            .def("__len__", &GlobalMarketState::marketStateCount)
            .def("market_state_list", &GlobalMarketState::marketStateList,
                 "Zwraca listę (symbol, market) dostępnych w GlobalMarketState")
        ;


    // ----- MarketState -----
    py::class_<MS>(m, "MarketState")
        .def(py::init<>(), "Tworzy nowy MarketState")
        .def_readonly("order_book", &MS::orderBook, py::return_value_policy::reference_internal)
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
        .def("do_nothing",
            &MS::doNothing,
            "xD it does nothing"
            )
        ;

    // ----- OrderBook -----
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("print_order_book", &OrderBook::printOrderBook)
        .def("asks",          &OrderBook::getAsks,
             "Return list of all ask levels in the book, in linked-list order")
        .def("bids",          &OrderBook::getBids,
             "Return list of all bid levels in the book, in linked-list order")
        .def("ask_count",           &OrderBook::askCount,
             "Number of ask levels in the book")
        .def("bid_count",           &OrderBook::bidCount,
             "Number of bid levels in the book")
        .def("sum_ask_quantity",    &OrderBook::sumAskQuantity,
             "Total quantity on the ask side")
        .def("sum_bid_quantity",    &OrderBook::sumBidQuantity,
             "Total quantity on the bid side")
        .def("best_ask_price",      &OrderBook::bestAskPrice,
             "Price of the best (lowest) ask")
        .def("best_bid_price",      &OrderBook::bestBidPrice,
             "Price of the best (highest) bid")
        .def("best_ask_quantity",   &OrderBook::bestAskQuantity,
             "Quantity available at the best ask")
        .def("best_bid_quantity",   &OrderBook::bestBidQuantity,
             "Quantity available at the best bid")
        .def("second_ask_price",    &OrderBook::secondAskPrice,
             "Price of the second-best ask")
        .def("second_bid_price",    &OrderBook::secondBidPrice,
             "Price of the second-best bid")
        .def("update",
             &OrderBook::update,
             py::arg("entry"),
             "Apply a single DifferenceDepthEntry update to this order book")
        ;

    // ----- SingleVariableCounter -----
    auto svc = m.def_submodule("single_variable_counter", "Compute single-variable order book metrics");
    svc.def("calculate_best_ask_price",        &SingleVariableCounter::calculateBestAskPrice,        py::arg("order_book"));
    svc.def("calculate_best_bid_price",        &SingleVariableCounter::calculateBestBidPrice,        py::arg("order_book"));
    svc.def("calculate_mid_price",             &SingleVariableCounter::calculateMidPrice,            py::arg("order_book"));
    svc.def("calculate_best_volume_imbalance", &SingleVariableCounter::calculateBestVolumeImbalance, py::arg("order_book"));
    svc.def("calculate_queue_imbalance",       &SingleVariableCounter::calculateQueueImbalance,      py::arg("order_book"));
    svc.def("calculate_volume_imbalance",      &SingleVariableCounter::calculateVolumeImbalance,     py::arg("order_book"));
    svc.def("calculate_gap",                   &SingleVariableCounter::calculateGap,                 py::arg("order_book"));
    svc.def("calculate_is_aggressor_ask",
            [](const TradeEntry &t){ return SingleVariableCounter::calculateIsAggressorAsk(&t); },
            py::arg("trade_entry"));

    // ----- DifferenceDepthEntry (DifferenceDepthEntry) -----
    py::class_<DifferenceDepthEntry>(m, "DifferenceDepthEntry")
        .def(py::init<>())
        .def_readwrite("timestamp_of_receive", &DifferenceDepthEntry::TimestampOfReceive)
        .def_readwrite("stream", &DifferenceDepthEntry::Stream)
        .def_readwrite("event_type", &DifferenceDepthEntry::EventType)
        .def_readwrite("event_time", &DifferenceDepthEntry::EventTime)
        .def_readwrite("transaction_time", &DifferenceDepthEntry::TransactionTime)
        .def_readwrite("symbol", &DifferenceDepthEntry::Symbol)
        .def_readwrite("first_update_id", &DifferenceDepthEntry::FirstUpdateId)
        .def_readwrite("final_update_id", &DifferenceDepthEntry::FinalUpdateId)
        .def_readwrite("final_update_id_in_last_stream", &DifferenceDepthEntry::FinalUpdateIdInLastStream)
        .def_readwrite("is_ask", &DifferenceDepthEntry::IsAsk)
        .def_readwrite("price", &DifferenceDepthEntry::Price)
        .def_readwrite("quantity", &DifferenceDepthEntry::Quantity)
        .def_readwrite("ps_unknown_field", &DifferenceDepthEntry::PSUnknownField)
        .def_readwrite("is_last", &DifferenceDepthEntry::IsLast)
        .def_readwrite("market", &DifferenceDepthEntry::Market_)
        .def("__str__", [](const DifferenceDepthEntry &entry) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(5);
            oss
            << "TimestampOfReceive: " << entry.TimestampOfReceive << " "
            << "Stream: " << entry.Stream << " "
            << "Symbol: " << entry.Symbol << " "
            << "IsAsk: " << entry.IsAsk << " "
            << "Price: " << entry.Price << " "
            << "Quantity: " << entry.Quantity << " "
            << "IsLast: " << entry.IsLast << " "
            << "Market_: " << entry.Market_ << " ";
            return oss.str();
        })
        .def("to_list", [](const DifferenceDepthEntry &e) {
            py::list v;
            v.append(e.TimestampOfReceive);
            v.append(e.Stream);
            v.append(e.EventType);
            v.append(e.EventTime);
            v.append(e.TransactionTime);
            v.append(e.Symbol);
            v.append(e.FirstUpdateId);
            v.append(e.FinalUpdateId);
            v.append(e.FinalUpdateIdInLastStream);
            v.append(e.IsAsk ? 1 : 0);
            v.append(e.Price);
            v.append(e.Quantity);
            v.append(e.PSUnknownField);
            v.append(e.IsLast ? 1 : 0);
            v.append(e.Market_);
            return v;
        })
        .def_static("field_names", []() {
            return std::vector<std::string>{
                "timestamp_of_receive",
                "stream",
                "event_type",
                "event_time",
                "transaction_time",
                "symbol",
                "first_update_id",
                "final_update_id",
                "final_update_id_in_last_stream",
                "is_ask",
                "price",
                "quantity",
                "ps_unknown_field",
                "is_last",
                "market"
            };
        });

    // ----- TradeEntry -----
    py::class_<TradeEntry>(m, "TradeEntry")
        .def(py::init<>())
        .def_readwrite("timestamp_of_receive",    &TradeEntry::TimestampOfReceive)
        .def_readwrite("stream",                  &TradeEntry::Stream)
        .def_readwrite("event_type",              &TradeEntry::EventType)
        .def_readwrite("event_time",              &TradeEntry::EventTime)
        .def_readwrite("transaction_time",        &TradeEntry::TransactionTime)
        .def_readwrite("symbol",                  &TradeEntry::Symbol)
        .def_readwrite("trade_id",                &TradeEntry::TradeId)
        .def_readwrite("price",                   &TradeEntry::Price)
        .def_readwrite("quantity",                &TradeEntry::Quantity)
        .def_readwrite("is_buyer_market_maker",   &TradeEntry::IsBuyerMarketMaker)
        .def_readwrite("m_unknown_parameter",     &TradeEntry::MUnknownParameter)
        .def_readwrite("x_unknown_parameter",     &TradeEntry::XUnknownParameter)
        .def_readwrite("is_last",                 &TradeEntry::IsLast)
        .def_readwrite("market",                  &TradeEntry::Market_)
        .def("__str__", [](const TradeEntry &entry) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(5);
            oss
            << "TimestampOfReceive: " << entry.TimestampOfReceive << " "
            << "Stream: " << entry.Stream << " "
            << "Symbol: " << entry.Symbol << " "
            << "TradeId: " << entry.TradeId << " "
            << "Price: " << entry.Price << " "
            << "Quantity: " << entry.Quantity << " "
            << "IsBuyerMarketMaker: " << entry.IsBuyerMarketMaker << " "
            << "IsLast: " << entry.IsLast << " "
            << "Market_: " << entry.Market_ << " ";
            return oss.str();
        })
        .def("to_list", [](const TradeEntry &e) {
            py::list v;
            v.append(e.TimestampOfReceive);
            v.append(e.Stream);
            v.append(e.EventType);
            v.append(e.EventTime);
            v.append(e.TransactionTime);
            v.append(e.Symbol);
            v.append(e.TradeId);
            v.append(e.Price);
            v.append(e.Quantity);
            v.append(e.IsBuyerMarketMaker ? 1 : 0);
            v.append(e.MUnknownParameter);
            v.append(e.XUnknownParameter);
            v.append(e.IsLast ? 1 : 0);
            v.append(e.Market_);
            return v;
        })
        .def_static("field_names", []() {
            return std::vector<std::string>{
                "timestamp_of_receive",
                "stream",
                "event_type",
                "event_time",
                "transaction_time",
                "symbol",
                "trade_id",
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

    // ----- OrderBookMetricsEntry -----
    py::class_<OrderBookMetricsEntry>(m, "OrderBookMetricsEntry")
        .def_readonly("timestampOfReceive",     &OrderBookMetricsEntry::timestampOfReceive)
        .def_readonly("bestAskPrice",           &OrderBookMetricsEntry::bestAskPrice)
        .def_readonly("bestBidPrice",           &OrderBookMetricsEntry::bestBidPrice)
        .def_readonly("midPrice",               &OrderBookMetricsEntry::midPrice)
        .def_readonly("bestVolumeImbalance",    &OrderBookMetricsEntry::bestVolumeImbalance)
        .def_readonly("queueImbalance",         &OrderBookMetricsEntry::queueImbalance)
        .def_readonly("volumeImbalance",        &OrderBookMetricsEntry::volumeImbalance)
        .def_readonly("gap",                    &OrderBookMetricsEntry::gap)
        .def_readonly("isAggressorAsk",         &OrderBookMetricsEntry::isAggressorAsk)
        .def("__str__", [](const OrderBookMetricsEntry &entry) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(5);
            oss
            << "timestampOfReceive: " << entry.timestampOfReceive << " "
            << "bestAskPrice: " << entry.bestAskPrice << " "
            << "bestBidPrice: " << entry.bestBidPrice << " "
            << "midPrice: " << entry.midPrice << " "
            << "bestVolumeImbalance: " << entry.bestVolumeImbalance << " "
            << "queueImbalance: " << entry.queueImbalance << " "
            << "volumeImbalance: " << entry.volumeImbalance << " "
            << "gap: " << entry.gap << " "
            << "isAggressorAsk: " << entry.isAggressorAsk << " ";
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
        .export_values();
}
