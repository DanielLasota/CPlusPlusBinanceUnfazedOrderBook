#pragma once

#include <vector>
#include <string>
#include <OrderBookMetricsEntry.h>

class OrderBookMetrics {
public:
    OrderBookMetrics() = default;

    void reserve(size_t n) {
        entries_.reserve(n);
    }

    void addEntry(const OrderBookMetricsEntry& entry) {
        entries_.push_back(entry);
    }

    const std::vector<OrderBookMetricsEntry>& entries() const {
        return entries_;
    }

    void toCSV(const std::string& path) const;

private:
    std::vector<OrderBookMetricsEntry> entries_;
};