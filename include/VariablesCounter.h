// ===== ./include/VariablesCounter.h =====
#ifndef VARIABLESCOUNTER_H
#define VARIABLESCOUNTER_H

#include <vector>
#include "OrderBook.h"

class VariablesCounter {
public:
    explicit VariablesCounter(size_t expectedSize);

    void update(const OrderBook& orderbook);
    void saveVariablesListToCSV(const std::string& filename) const;

private:
    std::vector<double> bestAsk;
    std::vector<double> bestBid;
    std::vector<double> midPrice;

    std::vector<double> bestVolumeImbalance;
    std::vector<double> queueImbalance;
    std::vector<double> volumeImbalance;

    std::vector<double> gap;

    void reserveMemory(size_t size);
};

#endif // VARIABLESCOUNTER_H
