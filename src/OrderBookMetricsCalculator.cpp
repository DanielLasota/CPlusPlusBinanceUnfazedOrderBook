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
        e.timestampOfReceive = ms.getLastTimestampOfReceive();
    }
    if (mask_ & market) {
        e.market = static_cast<uint8_t>(ms.getMarket());
    }
    if (mask_ & symbol) {
        e.symbol = static_cast<uint8_t>(ms.getSymbol());
    }
    if (mask_ & BestAskPrice) {
        e.bestAskPrice = SingleVariableCounter::calculateBestAskPrice(ms.orderBook);
    }
    if (mask_ & BestBidPrice) {
        e.bestBidPrice = SingleVariableCounter::calculateBestBidPrice(ms.orderBook);
    }
    if (mask_ & MidPrice) {
        e.midPrice = SingleVariableCounter::calculateMidPrice(ms.orderBook);
    }
    if (mask_ & BestVolumeImbalance) {
        e.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(ms.orderBook);
    }
    if (mask_ & BestVolumeRatio) {
        e.bestVolumeRatio = SingleVariableCounter::calculateBestVolumeRatio(ms.orderBook);
    }
    if (mask_ & BestTwoVolumeImbalance){
        e.bestTwoVolumeImbalance = SingleVariableCounter::calculateBestTwoVolumeImbalance(ms.orderBook);
    }
    if (mask_ & BestThreeVolumeImbalance){
        e.bestThreeVolumeImbalance = SingleVariableCounter::calculateBestThreeVolumeImbalance(ms.orderBook);
    }
    if (mask_ & BestFiveVolumeImbalance){
        e.bestFiveVolumeImbalance = SingleVariableCounter::calculateBestFiveVolumeImbalance(ms.orderBook);
    }
    if (mask_ & VolumeImbalance) {
        e.volumeImbalance = SingleVariableCounter::calculateVolumeImbalance(ms.orderBook);
    }
    if (mask_ & QueueImbalance) {
        e.queueImbalance = SingleVariableCounter::calculateQueueImbalance(ms.orderBook);
    }
    if (mask_ & Gap) {
        e.gap = SingleVariableCounter::calculateGap(ms.orderBook);
    }
    if (mask_ & IsAggressorAsk) {
        e.isAggressorAsk = SingleVariableCounter::calculateIsAggressorAsk(&ms.getLastTrade());
    }
    if (mask_ & VwapDeviation) {
        e.vwapDeviation = SingleVariableCounter::calculateVwapDeviation(ms.orderBook);
    }
    if (mask_ & simplifiedSlopeImbalance) {
        e.simplifiedSlopeImbalance = SingleVariableCounter::calculateSimplifiedSlopeImbalance(ms.orderBook);
    }
    if (mask_ & tradeCountImbalance1S) {
        e.tradeCountImbalance1S = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 1);
    }
    if (mask_ & cumulativeDelta10s) {
        e.cumulativeDelta10s = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 10);
    }
    return e;
}
