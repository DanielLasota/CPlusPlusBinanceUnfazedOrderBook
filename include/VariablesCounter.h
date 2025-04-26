#ifndef VARIABLESCOUNTER_H
#define VARIABLESCOUNTER_H

#include <enums/TradeEntry.h>
#include <vector>
#include "OrderBook.h"

class VariablesCounter {
public:
    explicit VariablesCounter(size_t expectedSize);

    void update(const OrderBook& orderbook, const TradeEntry* trade = nullptr);
    void saveVariablesListToCSV(const std::string& filename) const;

private:
    TradeEntry  lastTrade;
    bool hasLastTrade = false;

    std::vector<double> bestAsk;
    std::vector<double> bestBid;
    std::vector<double> midPrice;

    std::vector<double> bestVolumeImbalance;
    std::vector<double> queueImbalance;
    std::vector<double> volumeImbalance;

    std::vector<double> gap;

    std::vector<bool> isAgressorAsk;

    void reserveMemory(size_t size);
};

#endif // VARIABLESCOUNTER_H
