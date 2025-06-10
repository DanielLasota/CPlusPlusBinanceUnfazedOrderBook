#pragma once

#include <cstdint>

#include "enums/symbol.h"
#include "enums/Market.h"

struct DifferenceDepthEntry {
    int64_t timestampOfReceive;
    Symbol symbol;
    bool isAsk;
    double price;
    double quantity;
    bool isLast;
    Market market;
    DifferenceDepthEntry* prev_;
    DifferenceDepthEntry* next_;

    DifferenceDepthEntry(
        int64_t timestampOfReceive,
        Symbol symbol,
        bool isAsk,
        double price,
        double quantity,
        bool isLast,
        Market market,
        DifferenceDepthEntry* prev_ = nullptr,
        DifferenceDepthEntry* next_ = nullptr
    )
    : timestampOfReceive(timestampOfReceive)
    , symbol(symbol)
    , isAsk(isAsk)
    , price(price)
    , quantity(quantity)
    , isLast(isLast)
    , market(market)
    , prev_(prev_)
    , next_(next_)
    {}

    DifferenceDepthEntry() = default;

};
