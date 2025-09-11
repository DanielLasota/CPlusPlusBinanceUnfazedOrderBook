#include "OrderBookMetricsCalculator.h"
#include "SingleVariableCounter.h"

std::optional<OrderBookMetricsEntry> OrderBookMetricsCalculator::countMarketStateMetrics(const MarketState& marketState) const {
    if (!marketState.getHasLastTrade() || marketState.orderBook.askCount() < 2 || marketState.orderBook.bidCount() < 2){
        return std::nullopt;
    }

    OrderBookMetricsEntry e{};
    if (mask_ & timestampOfReceive) {
        e.timestampOfReceive = marketState.getLastTimestampOfReceive();
    }
    if (mask_ & market) {
        e.market = static_cast<uint8_t>(marketState.getMarket());
    }
    if (mask_ & symbol) {
        e.symbol = static_cast<uint8_t>(marketState.getSymbol());
    }
    if (mask_ & bestAskPrice) {
        e.bestAskPrice = SingleVariableCounter::calculateBestAskPrice(marketState.orderBook);
    }
    if (mask_ & bestBidPrice) {
        e.bestBidPrice = SingleVariableCounter::calculateBestBidPrice(marketState.orderBook);
    }
    if (mask_ & midPrice) {
        e.midPrice = SingleVariableCounter::calculateMidPrice(marketState.orderBook);
    }

    if (mask_ & microPriceDiff){
        e.microPriceDiff = SingleVariableCounter::calculateMicroPriceDiff(marketState.orderBook);
    }
    if (mask_ & microPriceImbalance){
        e.microPriceImbalance = SingleVariableCounter::calculateMicroPriceImbalance(marketState.orderBook);
    }
    if (mask_ & microPriceFisherImbalance){
        e.microPriceFisherImbalance = SingleVariableCounter::calculateMicroPriceFisherImbalance(marketState.orderBook);
    }
    if (mask_ & microPriceDeviation){
        e.microPriceDeviation = SingleVariableCounter::calculateMicroPriceDeviation(marketState.orderBook);
    }
    if (mask_ & microPriceLogRatio){
        e.microPriceLogRatio = SingleVariableCounter::calculateMicroPriceLogRatio(marketState.orderBook);
    }

    if (mask_ & bestBidQuantity){
        e.bestBidQuantity = SingleVariableCounter::calculateBestBidQuantity(marketState.orderBook);
    }
    if (mask_ & bestAskQuantity){
        e.bestAskQuantity = SingleVariableCounter::calculateBestAskQuantity(marketState.orderBook);
    }

    if (mask_ & bestOrderFlowDiff){
        e.bestOrderFlowDiff = SingleVariableCounter::calculateBestOrderFlowDiff(marketState.orderBook);
    }
    if (mask_ & bestOrderFlowImbalance){
        e.bestOrderFlowImbalance = SingleVariableCounter::calculateBestOrderFlowImbalance(marketState.orderBook);
    }
    if (mask_ & bestOrderFlowFisherImbalance){
        e.bestOrderFlowFisherImbalance = SingleVariableCounter::calculateBestOrderFlowFisherImbalance(marketState.orderBook);
    }

    if (mask_ & bestOrderFlowCKSDiff){
        e.bestOrderFlowCKSDiff = SingleVariableCounter::calculateBestOrderFlowCKSDiff(marketState.orderBook);
    }
    if (mask_ & bestOrderFlowCKSImbalance){
        e.bestOrderFlowCKSImbalance = SingleVariableCounter::calculateBestOrderFlowCKSImbalance(marketState.orderBook);
    }
    if (mask_ & bestOrderFlowCKSFisherImbalance){
        e.bestOrderFlowCKSFisherImbalance = SingleVariableCounter::calculateBestOrderFlowCKSFisherImbalance(marketState.orderBook);
    }

    if (mask_ & orderFlowDiff){
        e.orderFlowDiff = SingleVariableCounter::calculateOrderFlowDiff(marketState.orderBook);
    }
    if (mask_ & orderFlowImbalance){
        e.orderFlowImbalance = SingleVariableCounter::calculateOrderFlowImbalance(marketState.orderBook);
    }
    if (mask_ & orderFlowFisherImbalance){
        e.orderFlowFisherImbalance = SingleVariableCounter::calculateOrderFlowFisherImbalance(marketState.orderBook);
    }

    if (mask_ & queueCountFlowDiff){
        e.queueCountFlowDiff = SingleVariableCounter::calculateQueueCountFlowDelta(marketState.orderBook);
    }
    if (mask_ & queueCountFlowImbalance){
        e.queueCountFlowImbalance = SingleVariableCounter::calculateQueueCountFlowImbalance(marketState.orderBook);
    }
    if (mask_ & queueCountFlowFisherImbalance){
        e.queueCountFlowFisherImbalance = SingleVariableCounter::calculateQueueCountFlowFisherImbalance(marketState.orderBook);
    }

    if (mask_ & bestVolumeDiff) {
        e.bestVolumeDiff = SingleVariableCounter::calculateBestVolumeDiff(marketState.orderBook);
    }
    if (mask_ & bestVolumeImbalance) {
        e.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(marketState.orderBook);
    }
    if (mask_ & bestVolumeFisherImbalance) {
        e.bestVolumeFisherImbalance = SingleVariableCounter::calculateBestVolumeFisherImbalance(marketState.orderBook);
    }
    if (mask_ & bestVolumeLogRatio) {
        e.bestVolumeLogRatio = SingleVariableCounter::calculateBestVolumeLogRatio(marketState.orderBook);
    }
    if (mask_ & bestVolumeSignedLogRatioXVolume) {
        e.bestVolumeSignedLogRatioXVolume = SingleVariableCounter::calculateBestVolumeSignedLogRatioXVolume(marketState.orderBook);
    }

    if (mask_ & bestTwoVolumeDiff){
        e.bestTwoVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 2);
    }
    if (mask_ & bestThreeVolumeDiff){
        e.bestThreeVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 3);
    }
    if (mask_ & bestFiveVolumeDiff){
        e.bestFiveVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 5);
    }
    if (mask_ & bestTenVolumeDiff){
        e.bestTenVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 10);
    }
    if (mask_ & bestFifteenVolumeDiff){
        e.bestFifteenVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 15);
    }
    if (mask_ & bestTwentyVolumeDiff){
        e.bestTwentyVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 20);
    }
    if (mask_ & bestThirtyVolumeDiff){
        e.bestThirtyVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 30);
    }
    if (mask_ & bestFiftyVolumeDiff){
        e.bestFiftyVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 50);
    }

    if (mask_ & bestTwoVolumeImbalance){
        e.bestTwoVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 2);
    }
    if (mask_ & bestThreeVolumeImbalance){
        e.bestThreeVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 3);
    }
    if (mask_ & bestFiveVolumeImbalance){
        e.bestFiveVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 5);
    }
    if (mask_ & bestTenVolumeImbalance){
        e.bestTenVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 10);
    }
    if (mask_ & bestFifteenVolumeImbalance){
        e.bestFifteenVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 15);
    }
    if (mask_ & bestTwentyVolumeImbalance){
        e.bestTwentyVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 20);
    }
    if (mask_ & bestThirtyVolumeImbalance){
        e.bestThirtyVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 30);
    }
    if (mask_ & bestFiftyVolumeImbalance){
        e.bestFiftyVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 50);
    }

    if (mask_ & bestTwoVolumeLogRatio){
        e.bestTwoVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 2);
    }
    if (mask_ & bestThreeVolumeLogRatio){
        e.bestThreeVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 3);
    }
    if (mask_ & bestFiveVolumeLogRatio){
        e.bestFiveVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 5);
    }
    if (mask_ & bestTenVolumeLogRatio){
        e.bestTenVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 10);
    }
    if (mask_ & bestFifteenVolumeLogRatio){
        e.bestFifteenVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 15);
    }
    if (mask_ & bestTwentyVolumeLogRatio){
        e.bestTwentyVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 20);
    }
    if (mask_ & bestThirtyVolumeLogRatio){
        e.bestThirtyVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 30);
    }
    if (mask_ & bestFiftyVolumeLogRatio){
        e.bestFiftyVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 50);
    }

    if (mask_ & bestFiveVolumeLogRatioXVolume){
        e.bestFiveVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 5);
    }
    if (mask_ & bestFiftyVolumeLogRatioXVolume){
        e.bestFiftyVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 50);
    }

    if (mask_ & volumeDiff) {
        e.volumeDiff = SingleVariableCounter::calculateVolumeDiff(marketState.orderBook);
    }
    if (mask_ & volumeImbalance) {
        e.volumeImbalance = SingleVariableCounter::calculateVolumeImbalance(marketState.orderBook);
    }
    if (mask_ & volumeLogRatio) {
        e.volumeLogRatio = SingleVariableCounter::calculateVolumeLogRatio(marketState.orderBook);
    }
    if (mask_ & volumeLogRatioXVolume) {
        e.volumeLogRatioXVolume = SingleVariableCounter::calculateVolumeLogRatioXVolume(marketState.orderBook);
    }

    if (mask_ & queueDiff) {
        e.queueDiff = SingleVariableCounter::calculateQueueDiff(marketState.orderBook);
    }
    if (mask_ & queueImbalance) {
        e.queueImbalance = SingleVariableCounter::calculateQueueImbalance(marketState.orderBook);
    }
    if (mask_ & queueLogRatio) {
        e.queueLogRatio = SingleVariableCounter::calculateQueueLogRatio(marketState.orderBook);
    }
    if (mask_ & queueLogRatioXVolume) {
        e.queueLogRatioXVolume = SingleVariableCounter::calculateQueueLogRatioXVolume(marketState.orderBook);
    }

    if (mask_ & gap) {
        e.gap = SingleVariableCounter::calculateGap(marketState.orderBook);
    }
    if (mask_ & isAggressorAsk) {
        e.isAggressorAsk = SingleVariableCounter::calculateIsAggressorAsk(&marketState.getLastTrade());
    }

    if (mask_ & vwapDeviation) {
        e.vwapDeviation = SingleVariableCounter::calculateVwapDeviation(marketState.orderBook);
    }
    if (mask_ & vwapLogRatio) {
        e.vwapLogRatio = SingleVariableCounter::calculateVwapLogRatio(marketState.orderBook);
    }

    if (mask_ & simplifiedSlopeDiff) {
        e.simplifiedSlopeDiff = SingleVariableCounter::calculateSimplifiedSlopeDiff(marketState.orderBook);
    }
    if (mask_ & simplifiedSlopeImbalance) {
        e.simplifiedSlopeImbalance = SingleVariableCounter::calculateSimplifiedSlopeImbalance(marketState.orderBook);
    }
    if (mask_ & simplifiedSlopeLogRatio) {
        e.simplifiedSlopeLogRatio = SingleVariableCounter::calculateSimplifiedSlopeLogRatio(marketState.orderBook);
    }
    if (mask_ & bgcSlopeDiff) {
        e.bgcSlopeDiff = SingleVariableCounter::calculateBgcSlopeDiff(marketState.orderBook);
    }
    if (mask_ & bgcSlopeImbalance) {
        e.bgcSlopeImbalance = SingleVariableCounter::calculateBgcSlopeImbalance(marketState.orderBook);
    }
    if (mask_ & bgcSlopeLogRatio) {
        e.bgcSlopeLogRatio = SingleVariableCounter::calculateBgcSlopeLogRatio(marketState.orderBook);
    }

    if (mask_ & differenceDepthCount1Seconds) {
        e.differenceDepthCount1Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 1);
    }
    if (mask_ & differenceDepthCount3Seconds) {
        e.differenceDepthCount3Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 3);
    }
    if (mask_ & differenceDepthCount5Seconds) {
        e.differenceDepthCount5Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 5);
    }
    if (mask_ & differenceDepthCount10Seconds) {
        e.differenceDepthCount10Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 10);
    }
    if (mask_ & differenceDepthCount15Seconds) {
        e.differenceDepthCount15Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 15);
    }
    if (mask_ & differenceDepthCount30Seconds) {
        e.differenceDepthCount30Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 30);
    }
    if (mask_ & differenceDepthCount60Seconds) {
        e.differenceDepthCount60Seconds = SingleVariableCounter::calculateDifferenceDepthCount(marketState.rollingDifferenceDepthStatistics, 60);
    }

    if (mask_ & tradeCount1Seconds) {
        e.tradeCount1Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeCount3Seconds) {
        e.tradeCount3Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeCount5Seconds) {
        e.tradeCount5Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeCount10Seconds) {
        e.tradeCount10Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeCount15Seconds) {
        e.tradeCount15Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeCount30Seconds) {
        e.tradeCount30Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeCount60Seconds) {
        e.tradeCount60Seconds = SingleVariableCounter::calculateTradeCount(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & differenceDepthCountDiff1Seconds) {
        e.differenceDepthCountDiff1Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 1);
    }
    if (mask_ & differenceDepthCountDiff3Seconds) {
        e.differenceDepthCountDiff3Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 3);
    }
    if (mask_ & differenceDepthCountDiff5Seconds) {
        e.differenceDepthCountDiff5Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 5);
    }
    if (mask_ & differenceDepthCountDiff10Seconds) {
        e.differenceDepthCountDiff10Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 10);
    }
    if (mask_ & differenceDepthCountDiff15Seconds) {
        e.differenceDepthCountDiff15Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 15);
    }
    if (mask_ & differenceDepthCountDiff30Seconds) {
        e.differenceDepthCountDiff30Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 30);
    }
    if (mask_ & differenceDepthCountDiff60Seconds) {
        e.differenceDepthCountDiff60Seconds = SingleVariableCounter::calculateDifferenceDepthCountDiff(marketState.rollingDifferenceDepthStatistics, 60);
    }

    if (mask_ & differenceDepthCountImbalance1Seconds) {
        e.differenceDepthCountImbalance1Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 1);
    }
    if (mask_ & differenceDepthCountImbalance3Seconds) {
        e.differenceDepthCountImbalance3Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 3);
    }
    if (mask_ & differenceDepthCountImbalance5Seconds) {
        e.differenceDepthCountImbalance5Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 5);
    }
    if (mask_ & differenceDepthCountImbalance10Seconds) {
        e.differenceDepthCountImbalance10Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 10);
    }
    if (mask_ & differenceDepthCountImbalance15Seconds) {
        e.differenceDepthCountImbalance15Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 15);
    }
    if (mask_ & differenceDepthCountImbalance30Seconds) {
        e.differenceDepthCountImbalance30Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 30);
    }
    if (mask_ & differenceDepthCountImbalance60Seconds) {
        e.differenceDepthCountImbalance60Seconds = SingleVariableCounter::calculateDifferenceDepthCountImbalance(marketState.rollingDifferenceDepthStatistics, 60);
    }

    if (mask_ & differenceDepthCountFisherImbalance1Seconds) {
        e.differenceDepthCountFisherImbalance1Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 1);
    }
    if (mask_ & differenceDepthCountFisherImbalance3Seconds) {
        e.differenceDepthCountFisherImbalance3Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 3);
    }
    if (mask_ & differenceDepthCountFisherImbalance5Seconds) {
        e.differenceDepthCountFisherImbalance5Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 5);
    }
    if (mask_ & differenceDepthCountFisherImbalance10Seconds) {
        e.differenceDepthCountFisherImbalance10Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 10);
    }
    if (mask_ & differenceDepthCountFisherImbalance15Seconds) {
        e.differenceDepthCountFisherImbalance15Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 15);
    }
    if (mask_ & differenceDepthCountFisherImbalance30Seconds) {
        e.differenceDepthCountFisherImbalance30Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 30);
    }
    if (mask_ & differenceDepthCountFisherImbalance60Seconds) {
        e.differenceDepthCountFisherImbalance60Seconds = SingleVariableCounter::calculateDifferenceDepthCountFisherImbalance(marketState.rollingDifferenceDepthStatistics, 60);
    }

    if (mask_ & differenceDepthCountLogRatio1Seconds) {
        e.differenceDepthCountLogRatio1Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 1);
    }
    if (mask_ & differenceDepthCountLogRatio3Seconds) {
        e.differenceDepthCountLogRatio3Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 3);
    }
    if (mask_ & differenceDepthCountLogRatio5Seconds) {
        e.differenceDepthCountLogRatio5Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 5);
    }
    if (mask_ & differenceDepthCountLogRatio10Seconds) {
        e.differenceDepthCountLogRatio10Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 10);
    }
    if (mask_ & differenceDepthCountLogRatio15Seconds) {
        e.differenceDepthCountLogRatio15Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 15);
    }
    if (mask_ & differenceDepthCountLogRatio30Seconds) {
        e.differenceDepthCountLogRatio30Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 30);
    }
    if (mask_ & differenceDepthCountLogRatio60Seconds) {
        e.differenceDepthCountLogRatio60Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatio(marketState.rollingDifferenceDepthStatistics, 60);
    }

    if (mask_ & differenceDepthCountLogRatioXEventCount1Seconds) {
        e.differenceDepthCountLogRatioXEventCount1Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 1);
    }
    if (mask_ & differenceDepthCountLogRatioXEventCount3Seconds) {
        e.differenceDepthCountLogRatioXEventCount3Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 3);
    }
    if (mask_ & differenceDepthCountLogRatioXEventCount5Seconds) {
        e.differenceDepthCountLogRatioXEventCount5Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 5);
    }
    if (mask_ & differenceDepthCountLogRatioXEventCount10Seconds) {
        e.differenceDepthCountLogRatioXEventCount10Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 10);
    }
    if (mask_ & differenceDepthCountLogRatioXEventCount15Seconds) {
        e.differenceDepthCountLogRatioXEventCount15Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 15);
    }
    if (mask_ & differenceDepthCountLogRatioXEventCount30Seconds) {
        e.differenceDepthCountLogRatioXEventCount30Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 30);
    }
    if (mask_ & differenceDepthCountLogRatioXEventCount60Seconds) {
        e.differenceDepthCountLogRatioXEventCount60Seconds = SingleVariableCounter::calculateDifferenceDepthCountLogRatioXEventCount(marketState.rollingDifferenceDepthStatistics, 60);
    }

    if (mask_ & tradeCountDiff1Seconds) {
        e.tradeCountDiff1Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeCountDiff3Seconds) {
        e.tradeCountDiff3Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeCountDiff5Seconds) {
        e.tradeCountDiff5Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeCountDiff10Seconds) {
        e.tradeCountDiff10Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeCountDiff15Seconds) {
        e.tradeCountDiff15Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeCountDiff30Seconds) {
        e.tradeCountDiff30Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeCountDiff60Seconds) {
        e.tradeCountDiff60Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & tradeCountImbalance1Seconds) {
        e.tradeCountImbalance1Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeCountImbalance3Seconds) {
        e.tradeCountImbalance3Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeCountImbalance5Seconds) {
        e.tradeCountImbalance5Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeCountImbalance10Seconds) {
        e.tradeCountImbalance10Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeCountImbalance15Seconds) {
        e.tradeCountImbalance15Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeCountImbalance30Seconds) {
        e.tradeCountImbalance30Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeCountImbalance60Seconds) {
        e.tradeCountImbalance60Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & tradeCountFisherImbalance1Seconds) {
        e.tradeCountFisherImbalance1Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeCountFisherImbalance3Seconds) {
        e.tradeCountFisherImbalance3Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeCountFisherImbalance5Seconds) {
        e.tradeCountFisherImbalance5Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeCountFisherImbalance10Seconds) {
        e.tradeCountFisherImbalance10Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeCountFisherImbalance15Seconds) {
        e.tradeCountFisherImbalance15Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeCountFisherImbalance30Seconds) {
        e.tradeCountFisherImbalance30Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeCountFisherImbalance60Seconds) {
        e.tradeCountFisherImbalance60Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & tradeCountLogRatio1Seconds) {
        e.tradeCountLogRatio1Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeCountLogRatio3Seconds) {
        e.tradeCountLogRatio3Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeCountLogRatio5Seconds) {
        e.tradeCountLogRatio5Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeCountLogRatio10Seconds) {
        e.tradeCountLogRatio10Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeCountLogRatio15Seconds) {
        e.tradeCountLogRatio15Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeCountLogRatio30Seconds) {
        e.tradeCountLogRatio30Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeCountLogRatio60Seconds) {
        e.tradeCountLogRatio60Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & tradeVolumeDiff1Seconds) {
        e.tradeVolumeDiff1Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeVolumeDiff3Seconds) {
        e.tradeVolumeDiff3Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeVolumeDiff5Seconds) {
        e.tradeVolumeDiff5Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeVolumeDiff10Seconds) {
        e.tradeVolumeDiff10Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeVolumeDiff15Seconds) {
        e.tradeVolumeDiff15Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeVolumeDiff30Seconds) {
        e.tradeVolumeDiff30Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeVolumeDiff60Seconds) {
        e.tradeVolumeDiff60Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & tradeVolumeImbalance1Seconds) {
        e.tradeVolumeImbalance1Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeVolumeImbalance3Seconds) {
        e.tradeVolumeImbalance3Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeVolumeImbalance5Seconds) {
        e.tradeVolumeImbalance5Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeVolumeImbalance10Seconds) {
        e.tradeVolumeImbalance10Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeVolumeImbalance15Seconds) {
        e.tradeVolumeImbalance15Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeVolumeImbalance30Seconds) {
        e.tradeVolumeImbalance30Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeVolumeImbalance60Seconds) {
        e.tradeVolumeImbalance60Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & tradeVolumeLogRatio1Seconds) {
        e.tradeVolumeLogRatio1Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & tradeVolumeLogRatio3Seconds) {
        e.tradeVolumeLogRatio3Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & tradeVolumeLogRatio5Seconds) {
        e.tradeVolumeLogRatio5Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & tradeVolumeLogRatio10Seconds) {
        e.tradeVolumeLogRatio10Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & tradeVolumeLogRatio15Seconds) {
        e.tradeVolumeLogRatio15Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & tradeVolumeLogRatio30Seconds) {
        e.tradeVolumeLogRatio30Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & tradeVolumeLogRatio60Seconds) {
        e.tradeVolumeLogRatio60Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & avgTradeSizeDiff1Seconds) {
        e.avgTradeSizeDiff1Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & avgTradeSizeDiff3Seconds) {
        e.avgTradeSizeDiff3Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & avgTradeSizeDiff5Seconds) {
        e.avgTradeSizeDiff5Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & avgTradeSizeDiff10Seconds) {
        e.avgTradeSizeDiff10Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & avgTradeSizeDiff15Seconds) {
        e.avgTradeSizeDiff15Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & avgTradeSizeDiff30Seconds) {
        e.avgTradeSizeDiff30Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & avgTradeSizeDiff60Seconds) {
        e.avgTradeSizeDiff60Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & avgTradeSizeImbalance1Seconds) {
        e.avgTradeSizeImbalance1Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & avgTradeSizeImbalance3Seconds) {
        e.avgTradeSizeImbalance3Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & avgTradeSizeImbalance5Seconds) {
        e.avgTradeSizeImbalance5Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & avgTradeSizeImbalance10Seconds) {
        e.avgTradeSizeImbalance10Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & avgTradeSizeImbalance15Seconds) {
        e.avgTradeSizeImbalance15Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & avgTradeSizeImbalance30Seconds) {
        e.avgTradeSizeImbalance30Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & avgTradeSizeImbalance60Seconds) {
        e.avgTradeSizeImbalance60Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & avgTradeSizeLogRatio1Seconds) {
        e.avgTradeSizeLogRatio1Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & avgTradeSizeLogRatio3Seconds) {
        e.avgTradeSizeLogRatio3Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & avgTradeSizeLogRatio5Seconds) {
        e.avgTradeSizeLogRatio5Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & avgTradeSizeLogRatio10Seconds) {
        e.avgTradeSizeLogRatio10Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & avgTradeSizeLogRatio15Seconds) {
        e.avgTradeSizeLogRatio15Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & avgTradeSizeLogRatio30Seconds) {
        e.avgTradeSizeLogRatio30Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & avgTradeSizeLogRatio60Seconds) {
        e.avgTradeSizeLogRatio60Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & biggestSingleBuyTradeVolume1Seconds) {
        e.biggestSingleBuyTradeVolume1Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & biggestSingleBuyTradeVolume3Seconds) {
        e.biggestSingleBuyTradeVolume3Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & biggestSingleBuyTradeVolume5Seconds) {
        e.biggestSingleBuyTradeVolume5Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & biggestSingleBuyTradeVolume10Seconds) {
        e.biggestSingleBuyTradeVolume10Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & biggestSingleBuyTradeVolume15Seconds) {
        e.biggestSingleBuyTradeVolume15Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & biggestSingleBuyTradeVolume30Seconds) {
        e.biggestSingleBuyTradeVolume30Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & biggestSingleBuyTradeVolume60Seconds) {
        e.biggestSingleBuyTradeVolume60Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & biggestSingleSellTradeVolume1Seconds) {
        e.biggestSingleSellTradeVolume1Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & biggestSingleSellTradeVolume3Seconds) {
        e.biggestSingleSellTradeVolume3Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & biggestSingleSellTradeVolume5Seconds) {
        e.biggestSingleSellTradeVolume5Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & biggestSingleSellTradeVolume10Seconds) {
        e.biggestSingleSellTradeVolume10Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & biggestSingleSellTradeVolume15Seconds) {
        e.biggestSingleSellTradeVolume15Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & biggestSingleSellTradeVolume30Seconds) {
        e.biggestSingleSellTradeVolume30Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & biggestSingleSellTradeVolume60Seconds) {
        e.biggestSingleSellTradeVolume60Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.rollingTradeStatistics, 60);
    }


    if (mask_ & priceDifference1Seconds) {
        e.priceDifference1Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & priceDifference3Seconds) {
        e.priceDifference3Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & priceDifference5Seconds) {
        e.priceDifference5Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & priceDifference10Seconds) {
        e.priceDifference10Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & priceDifference15Seconds) {
        e.priceDifference15Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & priceDifference30Seconds) {
        e.priceDifference30Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & priceDifference60Seconds) {
        e.priceDifference60Seconds = SingleVariableCounter::calculatePriceDifference(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & rateOfReturn1Seconds) {
        e.rateOfReturn1Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & rateOfReturn3Seconds) {
        e.rateOfReturn3Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & rateOfReturn5Seconds) {
        e.rateOfReturn5Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & rateOfReturn10Seconds) {
        e.rateOfReturn10Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & rateOfReturn15Seconds) {
        e.rateOfReturn15Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & rateOfReturn30Seconds) {
        e.rateOfReturn30Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & rateOfReturn60Seconds) {
        e.rateOfReturn60Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & logReturnRatio1Seconds) {
        e.logReturnRatio1Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & logReturnRatio3Seconds) {
        e.logReturnRatio3Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & logReturnRatio5Seconds) {
        e.logReturnRatio5Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & logReturnRatio10Seconds) {
        e.logReturnRatio10Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & logReturnRatio15Seconds) {
        e.logReturnRatio15Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & logReturnRatio30Seconds) {
        e.logReturnRatio30Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & logReturnRatio60Seconds) {
        e.logReturnRatio60Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & logKylesLambda1Seconds) {
        e.logKylesLambda1Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 1);
    }
    if (mask_ & logKylesLambda3Seconds) {
        e.logKylesLambda3Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 3);
    }
    if (mask_ & logKylesLambda5Seconds) {
        e.logKylesLambda5Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & logKylesLambda10Seconds) {
        e.logKylesLambda10Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 10);
    }
    if (mask_ & logKylesLambda15Seconds) {
        e.logKylesLambda15Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 15);
    }
    if (mask_ & logKylesLambda30Seconds) {
        e.logKylesLambda30Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 30);
    }
    if (mask_ & logKylesLambda60Seconds) {
        e.logKylesLambda60Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.rollingTradeStatistics, 60);
    }

    if (mask_ & rsi5Seconds) {
        e.rsi5Seconds = SingleVariableCounter::calculateRSI(marketState.rollingTradeStatistics, 0, 5);
    }
    if (mask_ & stochRsi5Seconds) {
        e.stochRsi5Seconds = SingleVariableCounter::calculateStochRSI(marketState.rollingTradeStatistics, 5);
    }
    if (mask_ & macd2Seconds) {
        e.macd2Seconds = SingleVariableCounter::calculateMacd(marketState.rollingTradeStatistics, 2);
    }

    return e;
}
