#pragma once

#include <cstdint>

#include <enums/Market.h>

struct OrderBookMetricsEntry {
    int64_t timestampOfReceive;
    Market market;
    std::string symbol;
    double bestAskPrice;
    double bestBidPrice;
    double midPrice;
    double bestVolumeImbalance;
    double queueImbalance;
    double volumeImbalance;
    double gap;
    bool isAggressorAsk;
};
