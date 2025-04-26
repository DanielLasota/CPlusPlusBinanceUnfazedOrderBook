#ifndef ORDERBOOK_SESSION_SIMULATOR_H
#define ORDERBOOK_SESSION_SIMULATOR_H

#include <OrderBook.h>
#include <string>
#include <pybind11/pybind11.h>
#include "FinalOrderBookSnapshot.h"

namespace py = pybind11;

class OrderbookSessionSimulator {
public:
    OrderbookSessionSimulator();

    void computeBacktest(const std::string& csvPath, const py::object &python_callback = py::none());

    void computeVariables(const std::string &csvPath, std::vector<std::string> &variables);

    FinalOrderBookSnapshot computeFinalDepthSnapshot(const std::string &csvPath);

private:
    OrderBook orderBook;
};

#endif
