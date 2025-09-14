#pragma once

#include <cstdint>
// #include <iomanip>
// #include <chrono>

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
    // using namespace std::chrono;
    // const auto tp = system_clock::time_point{microseconds{t.timestampOfReceive}};
    // const std::time_t tt = system_clock::to_time_t(tp);
    // const std::tm tm_utc = *std::gmtime(&tt);
    //
    // const auto dur = tp.time_since_epoch();
    // const auto secs = duration_cast<seconds>(dur);
    // const auto uSecs = duration_cast<microseconds>(dur - secs).count();

    os << "Trade{"
       << "ts=" << t.timestampOfReceive
       // << ", dt=" << std::put_time(&tm_utc, "%Y-%m-%d %H:%M:%S")
       // << "." << std::setw(6) << std::setfill('0') << uSecs
       << ", sym=" << int(t.symbol)
       << ", mkt=" << int(t.market)
       << ", px=" << t.price
       << ", qty=" << t.quantity
       << ", isBuyerMM=" << (t.isBuyerMarketMaker?1:0)
       << ", isLast=" << (t.isLast?1:0)
       << "}";
    return os;
}
