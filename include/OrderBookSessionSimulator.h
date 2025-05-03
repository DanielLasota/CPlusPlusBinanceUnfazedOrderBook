#pragma once

#include "OrderBook.h"
#include "OrderBookMetrics.h"

#include <string>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class OrderBookSessionSimulator {
public:
    OrderBookSessionSimulator();

    std::vector<OrderBookMetricsEntry> computeVariables(const std::string &csvPath, std::vector<std::string> &variables);

    void computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const py::object &python_callback = py::none());

    OrderBook computeFinalDepthSnapshot(const std::string &csvPath);

private:
};
