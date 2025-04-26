#ifndef MARKETSTATE_H
#define MARKETSTATE_H

#include <OrderBookMetrics.h>
#include <enums/TradeEntry.h>
#include "OrderBook.h"
#include <optional>

class MarketState {
public:
    explicit MarketState();

    std::optional<OrderBookMetrics> update(const OrderBook& orderbook, const TradeEntry* trade = nullptr);

private:
    TradeEntry  lastTrade;

    bool hasLastTrade = false;

    void reserveMemory(size_t size);
};

#endif // MARKETSTATE_H
