#ifndef ORDERBOOK_SESSION_SIMULATOR_H
#define ORDERBOOK_SESSION_SIMULATOR_H

#include "OrderBookMetrics.h"
#include "OrderBook.h"
#include "FinalOrderBookSnapshot.h"

#include <string>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class OrderbookSessionSimulator {
public:
    OrderbookSessionSimulator();

    std::vector<OrderBookMetrics> computeVariables(const std::string &csvPath, std::vector<std::string> &variables);

    void computeBacktest(const std::string& csvPath, const py::object &python_callback = py::none());

    FinalOrderBookSnapshot computeFinalDepthSnapshot(const std::string &csvPath);

private:
    OrderBook orderBook;
};

#endif
