#pragma once

#include <string>
#include <pybind11/pybind11.h>

#include "OrderBook.h"

namespace py = pybind11;

class OrderBookSessionSimulator {
public:
    explicit OrderBookSessionSimulator();

    py::dict computeVariables(const std::string &csvPath, const std::vector<std::string> &variables);

    py::dict computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const py::object &python_callback = py::none());

    OrderBook computeFinalDepthSnapshot(const std::string &csvPath);
private:
    static size_t countOrderBookMetricsSize(const std::vector<DecodedEntry>& entries);
};
