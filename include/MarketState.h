#pragma once

#include <list>
#include <optional>
#include <variant>
#include <cstdint>

#include "OrderBook.h"
#include "OrderBookMetricsEntry.h"
#include "MetricMask.h"
#include "enums/TradeEntry.h"
#include "enums/DifferenceDepthEntry.h"


using DecodedEntry = std::variant<DifferenceDepthEntry, TradeEntry>;

// forward declaration of template
template <bool UsePool>
class MarketState;

// -----------------------------
// 1) Raw pointers version
// -----------------------------
template <>
class MarketState<false> {
public:
    MarketState();
    void update(DecodedEntry* entryPtr);
    std::optional<OrderBookMetricsEntry> countOrderBookMetrics(MetricMask mask) const;

    OrderBook orderBook;

private:
    TradeEntry   lastTradeStorage;
    TradeEntry*  lastTradePtr;
    bool         hasLastTrade;
    uint64_t     lastTimestampOfReceive;
};

// ------------------------------------------------------------
// 2) Pooled version – list<DifferenceDepthEntry> trzyma obiekty
//    i przy quantity==0 usuwa je natychmiast z pool+OrderBook
// ------------------------------------------------------------
template <>
class MarketState<true> {
public:
    MarketState();
    void update(DecodedEntry* entryPtr);
    std::optional<OrderBookMetricsEntry> countOrderBookMetrics(MetricMask mask) const;

    OrderBook orderBook;

private:
    std::list<DifferenceDepthEntry> depthPool;

    TradeEntry   lastTradeStorage;
    TradeEntry*  lastTradePtr;
    bool         hasLastTrade;
    uint64_t     lastTimestampOfReceive;

    void handleDepth(const DifferenceDepthEntry& dde);
};

// aliases for clarity
using MarketStateRaw    = MarketState<false>;
using MarketStatePooled = MarketState<true>;
