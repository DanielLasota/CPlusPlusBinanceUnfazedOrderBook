#pragma once

#include <cstdint>

#include "enums/symbol.h"
#include "enums/Market.h"

struct DifferenceDepthEntry {
    int64_t TimestampOfReceive;
    Symbol symbol;
    bool IsAsk;
    double Price;
    double Quantity;
    bool IsLast;
    Market Market_;
    DifferenceDepthEntry* prev_;
    DifferenceDepthEntry* next_;

    DifferenceDepthEntry(
        int64_t TimestampOfReceive,
        Symbol Symbol,
        bool IsAsk,
        double Price,
        double Quantity,
        bool IsLast,
        Market Market_,
        DifferenceDepthEntry* prev_ = nullptr,
        DifferenceDepthEntry* next_ = nullptr
    )
    : TimestampOfReceive(TimestampOfReceive)
    , symbol(Symbol)
    , IsAsk(IsAsk)
    , Price(Price)
    , Quantity(Quantity)
    , IsLast(IsLast)
    , Market_(Market_)
    , prev_(prev_)
    , next_(next_)
    {}

    DifferenceDepthEntry() = default;

};
