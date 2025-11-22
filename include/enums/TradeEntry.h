#pragma once

#include <cstdint>
#include "symbol.h"
#include "enums/Market.h"

struct TradeEntry {
    uint64_t timestampOfReceive;
    Symbol symbol;
    double price;
    double quantity;
    bool isBuyerMarketMaker;
    bool isLast;
    Market market;

    TradeEntry(
        uint64_t timestampOfReceive,
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
       << ", sym=" << t.symbol
       << ", mkt=" << t.market
       << ", px=" << t.price
       << ", qty=" << t.quantity
       << ", isBuyerMM=" << t.isBuyerMarketMaker
       << ", isLast=" << t.isLast
       << "}";
    return os;
}
