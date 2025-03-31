#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <vector>
#include "OrderBookEntry.h"

class OrderBook {
public:
    std::vector<OrderBookEntry*> asks;
    std::vector<OrderBookEntry*> bids;

    void addOrder(OrderBookEntry* order);
    void printOrderBook() const;
};

#endif // ORDERBOOK_H
