#ifndef TRADEENTRY_H
#define TRADEENTRY_H

#include <cstdint>
#include <string>
#include <variant>

struct TradeEntryBase {
    virtual ~TradeEntryBase() = default;
    int64_t TimestampOfReceive;
    std::string Stream;
    std::string EventType;
    int64_t EventTime;
    int64_t TransactionTime;
    std::string Symbol;
    int64_t TradeId;
    double Price;
    double Quantity;
    bool IsBuyerMarketMaker;
};

struct SpotTradeEntry : public TradeEntryBase {
    std::string MUnknownParameter;
};

struct USDMFuturesTradeEntry : public TradeEntryBase {
    std::string XUnknownParameter;
};

struct CoinMFuturesTradeEntry : public TradeEntryBase {
    std::string XUnknownParameter;
};

using TradeEntry = std::variant<
    SpotTradeEntry,
    USDMFuturesTradeEntry,
    CoinMFuturesTradeEntry
>;

#endif // TRADEENTRY_H


