#pragma once

#include <cstdint>
#include <string>
#include "enums/Market.h"

struct TradeEntry {
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
    std::string MUnknownParameter;
    std::string XUnknownParameter;
    bool IsLast;
    Market Market_;

    TradeEntry(
        int64_t TimestampOfReceive,
        const std::string& Stream,
        const std::string& EventType,
        int64_t EventTime,
        int64_t TransactionTime,
        const std::string& Symbol,
        int64_t TradeId,
        double Price,
        double Quantity,
        bool IsBuyerMarketMaker,
        const std::string& MUnknownParameter,
        const std::string& XUnknownParameter,
        bool IsLast,
        Market Market_
    )
    : TimestampOfReceive(TimestampOfReceive)
    , Stream(Stream)
    , EventType(EventType)
    , EventTime(EventTime)
    , TransactionTime(TransactionTime)
    , Symbol(Symbol)
    , TradeId(TradeId)
    , Price(Price)
    , Quantity(Quantity)
    , IsBuyerMarketMaker(IsBuyerMarketMaker)
    , MUnknownParameter(MUnknownParameter)
    , XUnknownParameter(XUnknownParameter)
    , IsLast(IsLast)
    , Market_(Market_)
    {}
    TradeEntry() = default;
};
