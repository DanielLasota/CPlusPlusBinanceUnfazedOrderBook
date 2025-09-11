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

inline std::ostream& operator<<(std::ostream& os, TradeEntry const& t) {
    os << "Trade{"
       << "ts=" << t.timestampOfReceive
       << ", sym=" << int(t.symbol)
       << ", mkt=" << int(t.market)
       << ", px=" << t.price
       << ", qty=" << t.quantity
       << ", isBuyerMM=" << (t.isBuyerMarketMaker?1:0)
       << ", isLast=" << (t.isLast?1:0)
       << "}";
    return os;
}
