#include <SingleVariableCounter.h>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "MarketState.h"
#include "TradeEntry.h"
#include "DifferenceDepthEntry.h"
#include "OrderBook.h"
#include "OrderBookMetricsEntry.h"
#include "OrderBookMetrics.h"
#include "OrderBookSessionSimulator.h"

namespace py = pybind11;
using MS = MarketState;

PYBIND11_MODULE(cpp_binance_orderbook, m) {

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
        .def("count_order_book_metrics",
             &MS::countOrderBookMetrics,
             py::arg("mask"),
             "Oblicza metryki zgodnie z MetricMask i zwraca Optional[OrderBookMetricsEntry]")
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

    // // --- PriceLevel ----
    // py::class_<OrderBook::PriceLevel>(m, "PriceLevel")
    //     .def_readonly("price",    &OrderBook::PriceLevel::price)
    //     .def_readonly("quantity", &OrderBook::PriceLevel::quantity)
    //     .def_readonly("is_ask",    &OrderBook::PriceLevel::isAsk)
    //     .def("__repr__", [](const OrderBook::PriceLevel &pl) {
    //         std::ostringstream o;
    //         o << "<PriceLevel price=" << pl.price
    //           << " qty=" << pl.quantity
    //           << " ask="  << std::boolalpha << pl.isAsk
    //           << ">";
    //         return o.str();
    //     });

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
        .def("__repr__", [](const DifferenceDepthEntry &entry) {
            std::ostringstream oss;
            oss << "<DifferenceDepthEntry "
                << "price=" << entry.Price << " qty=" << entry.Quantity << " is_ask=" << entry.IsAsk
                << ">";
            return oss.str();
        })
        .def("to_list", [](const DifferenceDepthEntry &entry) {
            py::list v;
            v.append(entry.TimestampOfReceive);
            v.append(entry.Stream);
            v.append(entry.EventType);
            v.append(entry.EventTime);
            v.append(entry.TransactionTime);
            v.append(entry.Symbol);
            v.append(entry.FirstUpdateId);
            v.append(entry.FinalUpdateId);
            v.append(entry.FinalUpdateIdInLastStream);
            v.append(entry.IsAsk ? 1 : 0);
            v.append(entry.Price);
            v.append(entry.Quantity);
            v.append(entry.PSUnknownField);
            v.append(entry.IsLast ? 1 : 0);
            return v;
        })
        .def_property_readonly("field_names", [](const DifferenceDepthEntry &) {
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
                "is_last"
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
        .def("__repr__", [](const TradeEntry &e) {
            std::ostringstream oss;
            oss << "<TradeEntry symbol=" << e.Symbol
                << " price=" << e.Price
                << " qty=" << e.Quantity
                << " is_buyer_maker=" << e.IsBuyerMarketMaker
                << ">";
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
            return v;
        })
        .def_property_readonly("field_names", [](const TradeEntry &) {
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
                "is_last"
            };
        })
    ;

    // ----- OrderBookMetricsEntry -----
    py::class_<OrderBookMetricsEntry>(m, "OrderBookMetricsEntry")
        .def_readonly("timestampOfReceive",            &OrderBookMetricsEntry::timestampOfReceive)
        .def_readonly("bestAsk",            &OrderBookMetricsEntry::bestAskPrice)
        .def_readonly("bestBid",            &OrderBookMetricsEntry::bestBidPrice)
        .def_readonly("midPrice",           &OrderBookMetricsEntry::midPrice)
        .def_readonly("bestVolumeImbalance",&OrderBookMetricsEntry::bestVolumeImbalance)
        .def_readonly("queueImbalance",     &OrderBookMetricsEntry::queueImbalance)
        .def_readonly("volumeImbalance",    &OrderBookMetricsEntry::volumeImbalance)
        .def_readonly("gap",                &OrderBookMetricsEntry::gap)
        .def_readonly("isAggressorAsk",     &OrderBookMetricsEntry::isAggressorAsk);

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
}
