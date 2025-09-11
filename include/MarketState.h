#pragma once

#include "OrderBook.h"
#include "enums/TradeEntry.h"
#include "RollingDifferenceDepthStatistics.h"
#include "RollingTradeStatistics.h"

class MarketState {
public:
    MarketState() = default;

    MarketState(const Market market_, const Symbol symbol)
        : market(market_), symbol(symbol), lastTrade()
    {}

    RollingTradeStatistics rollingTradeStatistics;
    RollingDifferenceDepthStatistics rollingDifferenceDepthStatistics;

    OrderBook orderBook;

    void update(DecodedEntry* entry);

    void updateOrderBook(int64_t timestampOfReceive, double price, double quantity, bool isAsk);

    void updateTradeRegistry(int64_t timestampOfReceive, double price, double quantity, bool isBuyerMM);

    bool getHasLastTrade() const {return hasLastTrade;}

    uint64_t getLastTimestampOfReceive() const {return lastTimestampOfReceive;}

    Market getMarket() const { return market; }

    Symbol getSymbol() const { return symbol; }

    const TradeEntry& getLastTrade() const {
        if (!hasLastTrade || lastTradePtr == nullptr) {
            throw std::runtime_error("missing lastTradeEntry");
        }
        return *lastTradePtr;
    }


private:
    Market market;
    Symbol symbol;

    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTrade;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};
};
