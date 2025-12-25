#pragma once

#include <string>
#include <vector>
#include <pybind11/pybind11.h>
#include "MetricsEntry.h"

namespace py = pybind11;

class MetricsStore {
public:
    explicit MetricsStore(const std::vector<std::string>& features, const size_t expectedCount)
        : features_(features) { entries_.reserve(expectedCount); }

    void update(const MetricsEntry& entry) { entries_.push_back(entry); }

    [[nodiscard]] const std::vector<MetricsEntry>& entries() const { return entries_; }

    [[nodiscard]] py::dict toNp() const;

    void toCSV(const std::string& path) const;

private:
    std::vector<MetricsEntry> entries_;
    std::vector<std::string> features_;
};
