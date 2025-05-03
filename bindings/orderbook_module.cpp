// ===== ./bindings/orderbook_module.cpp =====
#include <FinalOrderBookSnapshot.h>
#include <sstream>
#include <TradeEntry.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "OrderBookMetricsEntry.h"
#include "OrderBookMetrics.h"
#include "OrderBookSessionSimulator.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_binance_orderbook, m) {
    // ----- DifferenceDepthEntry (OrderBookEntry) -----
    py::class_<DifferenceDepthEntry>(m, "OrderBookEntry")
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
            oss << "<OrderBookEntry "
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

    // ----- OrderBook -----
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("add_order", &OrderBook::addOrder, "Dodaje lub usuwa zlecenie")
        .def("print_order_book", &OrderBook::printOrderBook, "Wypisuje stan orderbooka")
        .def_readonly("asks", &OrderBook::asks)
        .def_readonly("bids", &OrderBook::bids);

    // ----- FinalOrderBookSnapshot -----
    py::class_<FinalDifferenceDepthSnapshot>(m, "FinalOrderBookSnapshot")
        .def(py::init<>())
        .def_readonly("bids", &FinalDifferenceDepthSnapshot::bids)
        .def_readonly("asks", &FinalDifferenceDepthSnapshot::asks)
        .def("__repr__", [](const FinalDifferenceDepthSnapshot &s) {
            return "<FinalOrderBookSnapshot bids=" + std::to_string(s.bids.size())
                 + " asks=" + std::to_string(s.asks.size()) + ">";
        });

    // ----- OrderBookMetricsEntry -----
    py::class_<OrderBookMetricsEntry>(m, "OrderBookMetricsEntry")
        .def_readonly("timestampOfReceive",            &OrderBookMetricsEntry::timestampOfReceive)
        .def_readonly("bestAsk",            &OrderBookMetricsEntry::bestAsk)
        .def_readonly("bestBid",            &OrderBookMetricsEntry::bestBid)
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
}
