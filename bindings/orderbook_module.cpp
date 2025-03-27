// bindings/orderbook_module.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "OrderBookEntry.h"
#include "CSVParser.h"

namespace py = pybind11;

PYBIND11_MODULE(orderbook, m) {
    py::class_<OrderBookEntry>(m, "OrderBookEntry")
        .def_readonly("TimestampOfReceive", &OrderBookEntry::TimestampOfReceive)
        .def_readonly("EventTime", &OrderBookEntry::EventTime)
        .def_readonly("Symbol", &OrderBookEntry::Symbol)
        .def_readonly("FirstUpdateId", &OrderBookEntry::FirstUpdateId)
        .def_readonly("FinalUpdateId", &OrderBookEntry::FinalUpdateId)
        .def_readonly("IsAsk", &OrderBookEntry::IsAsk)
        .def_readonly("Price", &OrderBookEntry::Price)
        .def_readonly("Quantity", &OrderBookEntry::Quantity);

    m.def("loadCSV", &getOrderbookEntriesFromCSV, "Wczytuje plik CSV i zwraca listę wpisów OrderBookEntry", py::arg("directory"));
}
