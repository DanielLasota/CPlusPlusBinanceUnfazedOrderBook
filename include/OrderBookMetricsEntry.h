#pragma once

#include <cstdint>

#include <enums/Market.h>
#include <enums/symbol.h>


struct OrderBookMetricsEntry {
    int64_t timestampOfReceive;
    uint8_t market;
    uint8_t symbol;
    double bestAskPrice;
    double bestBidPrice;
    double midPrice;
    double bestVolumeImbalance;
    double bestVolumeRatio;
    double bestTwoVolumeImbalance;
    double bestThreeVolumeImbalance;
    double bestFiveVolumeImbalance;
    double volumeImbalance;
    double queueImbalance;
    double gap;
    bool isAggressorAsk;
    double vwapDeviation;
    double simplifiedSlopeImbalance;
};
