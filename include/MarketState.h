#ifndef MARKETSTATE_H
#define MARKETSTATE_H

#include <optional>

#include "OrderBook.h"
#include "EntryDecoder.h"
#include "OrderBookMetrics.h"
#include "enums/TradeEntry.h"

class MarketState {
public:
    explicit MarketState();

    void update(DecodedEntry* entry);

    std::optional<OrderBookMetrics> countOrderBookMetrics() const;

    OrderBook  orderBook;

private:
    const TradeEntry* lastTradePtr;

    bool hasLastTrade;

    void reserveMemory(size_t size);
};

#endif // MARKETSTATE_H
