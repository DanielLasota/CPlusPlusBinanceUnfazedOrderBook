#pragma once

#include <cstdint>
#include <string>

#include "symbol.h"
#include "enums/Market.h"

struct TradeEntry {
    int64_t timestampOfReceive;
    Symbol symbol;
    double price;
    double quantity;
    bool isBuyerMarketMaker;
    bool isLast;
    Market market;

    TradeEntry(
        int64_t timestampOfReceive,
        Symbol symbol,
        double price,
        double quantity,
        bool isBuyerMarketMaker,
        bool isLast,
        Market market
    )
    : timestampOfReceive(timestampOfReceive)
    , symbol(symbol)
    , price(price)
    , quantity(quantity)
    , isBuyerMarketMaker(isBuyerMarketMaker)
    , isLast(isLast)
    , market(market)
    {}

    TradeEntry() = default;

};
