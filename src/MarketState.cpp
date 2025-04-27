#include <iostream>
#include <optional>

#include "MarketState.h"
#include "enums/TradeEntry.h"
#include "OrderBookMetrics.h"
#include "SingleVariableCounter.h"
#include "MetricMask.h"

MarketState::MarketState()
    : lastTradePtr(nullptr)
    , hasLastTrade(false)
    , orderBook()
{}

void MarketState::update(DecodedEntry* entry) {
    if (auto* differenceDepthEntry = std::get_if<DifferenceDepthEntry>(entry)) {
        orderBook.addOrder(differenceDepthEntry);
        // std::cout << "received order: "<< differenceDepthEntry->Price << std::endl;
    }

    if (auto* tradeEntry = std::get_if<TradeEntry>(entry)) {
        lastTradePtr = tradeEntry;
        hasLastTrade = true;
        // std::cout << "received trade: "<< tradeEntry->Price << std::endl;
    }
}

std::optional<OrderBookMetricsEntry> MarketState::countOrderBookMetrics(MetricMask mask) const {
    if (orderBook.bids.size() < 2 || orderBook.asks.size() < 2 || !hasLastTrade)
        return std::nullopt;

    OrderBookMetricsEntry o{};
    if (mask & BestAsk)
        o.bestAsk = SingleVariableCounter::calculateBestAskPrice(orderBook);
    if (mask & BestBid)
        o.bestBid = SingleVariableCounter::calculateBestBidPrice(orderBook);
    if (mask & MidPrice)
        o.midPrice = SingleVariableCounter::calculateMidPrice(orderBook);
    if (mask & BestVolumeImbalance)
        o.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderBook);
    if (mask & QueueImbalance)
        o.queueImbalance = SingleVariableCounter::calculateQueueImbalance(orderBook);
    if (mask & VolumeImbalance)
        o.volumeImbalance = SingleVariableCounter::calculateVolumeImbalance(orderBook);
    if (mask & Gap)
        o.gap = SingleVariableCounter::calculateGap(orderBook);
    if (mask & IsAggressorAsk)
        o.isAggressorAsk = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);

    return o;
}
