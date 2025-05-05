#pragma once

#include <variant>
#include <optional>
#include <cstdint>
#include <OrderBook.h>
#include <vector>
#include <unordered_map>

#include "OrderBookMetricsEntry.h"
#include "MetricMask.h"
#include "enums/TradeEntry.h"
#include "enums/DifferenceDepthEntry.h"

// alias dla variantu
using DecodedEntry = std::variant<DifferenceDepthEntry, TradeEntry>;

// forward
template <bool UsePool>
class MarketState;

// ----------------------------------------------------------------
// 1) Raw‐pointers version (bez puli) – nie zmieniamy
// ----------------------------------------------------------------
template <>
class MarketState<false> {
public:
    MarketState();
    void update(DecodedEntry* entryPtr);
    std::optional<OrderBookMetricsEntry> countOrderBookMetrics(MetricMask mask) const;
    OrderBook orderBook;
private:
    TradeEntry   lastTradeStorage;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};
    uint64_t     lastTimestampOfReceive{0};
};

// ----------------------------------------------------------------
// 2) Pooled & optimized version
// ----------------------------------------------------------------
struct PriceSide {
    double price;
    bool   isAsk;
    bool operator==(PriceSide const& o) const noexcept {
        return price == o.price && isAsk == o.isAsk;
    }
};
struct PriceSideHash {
    size_t operator()(PriceSide const& ps) const noexcept {
        // bitowa reprezentacja double + bool
        auto h1 = std::hash<uint64_t>{}(
            *reinterpret_cast<uint64_t const*>(&ps.price)
        );
        auto h2 = std::hash<bool>{}(ps.isAsk);
        return h1 ^ (h2 << 1);
    }
};

template <>
class MarketState<true> {
public:
    // maxLevels – maksymalna liczba poziomów w book’u (pre-alloc)
    explicit MarketState(size_t maxLevels = 100000);

    // ultraszybkie update – O(1)
    void update(DecodedEntry* entryPtr);

    // O(1) count metryk (bo trzymamy sumy/liczniki)
    std::optional<OrderBookMetricsEntry> countOrderBookMetrics(MetricMask mask) const;

    void printOrderBook() const;

private:
    // intruzynowa struktura dla pool’a
    struct PriceLevel {
        double     price;
        bool       isAsk;
        double     quantity;
        PriceLevel* prev;
        PriceLevel* next;
    };

    // pool + free‐list
    std::vector<PriceLevel> arena;
    PriceLevel*             freeListHead_{nullptr};

    // heads dwu‐list dla ask i bid
    PriceLevel*             askHead_{nullptr};
    PriceLevel*             bidHead_{nullptr};

    // mapowanie (price,side) → node
    std::unordered_map<PriceSide, PriceLevel*, PriceSideHash> index_;

    // stan ostatniego trade’u
    TradeEntry   lastTradeStorage;
    TradeEntry*  lastTradePtr{nullptr};
    bool         hasLastTrade{false};

    // timestamp ostatniego update
    uint64_t     lastTimestampOfReceive{0};

    // dla metryk: liczniki i sumy
    size_t       askCount_{0}, bidCount_{0};
    double       sumAskQty_{0.0}, sumBidQty_{0.0};

    // alokacja i dealokacja node’a z pool’a
    PriceLevel* allocateNode(double price, bool isAsk, double quantity);
    void        deallocateNode(PriceLevel* node);

    // wstawianie/removal w head‐list
    static void insertHead(PriceLevel*& head, PriceLevel* node);
    static void removeNode(PriceLevel*& head, PriceLevel* node);
};
