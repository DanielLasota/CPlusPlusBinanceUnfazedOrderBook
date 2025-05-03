#ifndef MARKETSTATE_H
#define MARKETSTATE_H

#include <optional>

#include "MetricMask.h"
#include "OrderBook.h"
#include "EntryDecoder.h"
#include "OrderBookMetrics.h"
#include "enums/TradeEntry.h"

class MarketState {
public:
    explicit MarketState();

    void update(DecodedEntry* entry);

    std::optional<OrderBookMetricsEntry> countOrderBookMetrics(MetricMask mask) const;

    OrderBook  orderBook;

private:
    int64_t lastTimestampOfReceive;

    const TradeEntry* lastTradePtr;

    bool hasLastTrade;

    void reserveMemory(size_t size);
};

#endif // MARKETSTATE_H
