#pragma once

#include "OrderBook.h"
#include "enums/TradeEntry.h"

class MarketState {
public:
    MarketState() = default;

    MarketState(const Market market_, const std::string& symbol_)
        : market_(market_), symbol_(symbol_), lastTrade()
    {}

    Market get_market() const { return market_; }

    const std::string& get_symbol() const { return symbol_; }

    void update(DecodedEntry* entry);

    void updateOrderBook(int64_t timestampOfReceive, double price, double quantity, bool isAsk);

    void updateTradeRegistry(int64_t timestampOfReceive, double price, double quantity, bool isBuyerMM);

    void doNothing();

    OrderBook orderBook;

    bool getHasLastTrade() const {return hasLastTrade;}

    const TradeEntry& getLastTrade() const {
        if (!hasLastTrade || lastTradePtr == nullptr) {
            throw std::runtime_error("missing lastTradeEntry");
        }
        return *lastTradePtr;
    }

    uint64_t getLastTimestampOfReceive() const {return lastTimestampOfReceive;}

private:
    Market market_;
    std::string symbol_;

    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTrade;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};

    void reserveMemory(size_t size);
};
