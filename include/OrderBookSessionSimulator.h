#pragma once

#include <string>
#include <pybind11/pybind11.h>

#include "OrderBook.h"
#include "OrderBookMetricsEntry.h"

namespace py = pybind11;

class OrderBookSessionSimulator {
public:
    OrderBookSessionSimulator();

    std::vector<OrderBookMetricsEntry> computeVariables(const std::string &csvPath, std::vector<std::string> &variables);

    std::vector<OrderBookMetricsEntry> computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const py::object &python_callback = py::none());

    OrderBook computeFinalDepthSnapshot(const std::string &csvPath);

private:
};
