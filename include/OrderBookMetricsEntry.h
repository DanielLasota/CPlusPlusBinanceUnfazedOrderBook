#pragma once
#include <cstdint>

struct OrderBookMetricsEntry {
    int64_t timestampOfReceive;
    double bestAskPrice;
    double bestBidPrice;
    double midPrice;
    double bestVolumeImbalance;
    double queueImbalance;
    double volumeImbalance;
    double gap;
    bool isAggressorAsk;
};
