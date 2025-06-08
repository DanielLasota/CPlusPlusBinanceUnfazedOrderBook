#pragma once

#include <vector>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "OrderBookMetricsEntry.h"

class OrderBookMetricsArrays {
public:
    void reserve(size_t n);
    void addEntry(const OrderBookMetricsEntry &entry);
    pybind11::dict toNumpyDict(const std::vector<std::string> &variables);

private:
    std::vector<int64_t> timestampOfReceive_;
    std::vector<int> market_;
    std::vector<std::string> symbol_;
    std::vector<double> bestAskPrice_;
    std::vector<double> bestBidPrice_;
    std::vector<double> midPrice_;
    std::vector<double> bestVolumeImbalance_;
    std::vector<double> queueImbalance_;
    std::vector<double> volumeImbalance_;
    std::vector<double> gap_;
    std::vector<int8_t> isAggressorAsk_;
};

