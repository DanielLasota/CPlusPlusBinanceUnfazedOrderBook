#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "OrderBookSessionSimulator.h"

namespace py = pybind11;

PYBIND11_MODULE(orderbook, m) {
    py::class_<OrderBookEntry>(m, "OrderBookEntry")
        .def(py::init<>())
        .def_readwrite("TimestampOfReceive", &OrderBookEntry::TimestampOfReceive)
        .def_readwrite("EventTime", &OrderBookEntry::EventTime)
        .def_readwrite("Symbol", &OrderBookEntry::Symbol)
        .def_readwrite("FirstUpdateId", &OrderBookEntry::FirstUpdateId)
        .def_readwrite("FinalUpdateId", &OrderBookEntry::FinalUpdateId)
        .def_readwrite("IsAsk", &OrderBookEntry::IsAsk)
        .def_readwrite("Price", &OrderBookEntry::Price)
        .def_readwrite("Quantity", &OrderBookEntry::Quantity);

    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("addOrder", &OrderBook::addOrder)
        .def("printOrderBook", &OrderBook::printOrderBook)
        .def_readonly("asks", &OrderBook::asks)
        .def_readonly("bids", &OrderBook::bids);

    py::class_<OrderbookSessionSimulator>(m, "OrderbookSessionSimulator")
        .def(py::init<>())
        .def("processOrderbook", &OrderbookSessionSimulator::processOrderbook,
             py::arg("csvPath"), py::arg("python_callback") = py::none(),
             "Przetwarza orderbook z pliku CSV, podobnie jak w funkcji main() w C++");
    // m.def("processOrderbook", &processOrderbook, "Process entire orderbook from CSV", py::arg("csvPath"));
}
