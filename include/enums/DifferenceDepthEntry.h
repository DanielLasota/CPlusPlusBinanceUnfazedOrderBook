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

inline std::ostream& operator<<(std::ostream& os, DifferenceDepthEntry const& e) {
    os << "DiffDepth{"
       << "ts=" << e.timestampOfReceive
       << ", sym=" << int(e.symbol)
       << ", mkt=" << int(e.market)
       << ", isAsk=" << (e.isAsk?1:0)
       << ", px=" << e.price
       << ", qty=" << e.quantity
       << ", isLast=" << (e.isLast?1:0)
       << "}";
    return os;
}
