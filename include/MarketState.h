#pragma once

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

    OrderBook orderBook;

private:
    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTradeStorage;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};

    void reserveMemory(size_t size);
};
