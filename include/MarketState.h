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

    void updateTradeRegistry(int64_t timestampOfReceive, double price, double quantity, bool isBuyerMM);

    void doNothing();

    OrderBook orderBook;

    bool getHasLastTrade() const {
        return hasLastTrade;
    }

    const TradeEntry& getLastTrade() const {
        if (!hasLastTrade || lastTradePtr == nullptr) {
            throw std::runtime_error("missing lastTradeEntry");
        }
        return *lastTradePtr;
    }

    uint64_t getLastTimestampOfReceive() const {
        return lastTimestampOfReceive;
    }

private:
    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTrade;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};

    void reserveMemory(size_t size);
};
