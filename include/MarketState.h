#pragma once

#include "OrderBook.h"
#include "enums/TradeEntry.h"
#include "RollingDifferenceDepthStatistics.h"
#include "RollingTradeStatistics.h"

class MarketState {
public:
    MarketState() = default;

    RollingTradeStatistics rollingTradeStatistics;
    RollingDifferenceDepthStatistics rollingDifferenceDepthStatistics;

    MarketState(const Market market_, Symbol symbol)
        : market(market_), symbol(symbol), lastTrade()
    {}

    Market getMarket() const { return market; }

    Symbol getSymbol() const { return symbol; }

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
    Market market;
    Symbol symbol;

    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTrade;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};

    void reserveMemory(size_t size);
};
