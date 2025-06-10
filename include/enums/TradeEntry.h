#pragma once

#include <cstdint>
#include <string>

#include "symbol.h"
#include "enums/Market.h"

struct TradeEntry {
    int64_t TimestampOfReceive;
    Symbol symbol;
    double Price;
    double Quantity;
    bool IsBuyerMarketMaker;
    std::string MUnknownParameter;
    std::string XUnknownParameter;
    bool IsLast;
    Market Market_;

    TradeEntry(
        int64_t TimestampOfReceive,
        Symbol Symbol,
        double Price,
        double Quantity,
        bool IsBuyerMarketMaker,
        const std::string& MUnknownParameter,
        const std::string& XUnknownParameter,
        bool IsLast,
        Market Market_
    )
    : TimestampOfReceive(TimestampOfReceive)
    , symbol(Symbol)
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
