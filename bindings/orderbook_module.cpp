#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "OrderBookSessionSimulator.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_binance_orderbook, m) {
    py::class_<DifferenceDepthEntry>(m, "OrderBookEntry")
        .def(py::init<>())
        .def_readwrite("TimestampOfReceive", &DifferenceDepthEntry::TimestampOfReceive)
        .def_readwrite("Stream", &DifferenceDepthEntry::Stream)
        .def_readwrite("EventType", &DifferenceDepthEntry::EventType)
        .def_readwrite("EventTime", &DifferenceDepthEntry::EventTime)
        .def_readwrite("TransactionTime", &DifferenceDepthEntry::TransactionTime)
        .def_readwrite("Symbol", &DifferenceDepthEntry::Symbol)
        .def_readwrite("FirstUpdateId", &DifferenceDepthEntry::FirstUpdateId)
        .def_readwrite("FinalUpdateId", &DifferenceDepthEntry::FinalUpdateId)
        .def_readwrite("FinalUpdateIdInLastStream", &DifferenceDepthEntry::FinalUpdateIdInLastStream)
        .def_readwrite("IsAsk", &DifferenceDepthEntry::IsAsk)
        .def_readwrite("Price", &DifferenceDepthEntry::Price)
        .def_readwrite("Quantity", &DifferenceDepthEntry::Quantity)
        .def_readwrite("PSUnknownField", &DifferenceDepthEntry::PSUnknownField)
        .def("__repr__", [](const DifferenceDepthEntry &entry) {
            std::ostringstream oss;
            oss << "TimestampOfReceive: " << entry.TimestampOfReceive << " ";
            oss << "Stream: " << entry.Stream << " ";
            oss << "EventType: " << entry.EventType << " ";
            oss << "EventTime: " << entry.EventTime << " ";
            oss << "TransactionTime: " << entry.TransactionTime << " ";
            oss << "Symbol: '" << entry.Symbol << "' ";
            oss << "FirstUpdateId: " << entry.FirstUpdateId << " ";
            oss << "FinalUpdateId: " << entry.FinalUpdateId << " ";
            oss << "FinalUpdateIdInLastStream: " << entry.FinalUpdateIdInLastStream << " ";
            oss << "IsAsk: " << (entry.IsAsk ? "true" : "false") << " ";
            oss << "Price: " << entry.Price << " ";
            oss << "Quantity: " << entry.Quantity << " ";
            oss << "PSUnknownField: " << entry.PSUnknownField;
            return oss.str();
    })
        .def("to_list", [](const DifferenceDepthEntry &entry) {
            py::list values;
            values.append(entry.TimestampOfReceive);
            values.append(entry.Stream);
            values.append(entry.EventType);
            values.append(entry.EventTime);
            values.append(entry.TransactionTime);
            values.append(entry.Symbol);
            values.append(entry.FirstUpdateId);
            values.append(entry.FinalUpdateId);
            values.append(entry.FinalUpdateIdInLastStream);
            values.append(entry.IsAsk ? 1 : 0);
            values.append(entry.Price);
            values.append(entry.Quantity);
            values.append(entry.PSUnknownField);
            return values;
        })
        .def_property_readonly("field_names", [](const DifferenceDepthEntry &) {
            return std::vector<std::string>{
                "TimestampOfReceive",
                "Stream",
                "EventType",
                "EventTime",
                "TransactionTime",
                "Symbol",
                "FirstUpdateId",
                "FinalUpdateId",
                "FinalUpdateIdInLastStream",
                "IsAsk",
                "Price",
                "Quantity",
                "PSUnknownField"
            };
        });

    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("addOrder", &OrderBook::addOrder)
        .def("printOrderBook", &OrderBook::printOrderBook)
        .def_readonly("asks", &OrderBook::asks)
        .def_readonly("bids", &OrderBook::bids);

    py::class_<FinalOrderBookSnapshot>(m, "FinalOrderBookSnapshot")
        .def(py::init<>())
        .def_readonly("bids", &FinalOrderBookSnapshot::bids)
        .def_readonly("asks", &FinalOrderBookSnapshot::asks)
        .def("__repr__", [](const FinalOrderBookSnapshot &snapshot) {
            std::ostringstream oss;
            oss << "FinalOrderBookSnapshot(bids: " << snapshot.bids.size()
                << ", asks: " << snapshot.asks.size() << ")";
            return oss.str();
        });

    py::class_<OrderbookSessionSimulator>(m, "OrderbookSessionSimulator")
        .def(py::init<>())
        .def("processOrderbook", &OrderbookSessionSimulator::computeBacktest,
             py::arg("csvPath"), py::arg("python_callback") = py::none(),
             "Przetwarza orderbook z pliku CSV, podobnie jak w funkcji main() w C++")
        .def("getFinalOrderBookSnapshot", &OrderbookSessionSimulator::computeFinalDepthSnapshot,
             py::arg("csvPath"),
             "Returns orderbook snapshot after whole utc z day")
    ;
}
