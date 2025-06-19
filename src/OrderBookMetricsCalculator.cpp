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
    if (mask_ & tradeCountImbalance1Seconds) {
        e.tradeCountImbalance1Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 1);
    }
    if (mask_ & tradeCountImbalance3Seconds) {
        e.tradeCountImbalance3Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 3);
    }
    if (mask_ & tradeCountImbalance5Seconds) {
        e.tradeCountImbalance5Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 5);
    }
    if (mask_ & tradeCountImbalance10Seconds) {
        e.tradeCountImbalance10Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 10);
    }
    if (mask_ & tradeCountImbalance15Seconds) {
        e.tradeCountImbalance15Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 15);
    }
    if (mask_ & tradeCountImbalance30Seconds) {
        e.tradeCountImbalance30Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 30);
    }
    if (mask_ & tradeCountImbalance60Seconds) {
        e.tradeCountImbalance60Seconds = SingleVariableCounter::calculateTradeCountImbalance(ms.rollingStatisticsData, 60);
    }


    if (mask_ & cumulativeDelta1Seconds) {
        e.cumulativeDelta1Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 1);
    }
    if (mask_ & cumulativeDelta3Seconds) {
        e.cumulativeDelta3Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 3);
    }
    if (mask_ & cumulativeDelta5Seconds) {
        e.cumulativeDelta5Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 5);
    }
    if (mask_ & cumulativeDelta10Seconds) {
        e.cumulativeDelta10Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 10);
    }
    if (mask_ & cumulativeDelta15Seconds) {
        e.cumulativeDelta15Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 15);
    }
    if (mask_ & cumulativeDelta30Seconds) {
        e.cumulativeDelta30Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 30);
    }
    if (mask_ & cumulativeDelta60Seconds) {
        e.cumulativeDelta60Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 60);
    }

    if (mask_ & priceDifference1Seconds) {
        e.priceDifference1Seconds = SingleVariableCounter::calculatePriceDifference(ms.rollingStatisticsData, 1);
    }
    if (mask_ & priceDifference3Seconds) {
        e.priceDifference3Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 3);
    }
    if (mask_ & priceDifference5Seconds) {
        e.priceDifference5Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 5);
    }
    if (mask_ & priceDifference10Seconds) {
        e.priceDifference10Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 10);
    }
    if (mask_ & priceDifference15Seconds) {
        e.priceDifference15Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 15);
    }
    if (mask_ & priceDifference30Seconds) {
        e.priceDifference30Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 30);
    }
    if (mask_ & priceDifference60Seconds) {
        e.priceDifference60Seconds = SingleVariableCounter::calculateCumulativeDelta(ms.rollingStatisticsData, 60);
    }

    if (mask_ & rateOfReturn1Seconds) {
        e.rateOfReturn1Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 1);
    }
    if (mask_ & rateOfReturn3Seconds) {
        e.rateOfReturn3Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 3);
    }
    if (mask_ & rateOfReturn5Seconds) {
        e.rateOfReturn5Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 5);
    }
    if (mask_ & rateOfReturn10Seconds) {
        e.rateOfReturn10Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 10);
    }
    if (mask_ & rateOfReturn15Seconds) {
        e.rateOfReturn15Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 15);
    }
    if (mask_ & rateOfReturn30Seconds) {
        e.rateOfReturn30Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 30);
    }
    if (mask_ & rateOfReturn60Seconds) {
        e.rateOfReturn60Seconds = SingleVariableCounter::calculateRateOfReturn(ms.rollingStatisticsData, 60);
    }

    if (mask_ & differenceDepthVolatilityImbalance1Seconds) {
        e.differenceDepthVolatilityImbalance1Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 1);
    }
    if (mask_ & differenceDepthVolatilityImbalance3Seconds) {
        e.differenceDepthVolatilityImbalance3Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 3);
    }
    if (mask_ & differenceDepthVolatilityImbalance5Seconds) {
        e.differenceDepthVolatilityImbalance5Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 5);
    }
    if (mask_ & differenceDepthVolatilityImbalance10Seconds) {
        e.differenceDepthVolatilityImbalance10Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 10);
    }
    if (mask_ & differenceDepthVolatilityImbalance15Seconds) {
        e.differenceDepthVolatilityImbalance15Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 15);
    }
    if (mask_ & differenceDepthVolatilityImbalance30Seconds) {
        e.differenceDepthVolatilityImbalance30Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 30);
    }
    if (mask_ & differenceDepthVolatilityImbalance60Seconds) {
        e.differenceDepthVolatilityImbalance60Seconds = SingleVariableCounter::calculateDifferenceDepthVolatilityImbalance(ms.rollingStatisticsData, 60);
    }

    if (mask_ & rsi5Seconds) {
        e.rsi5Seconds = SingleVariableCounter::calculateRSI(ms.rollingStatisticsData, 5);
    }
    if (mask_ & stochRsi5Seconds) {
        e.stochRsi5Seconds = SingleVariableCounter::calculateStochRSI(ms.rollingStatisticsData, 5);
    }
    if (mask_ & macd2Seconds) {
        e.macd2Seconds = SingleVariableCounter::calculateMacd(ms.rollingStatisticsData, 5);
    }
    return e;
}
