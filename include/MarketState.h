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

    void updateOrderBook(int64_t timestampOfReceive, double price, double quantity, bool isAsk);

    void updateTradeRegister(int64_t timestampOfReceive, double price, double quantity, bool isBuyerMM);

    std::optional<OrderBookMetricsEntry> countOrderBookMetrics(MetricMask mask) const;

    void doNothing();

    OrderBook orderBook;

private:
    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTradeStorage;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};

    void reserveMemory(size_t size);
};
