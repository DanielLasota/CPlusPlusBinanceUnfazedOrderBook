#include "OrderBookMetricsCalculator.h"
#include "SingleVariableCounter.h"

std::optional<OrderBookMetricsEntry> OrderBookMetricsCalculator::countMarketStateMetrics(const MarketState& ms) const {
    if (!ms.getHasLastTrade() ||
        ms.orderBook.askCount() < 2 ||
        ms.orderBook.bidCount() < 2)
    {
        return std::nullopt;
    }

    OrderBookMetricsEntry e{};
    if (mask_ & TimestampOfReceive) {
        e.timestampOfReceive    = ms.getLastTimestampOfReceive();
    }
    if (mask_ & market) {
        e.market                = static_cast<uint8_t>(ms.getMarket());
    }
    if (mask_ & symbol) {
        e.symbol                = static_cast<uint8_t>(ms.getSymbol());
    }
    if (mask_ & BestAskPrice) {
        e.bestAskPrice          = SingleVariableCounter::calculateBestAskPrice(ms.orderBook);
    }
    if (mask_ & BestBidPrice) {
        e.bestBidPrice          = SingleVariableCounter::calculateBestBidPrice(ms.orderBook);
    }
    if (mask_ & MidPrice) {
        e.midPrice              = SingleVariableCounter::calculateMidPrice(ms.orderBook);
    }
    if (mask_ & BestVolumeImbalance) {
        e.bestVolumeImbalance   = SingleVariableCounter::calculateBestVolumeImbalance(ms.orderBook);
    }
    if (mask_ & QueueImbalance) {
        e.queueImbalance        = SingleVariableCounter::calculateQueueImbalance(ms.orderBook);
    }
    if (mask_ & VolumeImbalance) {
        e.volumeImbalance       = SingleVariableCounter::calculateVolumeImbalance(ms.orderBook);
    }
    if (mask_ & Gap) {
        e.gap                   = SingleVariableCounter::calculateGap(ms.orderBook);
    }
    if (mask_ & IsAggressorAsk) {
        e.isAggressorAsk        = SingleVariableCounter::calculateIsAggressorAsk(&ms.getLastTrade());
    }
    return e;
}
