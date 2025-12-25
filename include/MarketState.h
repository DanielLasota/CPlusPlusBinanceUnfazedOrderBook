#pragma once

#include "DepthRoll.h"
#include "OrderBook.h"
#include "TradeRoll.h"
#include "enums/TradeEntry.h"

class MarketState {
public:
    MarketState() = default;

    MarketState(const Market market_, const Symbol symbol)
        : market(market_), symbol(symbol), lastTrade()
    {}

    TradeRoll tradeRoll;

    DepthRoll depthRoll;

    OrderBook orderBook;

    void update(XEvent* entry);

    void updateOrderBook(uint64_t timestampOfReceive, double price, double quantity, bool isAsk);

    void updateTradeRegistry(uint64_t timestampOfReceive, double price, double quantity, bool isBuyerMM);

    uint64_t getLastTimestampOfReceive() const { return lastTimestampOfReceive; }

    Market getMarket() const { return market; }

    Symbol getSymbol() const { return symbol; }

    bool getHasLastTrade() const { return hasLastTrade; }

    StreamType getLastStreamType() const { return lastStreamType; }

    const TradeEntry& getLastTrade() const {
        if (!hasLastTrade || lastTradePtr == nullptr) { throw std::runtime_error("missing lastTradeEntry"); }
        return *lastTradePtr;
    }


private:
    Market market;
    Symbol symbol;
    StreamType lastStreamType;

    uint64_t lastTimestampOfReceive{0};

    TradeEntry   lastTrade;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};
};
