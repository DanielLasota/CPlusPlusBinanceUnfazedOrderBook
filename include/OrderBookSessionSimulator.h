#ifndef ORDERBOOK_SESSION_SIMULATOR_H
#define ORDERBOOK_SESSION_SIMULATOR_H

#include "OrderBook.h"
#include <string>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class OrderbookSessionSimulator {
public:
    OrderbookSessionSimulator();

    void processOrderbook(const std::string& csvPath, const py::object &python_callback = py::none());

private:
    OrderBook orderbook;
};

#endif
