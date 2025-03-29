#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "CSVParser.h"
#include <chrono>
#include <string>
#include <iostream>

namespace py = pybind11;

void processOrderbook(const std::string &csvPath) {
    OrderBook orderbook;
    try {
        auto entries = getOrderbookEntriesFromCSV(csvPath);

        auto start = std::chrono::steady_clock::now();

        for (auto &entry : entries) {
            orderbook.addOrder(entry);
        }

        auto finish = std::chrono::steady_clock::now();

        auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

        std::cout << "Start timestamp (ms): " << start_ms << std::endl;
        std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
        std::cout << "Elapsed: " << elapsed_ms << " ms" << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

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

    m.def("loadCSV", &getOrderbookEntriesFromCSV, "Load CSV into OrderBookEntry list", py::arg("csvPath"));

    m.def("processOrderbook", &processOrderbook, "Process entire orderbook from CSV", py::arg("csvPath"));
}
