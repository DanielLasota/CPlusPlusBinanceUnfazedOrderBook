#pragma once

#include <vector>
#include <string>
#include <pybind11/pybind11.h>
#include "OrderBookMetricsEntry.h"

namespace py = pybind11;

class OrderBookMetrics {
public:
    explicit OrderBookMetrics(const std::vector<std::string>& variables, const size_t expected_count)
        : variables_(variables) { entries_.reserve(expected_count); }

    void addOrderBookMetricsEntry(const OrderBookMetricsEntry& entry) { entries_.push_back(entry); }

    [[nodiscard]] const std::vector<OrderBookMetricsEntry>& entries() const { return entries_; }

    [[nodiscard]] py::dict convertToNumpyArrays() const;

    void toCSV(const std::string& path) const;

private:
    std::vector<OrderBookMetricsEntry> entries_;
    std::vector<std::string> variables_;
};