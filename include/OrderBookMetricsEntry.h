#pragma once
#include <cstdint>

struct OrderBookMetricsEntry {
    int64_t timestampOfReceive;
    double bestAsk;
    double bestBid;
    double midPrice;
    double bestVolumeImbalance;
    double queueImbalance;
    double volumeImbalance;
    double gap;
    bool isAggressorAsk;
};
