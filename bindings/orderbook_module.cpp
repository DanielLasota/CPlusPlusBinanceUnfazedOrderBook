// ===== ./bindings/orderbook_module.cpp =====
#include <sstream>
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
                "ps_unknown_field"
            };
        });

    // ----- OrderBook -----
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("add_order", &OrderBook::addOrder, "Dodaje lub usuwa zlecenie")
        .def("print_order_book", &OrderBook::printOrderBook, "Wypisuje stan orderbooka")
        .def_readonly("asks", &OrderBook::asks)
        .def_readonly("bids", &OrderBook::bids);

    // ----- FinalOrderBookSnapshot -----
    py::class_<FinalOrderBookSnapshot>(m, "FinalOrderBookSnapshot")
        .def(py::init<>())
        .def_readonly("bids", &FinalOrderBookSnapshot::bids)
        .def_readonly("asks", &FinalOrderBookSnapshot::asks)
        .def("__repr__", [](const FinalOrderBookSnapshot &s) {
            return "<FinalOrderBookSnapshot bids=" + std::to_string(s.bids.size())
                 + " asks=" + std::to_string(s.asks.size()) + ">";
        });

    // ----- OrderBookMetricsEntry -----
    py::class_<OrderBookMetricsEntry>(m, "OrderBookMetricsEntry")
        .def_readonly("best_ask",             &OrderBookMetricsEntry::bestAsk)
        .def_readonly("best_bid",             &OrderBookMetricsEntry::bestBid)
        .def_readonly("mid_price",            &OrderBookMetricsEntry::midPrice)
        .def_readonly("best_volume_imbalance", &OrderBookMetricsEntry::bestVolumeImbalance)
        .def_readonly("queue_imbalance",      &OrderBookMetricsEntry::queueImbalance)
        .def_readonly("volume_imbalance",     &OrderBookMetricsEntry::volumeImbalance)
        .def_readonly("gap",                  &OrderBookMetricsEntry::gap)
        .def_readonly("is_aggressor_ask",     &OrderBookMetricsEntry::isAggressorAsk);

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
    py::class_<OrderbookSessionSimulator>(m, "OrderbookSessionSimulator")
        .def(py::init<>())
        .def("compute_backtest", &OrderbookSessionSimulator::computeBacktest,
             py::arg("csv_path"), py::arg("variables"), py::arg("python_callback") = py::none(),
             "Uruchamia backtest; zwraca void")
        .def("compute_final_depth_snapshot", &OrderbookSessionSimulator::computeFinalDepthSnapshot,
             py::arg("csv_path"),
             "Zwraca FinalOrderBookSnapshot")
        .def("compute_variables", &OrderbookSessionSimulator::computeVariables,
             py::arg("csv_path"), py::arg("variables"),
             "Oblicza metryki z pliku CSV i zwraca OrderBookMetrics")
    ;
}
