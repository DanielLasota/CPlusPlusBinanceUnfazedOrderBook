// ===== src/MarketState.cpp =====
#include "MarketState.h"
#include "OrderBookMetrics.h"
#include "SingleVariableCounter.h"

#include <algorithm>
#include <type_traits>

// -----------------------------
// 1) Raw pointers version
// -----------------------------
MarketState<false>::MarketState()
  : lastTradePtr(nullptr),
    hasLastTrade(false),
    lastTimestampOfReceive(0)
{}

void MarketState<false>::update(DecodedEntry* entryPtr) {
    // 1) refresh timestamp
    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entryPtr);

    // 2a) depth entry → forward pointer
    if (auto* dde = std::get_if<DifferenceDepthEntry>(entryPtr)) {
        orderBook.addOrder(dde);
    }
    // 2b) trade entry → copy into storage
    if (auto* te = std::get_if<TradeEntry>(entryPtr)) {
        lastTradeStorage = *te;
        lastTradePtr     = &lastTradeStorage;
        hasLastTrade     = true;
    }
}

std::optional<OrderBookMetricsEntry> MarketState<false>::countOrderBookMetrics(MetricMask mask) const {
    if (orderBook.bids.size() < 2 || orderBook.asks.size() < 2 || !hasLastTrade)
        return std::nullopt;

    OrderBookMetricsEntry o{};
    if (mask & TimestampOfReceive)       o.timestampOfReceive  = lastTimestampOfReceive;
    if (mask & BestAsk)                  o.bestAsk             = SingleVariableCounter::calculateBestAskPrice(orderBook);
    if (mask & BestBid)                  o.bestBid             = SingleVariableCounter::calculateBestBidPrice(orderBook);
    if (mask & MidPrice)                 o.midPrice            = SingleVariableCounter::calculateMidPrice(orderBook);
    if (mask & BestVolumeImbalance)      o.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderBook);
    if (mask & QueueImbalance)           o.queueImbalance      = SingleVariableCounter::calculateQueueImbalance(orderBook);
    if (mask & VolumeImbalance)          o.volumeImbalance     = SingleVariableCounter::calculateVolumeImbalance(orderBook);
    if (mask & Gap)                      o.gap                 = SingleVariableCounter::calculateGap(orderBook);
    if (mask & IsAggressorAsk)           o.isAggressorAsk      = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);
    return o;
}

// ------------------------------------------------------------
// 2) Pooled version
// ------------------------------------------------------------
MarketState<true>::MarketState()
  : lastTradePtr(nullptr),
    hasLastTrade(false),
    lastTimestampOfReceive(0)
{}

void MarketState<true>::update(DecodedEntry* entryPtr) {
    // 1) refresh timestamp
    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entryPtr);

    // 2) dispatch by variant type
    std::visit([this](auto const& e){
        using T = std::decay_t<decltype(e)>;
        if constexpr (std::is_same_v<T, DifferenceDepthEntry>) {
            handleDepth(e);
        }
        else if constexpr (std::is_same_v<T, TradeEntry>) {
            lastTradeStorage = e;
            lastTradePtr     = &lastTradeStorage;
            hasLastTrade     = true;
        }
    }, *entryPtr);
}

void MarketState<true>::handleDepth(const DifferenceDepthEntry& dde) {
    // find existing level (price + side)
    auto it = std::find_if(depthPool.begin(), depthPool.end(),
        [&](auto const& stored){
            return stored.Price == dde.Price
                && stored.IsAsk == dde.IsAsk;
        });

    if (dde.Quantity == 0.0) {
        // removal
        if (it != depthPool.end()) {
            // OrderBook::addOrder(z quantity==0) usunie level z OrderBook
            orderBook.addOrder(&*it);
            depthPool.erase(it);
        }
    }
    else {
        if (it != depthPool.end()) {
            // update existing
            *it = dde;
            orderBook.addOrder(&*it);
        } else {
            // insert new
            depthPool.push_back(dde);
            auto& stored = depthPool.back();
            orderBook.addOrder(&stored);
        }
    }
}

std::optional<OrderBookMetricsEntry> MarketState<true>::countOrderBookMetrics(MetricMask mask) const {
    // same as raw pointers version
    if (orderBook.bids.size() < 2 || orderBook.asks.size() < 2 || !hasLastTrade)
        return std::nullopt;

    OrderBookMetricsEntry o{};
    if (mask & TimestampOfReceive)       o.timestampOfReceive  = lastTimestampOfReceive;
    if (mask & BestAsk)                  o.bestAsk             = SingleVariableCounter::calculateBestAskPrice(orderBook);
    if (mask & BestBid)                  o.bestBid             = SingleVariableCounter::calculateBestBidPrice(orderBook);
    if (mask & MidPrice)                 o.midPrice            = SingleVariableCounter::calculateMidPrice(orderBook);
    if (mask & BestVolumeImbalance)      o.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderBook);
    if (mask & QueueImbalance)           o.queueImbalance      = SingleVariableCounter::calculateQueueImbalance(orderBook);
    if (mask & VolumeImbalance)          o.volumeImbalance     = SingleVariableCounter::calculateVolumeImbalance(orderBook);
    if (mask & Gap)                      o.gap                 = SingleVariableCounter::calculateGap(orderBook);
    if (mask & IsAggressorAsk)           o.isAggressorAsk      = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);
    return o;
}
