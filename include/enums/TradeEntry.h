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
    std::string mUnknownParameter;
    std::string xUnknownParameter;
    bool isLast;
    Market market;

    TradeEntry(
        int64_t timestampOfReceive,
        Symbol symbol,
        double price,
        double quantity,
        bool isBuyerMarketMaker,
        const std::string& mUnknownParameter,
        const std::string& xUnknownParameter,
        bool isLast,
        Market market
    )
    : timestampOfReceive(timestampOfReceive)
    , symbol(symbol)
    , price(price)
    , quantity(quantity)
    , isBuyerMarketMaker(isBuyerMarketMaker)
    , mUnknownParameter(mUnknownParameter)
    , xUnknownParameter(xUnknownParameter)
    , isLast(isLast)
    , market(market)
    {}

    TradeEntry() = default;

};
