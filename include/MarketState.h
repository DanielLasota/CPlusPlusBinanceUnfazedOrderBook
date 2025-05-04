#pragma once

#include <memory>
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

    std::unique_ptr<OrderBook> orderBook;

private:
    int64_t lastTimestampOfReceive;

    const TradeEntry* lastTradePtr;

    bool hasLastTrade;

    void reserveMemory(size_t size);
};
