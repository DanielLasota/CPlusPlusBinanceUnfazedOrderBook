#ifndef ORDERBOOKMETRICS_H
#define ORDERBOOKMETRICS_H
#pragma once

struct OrderBookMetrics {
    double bestAsk;
    double bestBid;
    double midPrice;
    double bestVolumeImbalance;
    double queueImbalance;
    double volumeImbalance;
    double gap;
    bool isAggressorAsk;
};
#endif //ORDERBOOKMETRICS_H
