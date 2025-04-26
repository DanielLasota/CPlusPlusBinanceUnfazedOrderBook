#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <vector>
#include "enums/OrderBookEntry.h"

class OrderBook {
public:
    std::vector<DifferenceDepthEntry*> asks;
    std::vector<DifferenceDepthEntry*> bids;

    void addOrder(DifferenceDepthEntry* order);
    void printOrderBook() const;
};

#endif // ORDERBOOK_H
