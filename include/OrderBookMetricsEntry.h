#pragma once

struct OrderBookMetricsEntry {
    double bestAsk;
    double bestBid;
    double midPrice;
    double bestVolumeImbalance;
    double queueImbalance;
    double volumeImbalance;
    double gap;
    bool isAggressorAsk;
};
