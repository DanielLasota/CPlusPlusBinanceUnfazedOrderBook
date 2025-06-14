#pragma once

#include <vector>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "OrderBookMetricsEntry.h"

namespace py = pybind11;

class OrderBookMetrics {
public:
    explicit OrderBookMetrics(const std::vector<std::string>& variables)
        : variables_(variables) {}

    void reserve(size_t n) {
        entries_.reserve(n);
    }

    void addEntry(const OrderBookMetricsEntry& entry) {
        entries_.push_back(entry);
    }

    py::dict convertToNumpyArrays() const;

    const std::vector<OrderBookMetricsEntry>& entries() const {
        return entries_;
    }

    void toCSV(const std::string& path) const;

private:
    std::vector<OrderBookMetricsEntry> entries_;
    std::vector<std::string> variables_;
};