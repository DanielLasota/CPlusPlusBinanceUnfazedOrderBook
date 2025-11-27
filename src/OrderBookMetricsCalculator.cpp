#include "OrderBookMetricsCalculator.h"
#include "SingleVariableCounter.h"

std::optional<OrderBookMetricsEntry> OrderBookMetricsCalculator::countMarketStateMetrics(const MarketState& marketState) const {
    if (!marketState.getHasLastTrade() || marketState.orderBook.askCount() < 2 || marketState.orderBook.bidCount() < 2){
        // std::cout
        // << "marketState.getHasLastTrade()=" << marketState.getHasLastTrade()
        // << " marketState.orderBook.askCount()=" << marketState.orderBook.askCount()
        // << " marketState.orderBook.bidCount()=" << marketState.orderBook.bidCount()
        // << std::endl;
        return std::nullopt;
    }

    OrderBookMetricsEntry e;
    if (mask_ & timestampOfReceive) {
        e.timestampOfReceive = marketState.getLastTimestampOfReceive();
    }
    if (mask_ & market) {
        e.market = static_cast<uint8_t>(marketState.getMarket());
    }
    if (mask_ & symbol) {
        e.symbol = static_cast<uint8_t>(marketState.getSymbol());
    }
    if (mask_ & streamType) {
        e.streamType = static_cast<uint8_t>(marketState.getLastStreamType());
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
        e.queueCountFlowDiff = SingleVariableCounter::calculateQueueCountFlowDiff(marketState.orderBook);
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
    if (mask_ & bestHundredVolumeDiff){
        e.bestHundredVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 100);
    }
    if (mask_ & bestThreeHundredVolumeDiff){
        e.bestThreeHundredVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 300);
    }
    if (mask_ & bestFiveHundredVolumeDiff){
        e.bestFiveHundredVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 500);
    }
    if (mask_ & bestSevenHundredVolumeDiff){
        e.bestSevenHundredVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 700);
    }
    if (mask_ & bestThousandVolumeDiff){
        e.bestThousandVolumeDiff = SingleVariableCounter::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 1000);
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
    if (mask_ & bestHundredVolumeImbalance){
        e.bestHundredVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 100);
    }
    if (mask_ & bestThreeHundredVolumeImbalance){
        e.bestThreeHundredVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 300);
    }
    if (mask_ & bestFiveHundredVolumeImbalance){
        e.bestFiveHundredVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 500);
    }
    if (mask_ & bestSevenHundredVolumeImbalance){
        e.bestSevenHundredVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 700);
    }
    if (mask_ & bestThousandVolumeImbalance){
        e.bestThousandVolumeImbalance = SingleVariableCounter::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 1000);
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
    if (mask_ & bestHundredVolumeLogRatio){
        e.bestHundredVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 100);
    }
    if (mask_ & bestThreeHundredVolumeLogRatio){
        e.bestThreeHundredVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 300);
    }
    if (mask_ & bestFiveHundredVolumeLogRatio){
        e.bestFiveHundredVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 500);
    }
    if (mask_ & bestSevenHundredVolumeLogRatio){
        e.bestSevenHundredVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 700);
    }
    if (mask_ & bestThousandVolumeLogRatio){
        e.bestThousandVolumeLogRatio = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 1000);
    }

    if (mask_ & bestFiveVolumeLogRatioXVolume){
        e.bestFiveVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 5);
    }
    if (mask_ & bestFiftyVolumeLogRatioXVolume){
        e.bestFiftyVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 50);
    }
    if (mask_ & bestHundredVolumeLogRatioXVolume){
        e.bestHundredVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 100);
    }
    if (mask_ & bestFiveHundredVolumeLogRatioXVolume){
        e.bestFiveHundredVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 500);
    }
    if (mask_ & bestThousandVolumeLogRatioXVolume){
        e.bestThousandVolumeLogRatioXVolume = SingleVariableCounter::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 1000);
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

    if (mask_ & sinTime24h) {
        e.sinTime24h = SingleVariableCounter::calculateSinTime24h(marketState.getLastTimestampOfReceive());
    }
    if (mask_ & longSinTime24h) {
        e.longSinTime24h = SingleVariableCounter::calculateLongSinTime24h(marketState.getLastTimestampOfReceive());
    }
    if (mask_ & cosTime24h) {
        e.cosTime24h = SingleVariableCounter::calculateCosTime24h(marketState.getLastTimestampOfReceive());
    }
    if (mask_ & isAsiaTime) {
        e.isAsiaTime = SingleVariableCounter::calculateIsAsiaTime(marketState.getLastTimestampOfReceive());
    }
    if (mask_ & isEuropeTime) {
        e.isEuropeTime = SingleVariableCounter::calculateIsEuropeTime(marketState.getLastTimestampOfReceive());
    }
    if (mask_ & isUSATime) {
        e.isUSATime = SingleVariableCounter::calculateIsUSATime(marketState.getLastTimestampOfReceive());
    }

    if (mask_ & depthCount1Seconds) {
        e.depthCount1Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 1);
    }
    if (mask_ & depthCount3Seconds) {
        e.depthCount3Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 3);
    }
    if (mask_ & depthCount5Seconds) {
        e.depthCount5Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 5);
    }
    if (mask_ & depthCount10Seconds) {
        e.depthCount10Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 10);
    }
    if (mask_ & depthCount15Seconds) {
        e.depthCount15Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 15);
    }
    if (mask_ & depthCount30Seconds) {
        e.depthCount30Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 30);
    }
    if (mask_ & depthCount60Seconds) {
        e.depthCount60Seconds = SingleVariableCounter::calculateDepthCount(marketState.depthRoll, 60);
    }

    if (mask_ & depthCountDiff1Seconds) {
        e.depthCountDiff1Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 1);
    }
    if (mask_ & depthCountDiff3Seconds) {
        e.depthCountDiff3Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 3);
    }
    if (mask_ & depthCountDiff5Seconds) {
        e.depthCountDiff5Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 5);
    }
    if (mask_ & depthCountDiff10Seconds) {
        e.depthCountDiff10Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 10);
    }
    if (mask_ & depthCountDiff15Seconds) {
        e.depthCountDiff15Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 15);
    }
    if (mask_ & depthCountDiff30Seconds) {
        e.depthCountDiff30Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 30);
    }
    if (mask_ & depthCountDiff60Seconds) {
        e.depthCountDiff60Seconds = SingleVariableCounter::calculateDepthCountDiff(marketState.depthRoll, 60);
    }

    if (mask_ & depthCountImbalance1Seconds) {
        e.depthCountImbalance1Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 1);
    }
    if (mask_ & depthCountImbalance3Seconds) {
        e.depthCountImbalance3Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 3);
    }
    if (mask_ & depthCountImbalance5Seconds) {
        e.depthCountImbalance5Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 5);
    }
    if (mask_ & depthCountImbalance10Seconds) {
        e.depthCountImbalance10Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 10);
    }
    if (mask_ & depthCountImbalance15Seconds) {
        e.depthCountImbalance15Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 15);
    }
    if (mask_ & depthCountImbalance30Seconds) {
        e.depthCountImbalance30Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 30);
    }
    if (mask_ & depthCountImbalance60Seconds) {
        e.depthCountImbalance60Seconds = SingleVariableCounter::calculateDepthCountImbalance(marketState.depthRoll, 60);
    }

    if (mask_ & depthCountFisherImbalance1Seconds) {
        e.depthCountFisherImbalance1Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 1);
    }
    if (mask_ & depthCountFisherImbalance3Seconds) {
        e.depthCountFisherImbalance3Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 3);
    }
    if (mask_ & depthCountFisherImbalance5Seconds) {
        e.depthCountFisherImbalance5Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 5);
    }
    if (mask_ & depthCountFisherImbalance10Seconds) {
        e.depthCountFisherImbalance10Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 10);
    }
    if (mask_ & depthCountFisherImbalance15Seconds) {
        e.depthCountFisherImbalance15Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 15);
    }
    if (mask_ & depthCountFisherImbalance30Seconds) {
        e.depthCountFisherImbalance30Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 30);
    }
    if (mask_ & depthCountFisherImbalance60Seconds) {
        e.depthCountFisherImbalance60Seconds = SingleVariableCounter::calculateDepthCountFisherImbalance(marketState.depthRoll, 60);
    }

    if (mask_ & depthCountLogRatio1Seconds) {
        e.depthCountLogRatio1Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 1);
    }
    if (mask_ & depthCountLogRatio3Seconds) {
        e.depthCountLogRatio3Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 3);
    }
    if (mask_ & depthCountLogRatio5Seconds) {
        e.depthCountLogRatio5Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 5);
    }
    if (mask_ & depthCountLogRatio10Seconds) {
        e.depthCountLogRatio10Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 10);
    }
    if (mask_ & depthCountLogRatio15Seconds) {
        e.depthCountLogRatio15Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 15);
    }
    if (mask_ & depthCountLogRatio30Seconds) {
        e.depthCountLogRatio30Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 30);
    }
    if (mask_ & depthCountLogRatio60Seconds) {
        e.depthCountLogRatio60Seconds = SingleVariableCounter::calculateDepthCountLogRatio(marketState.depthRoll, 60);
    }

    if (mask_ & depthCountLogRatioXEventCount1Seconds) {
        e.depthCountLogRatioXEventCount1Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 1);
    }
    if (mask_ & depthCountLogRatioXEventCount3Seconds) {
        e.depthCountLogRatioXEventCount3Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 3);
    }
    if (mask_ & depthCountLogRatioXEventCount5Seconds) {
        e.depthCountLogRatioXEventCount5Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 5);
    }
    if (mask_ & depthCountLogRatioXEventCount10Seconds) {
        e.depthCountLogRatioXEventCount10Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 10);
    }
    if (mask_ & depthCountLogRatioXEventCount15Seconds) {
        e.depthCountLogRatioXEventCount15Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 15);
    }
    if (mask_ & depthCountLogRatioXEventCount30Seconds) {
        e.depthCountLogRatioXEventCount30Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 30);
    }
    if (mask_ & depthCountLogRatioXEventCount60Seconds) {
        e.depthCountLogRatioXEventCount60Seconds = SingleVariableCounter::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 60);
    }

    if (mask_ & tradeCount1Seconds) {
        e.tradeCount1Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeCount3Seconds) {
        e.tradeCount3Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeCount5Seconds) {
        e.tradeCount5Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeCount10Seconds) {
        e.tradeCount10Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeCount15Seconds) {
        e.tradeCount15Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeCount30Seconds) {
        e.tradeCount30Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeCount60Seconds) {
        e.tradeCount60Seconds = SingleVariableCounter::calculateTradeCount(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeCountDiff1Seconds) {
        e.tradeCountDiff1Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeCountDiff3Seconds) {
        e.tradeCountDiff3Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeCountDiff5Seconds) {
        e.tradeCountDiff5Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeCountDiff10Seconds) {
        e.tradeCountDiff10Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeCountDiff15Seconds) {
        e.tradeCountDiff15Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeCountDiff30Seconds) {
        e.tradeCountDiff30Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeCountDiff60Seconds) {
        e.tradeCountDiff60Seconds = SingleVariableCounter::calculateTradeCountDiff(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeCountImbalance1Seconds) {
        e.tradeCountImbalance1Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeCountImbalance3Seconds) {
        e.tradeCountImbalance3Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeCountImbalance5Seconds) {
        e.tradeCountImbalance5Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeCountImbalance10Seconds) {
        e.tradeCountImbalance10Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeCountImbalance15Seconds) {
        e.tradeCountImbalance15Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeCountImbalance30Seconds) {
        e.tradeCountImbalance30Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeCountImbalance60Seconds) {
        e.tradeCountImbalance60Seconds = SingleVariableCounter::calculateTradeCountImbalance(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeCountFisherImbalance1Seconds) {
        e.tradeCountFisherImbalance1Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeCountFisherImbalance3Seconds) {
        e.tradeCountFisherImbalance3Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeCountFisherImbalance5Seconds) {
        e.tradeCountFisherImbalance5Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeCountFisherImbalance10Seconds) {
        e.tradeCountFisherImbalance10Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeCountFisherImbalance15Seconds) {
        e.tradeCountFisherImbalance15Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeCountFisherImbalance30Seconds) {
        e.tradeCountFisherImbalance30Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeCountFisherImbalance60Seconds) {
        e.tradeCountFisherImbalance60Seconds = SingleVariableCounter::calculateTradeCountFisherImbalance(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeCountLogRatio1Seconds) {
        e.tradeCountLogRatio1Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeCountLogRatio3Seconds) {
        e.tradeCountLogRatio3Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeCountLogRatio5Seconds) {
        e.tradeCountLogRatio5Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeCountLogRatio10Seconds) {
        e.tradeCountLogRatio10Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeCountLogRatio15Seconds) {
        e.tradeCountLogRatio15Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeCountLogRatio30Seconds) {
        e.tradeCountLogRatio30Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeCountLogRatio60Seconds) {
        e.tradeCountLogRatio60Seconds = SingleVariableCounter::calculateTradeCountLogRatio(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeVolumeSum1Seconds) {
        e.tradeVolumeSum1Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeVolumeSum3Seconds) {
        e.tradeVolumeSum3Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeVolumeSum5Seconds) {
        e.tradeVolumeSum5Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeVolumeSum10Seconds) {
        e.tradeVolumeSum10Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeVolumeSum15Seconds) {
        e.tradeVolumeSum15Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeVolumeSum30Seconds) {
        e.tradeVolumeSum30Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeVolumeSum60Seconds) {
        e.tradeVolumeSum60Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 60);
    }
    if (mask_ & tradeVolumeSum600Seconds) {
        e.tradeVolumeSum600Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 600);
    }
    if (mask_ & tradeVolumeSum1200Seconds) {
        e.tradeVolumeSum1200Seconds = SingleVariableCounter::calculateTradeVolumeSum(marketState.tradeRoll, 1200);
    }

    if (mask_ & tradeVolumeDiff1Seconds) {
        e.tradeVolumeDiff1Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeVolumeDiff3Seconds) {
        e.tradeVolumeDiff3Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeVolumeDiff5Seconds) {
        e.tradeVolumeDiff5Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeVolumeDiff10Seconds) {
        e.tradeVolumeDiff10Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeVolumeDiff15Seconds) {
        e.tradeVolumeDiff15Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeVolumeDiff30Seconds) {
        e.tradeVolumeDiff30Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeVolumeDiff60Seconds) {
        e.tradeVolumeDiff60Seconds = SingleVariableCounter::calculateTradeVolumeDiff(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeVolumeImbalance1Seconds) {
        e.tradeVolumeImbalance1Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeVolumeImbalance3Seconds) {
        e.tradeVolumeImbalance3Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeVolumeImbalance5Seconds) {
        e.tradeVolumeImbalance5Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeVolumeImbalance10Seconds) {
        e.tradeVolumeImbalance10Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeVolumeImbalance15Seconds) {
        e.tradeVolumeImbalance15Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeVolumeImbalance30Seconds) {
        e.tradeVolumeImbalance30Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeVolumeImbalance60Seconds) {
        e.tradeVolumeImbalance60Seconds = SingleVariableCounter::calculateTradeVolumeImbalance(marketState.tradeRoll, 60);
    }

    if (mask_ & tradeVolumeLogRatio1Seconds) {
        e.tradeVolumeLogRatio1Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 1);
    }
    if (mask_ & tradeVolumeLogRatio3Seconds) {
        e.tradeVolumeLogRatio3Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 3);
    }
    if (mask_ & tradeVolumeLogRatio5Seconds) {
        e.tradeVolumeLogRatio5Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 5);
    }
    if (mask_ & tradeVolumeLogRatio10Seconds) {
        e.tradeVolumeLogRatio10Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 10);
    }
    if (mask_ & tradeVolumeLogRatio15Seconds) {
        e.tradeVolumeLogRatio15Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 15);
    }
    if (mask_ & tradeVolumeLogRatio30Seconds) {
        e.tradeVolumeLogRatio30Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 30);
    }
    if (mask_ & tradeVolumeLogRatio60Seconds) {
        e.tradeVolumeLogRatio60Seconds = SingleVariableCounter::calculateTradeVolumeLogRatio(marketState.tradeRoll, 60);
    }

    if (mask_ & biggestSingleBuyTradeVolume1Seconds) {
        e.biggestSingleBuyTradeVolume1Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 1);
    }
    if (mask_ & biggestSingleBuyTradeVolume3Seconds) {
        e.biggestSingleBuyTradeVolume3Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 3);
    }
    if (mask_ & biggestSingleBuyTradeVolume5Seconds) {
        e.biggestSingleBuyTradeVolume5Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 5);
    }
    if (mask_ & biggestSingleBuyTradeVolume10Seconds) {
        e.biggestSingleBuyTradeVolume10Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 10);
    }
    if (mask_ & biggestSingleBuyTradeVolume15Seconds) {
        e.biggestSingleBuyTradeVolume15Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 15);
    }
    if (mask_ & biggestSingleBuyTradeVolume30Seconds) {
        e.biggestSingleBuyTradeVolume30Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 30);
    }
    if (mask_ & biggestSingleBuyTradeVolume60Seconds) {
        e.biggestSingleBuyTradeVolume60Seconds = SingleVariableCounter::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 60);
    }

    if (mask_ & biggestSingleSellTradeVolume1Seconds) {
        e.biggestSingleSellTradeVolume1Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 1);
    }
    if (mask_ & biggestSingleSellTradeVolume3Seconds) {
        e.biggestSingleSellTradeVolume3Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 3);
    }
    if (mask_ & biggestSingleSellTradeVolume5Seconds) {
        e.biggestSingleSellTradeVolume5Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 5);
    }
    if (mask_ & biggestSingleSellTradeVolume10Seconds) {
        e.biggestSingleSellTradeVolume10Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 10);
    }
    if (mask_ & biggestSingleSellTradeVolume15Seconds) {
        e.biggestSingleSellTradeVolume15Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 15);
    }
    if (mask_ & biggestSingleSellTradeVolume30Seconds) {
        e.biggestSingleSellTradeVolume30Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 30);
    }
    if (mask_ & biggestSingleSellTradeVolume60Seconds) {
        e.biggestSingleSellTradeVolume60Seconds = SingleVariableCounter::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 60);
    }

    if (mask_ & avgTradeSizeDiff1Seconds) {
        e.avgTradeSizeDiff1Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 1);
    }
    if (mask_ & avgTradeSizeDiff3Seconds) {
        e.avgTradeSizeDiff3Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 3);
    }
    if (mask_ & avgTradeSizeDiff5Seconds) {
        e.avgTradeSizeDiff5Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 5);
    }
    if (mask_ & avgTradeSizeDiff10Seconds) {
        e.avgTradeSizeDiff10Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 10);
    }
    if (mask_ & avgTradeSizeDiff15Seconds) {
        e.avgTradeSizeDiff15Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 15);
    }
    if (mask_ & avgTradeSizeDiff30Seconds) {
        e.avgTradeSizeDiff30Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 30);
    }
    if (mask_ & avgTradeSizeDiff60Seconds) {
        e.avgTradeSizeDiff60Seconds = SingleVariableCounter::calculateAvgTradeSizeDiff(marketState.tradeRoll, 60);
    }

    if (mask_ & avgTradeSizeImbalance1Seconds) {
        e.avgTradeSizeImbalance1Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 1);
    }
    if (mask_ & avgTradeSizeImbalance3Seconds) {
        e.avgTradeSizeImbalance3Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 3);
    }
    if (mask_ & avgTradeSizeImbalance5Seconds) {
        e.avgTradeSizeImbalance5Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 5);
    }
    if (mask_ & avgTradeSizeImbalance10Seconds) {
        e.avgTradeSizeImbalance10Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 10);
    }
    if (mask_ & avgTradeSizeImbalance15Seconds) {
        e.avgTradeSizeImbalance15Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 15);
    }
    if (mask_ & avgTradeSizeImbalance30Seconds) {
        e.avgTradeSizeImbalance30Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 30);
    }
    if (mask_ & avgTradeSizeImbalance60Seconds) {
        e.avgTradeSizeImbalance60Seconds = SingleVariableCounter::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 60);
    }

    if (mask_ & avgTradeSizeLogRatio1Seconds) {
        e.avgTradeSizeLogRatio1Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 1);
    }
    if (mask_ & avgTradeSizeLogRatio3Seconds) {
        e.avgTradeSizeLogRatio3Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 3);
    }
    if (mask_ & avgTradeSizeLogRatio5Seconds) {
        e.avgTradeSizeLogRatio5Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 5);
    }
    if (mask_ & avgTradeSizeLogRatio10Seconds) {
        e.avgTradeSizeLogRatio10Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 10);
    }
    if (mask_ & avgTradeSizeLogRatio15Seconds) {
        e.avgTradeSizeLogRatio15Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 15);
    }
    if (mask_ & avgTradeSizeLogRatio30Seconds) {
        e.avgTradeSizeLogRatio30Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 30);
    }
    if (mask_ & avgTradeSizeLogRatio60Seconds) {
        e.avgTradeSizeLogRatio60Seconds = SingleVariableCounter::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 60);
    }

    if (mask_ & priceDifference1Seconds) {
        e.priceDifference1Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 1);
    }
    if (mask_ & priceDifference3Seconds) {
        e.priceDifference3Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 3);
    }
    if (mask_ & priceDifference5Seconds) {
        e.priceDifference5Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 5);
    }
    if (mask_ & priceDifference10Seconds) {
        e.priceDifference10Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 10);
    }
    if (mask_ & priceDifference15Seconds) {
        e.priceDifference15Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 15);
    }
    if (mask_ & priceDifference30Seconds) {
        e.priceDifference30Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 30);
    }
    if (mask_ & priceDifference60Seconds) {
        e.priceDifference60Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 60);
    }
    if (mask_ & priceDifference600Seconds) {
        e.priceDifference600Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 600);
    }
    if (mask_ & priceDifference1200Seconds) {
        e.priceDifference1200Seconds = SingleVariableCounter::calculatePriceDifference(marketState.tradeRoll, 1200);
    }

    if (mask_ & rateOfReturn1Seconds) {
        e.rateOfReturn1Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 1);
    }
    if (mask_ & rateOfReturn3Seconds) {
        e.rateOfReturn3Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 3);
    }
    if (mask_ & rateOfReturn5Seconds) {
        e.rateOfReturn5Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 5);
    }
    if (mask_ & rateOfReturn10Seconds) {
        e.rateOfReturn10Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 10);
    }
    if (mask_ & rateOfReturn15Seconds) {
        e.rateOfReturn15Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 15);
    }
    if (mask_ & rateOfReturn30Seconds) {
        e.rateOfReturn30Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 30);
    }
    if (mask_ & rateOfReturn60Seconds) {
        e.rateOfReturn60Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 60);
    }
    if (mask_ & rateOfReturn600Seconds) {
        e.rateOfReturn600Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 600);
    }
    if (mask_ & rateOfReturn1200Seconds) {
        e.rateOfReturn1200Seconds = SingleVariableCounter::calculateRateOfReturn(marketState.tradeRoll, 1200);
    }

    if (mask_ & logReturnRatio1Seconds) {
        e.logReturnRatio1Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 1);
    }
    if (mask_ & logReturnRatio3Seconds) {
        e.logReturnRatio3Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 3);
    }
    if (mask_ & logReturnRatio5Seconds) {
        e.logReturnRatio5Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 5);
    }
    if (mask_ & logReturnRatio10Seconds) {
        e.logReturnRatio10Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 10);
    }
    if (mask_ & logReturnRatio15Seconds) {
        e.logReturnRatio15Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 15);
    }
    if (mask_ & logReturnRatio30Seconds) {
        e.logReturnRatio30Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 30);
    }
    if (mask_ & logReturnRatio60Seconds) {
        e.logReturnRatio60Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 60);
    }
    if (mask_ & logReturnRatio600Seconds) {
        e.logReturnRatio600Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 600);
    }
    if (mask_ & logReturnRatio1200Seconds) {
        e.logReturnRatio1200Seconds = SingleVariableCounter::calculateLogReturnRatio(marketState.tradeRoll, 1200);
    }

    if (mask_ & smaLogRatio3Seconds) {
        e.smaLogRatio3Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 3);
    }
    if (mask_ & smaLogRatio5Seconds) {
        e.smaLogRatio5Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 5);
    }
    if (mask_ & smaLogRatio10Seconds) {
        e.smaLogRatio10Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 10);
    }
    if (mask_ & smaLogRatio15Seconds) {
        e.smaLogRatio15Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 15);
    }
    if (mask_ & smaLogRatio30Seconds) {
        e.smaLogRatio30Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 30);
    }
    if (mask_ & smaLogRatio60Seconds) {
        e.smaLogRatio60Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 60);
    }
    if (mask_ & smaLogRatio300Seconds) {
        e.smaLogRatio300Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 300);
    }
    if (mask_ & smaLogRatio600Seconds) {
        e.smaLogRatio600Seconds = SingleVariableCounter::calculateSMALogRatio(marketState.tradeRoll, 600);
    }

    if (mask_ & hlc3tLogRatio300Seconds) {
        e.hlc3tLogRatio300Seconds = SingleVariableCounter::calculateHLC3TLogRatio(marketState.tradeRoll, 300);
    }
    if (mask_ & hlc3tLogRatio600Seconds) {
        e.hlc3tLogRatio600Seconds = SingleVariableCounter::calculateHLC3TLogRatio(marketState.tradeRoll, 600);
    }
    if (mask_ & hlc3tLogRatio1200Seconds) {
        e.hlc3tLogRatio1200Seconds = SingleVariableCounter::calculateHLC3TLogRatio(marketState.tradeRoll, 1200);
    }

    if (mask_ & bollingerBandsDistance300Seconds) {
        e.bollingerBandsDistance300Seconds = SingleVariableCounter::calculateBollingerBandsDistance(marketState.tradeRoll, 300);
    }
    if (mask_ & bollingerBandsDistance600Seconds) {
        e.bollingerBandsDistance600Seconds = SingleVariableCounter::calculateBollingerBandsDistance(marketState.tradeRoll, 600);
    }
    if (mask_ & bollingerBandsDistance1200Seconds) {
        e.bollingerBandsDistance1200Seconds = SingleVariableCounter::calculateBollingerBandsDistance(marketState.tradeRoll, 1200);
    }

    if (mask_ & priceDistance1Seconds) {
        e.priceDistance1Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 1);
    }
    if (mask_ & priceDistance3Seconds) {
        e.priceDistance3Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 3);
    }
    if (mask_ & priceDistance5Seconds) {
        e.priceDistance5Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 5);
    }
    if (mask_ & priceDistance10Seconds) {
        e.priceDistance10Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 10);
    }
    if (mask_ & priceDistance15Seconds) {
        e.priceDistance15Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 15);
    }
    if (mask_ & priceDistance30Seconds) {
        e.priceDistance30Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 30);
    }
    if (mask_ & priceDistance60Seconds) {
        e.priceDistance60Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 60);
    }
    if (mask_ & priceDistance300Seconds) {
        e.priceDistance300Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 300);
    }
    if (mask_ & priceDistance600Seconds) {
        e.priceDistance600Seconds = SingleVariableCounter::calculatePriceDistance(marketState.tradeRoll, 600);
    }

    if (mask_ & logKylesLambda1Seconds) {
        e.logKylesLambda1Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 1);
    }
    if (mask_ & logKylesLambda3Seconds) {
        e.logKylesLambda3Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 3);
    }
    if (mask_ & logKylesLambda5Seconds) {
        e.logKylesLambda5Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 5);
    }
    if (mask_ & logKylesLambda10Seconds) {
        e.logKylesLambda10Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 10);
    }
    if (mask_ & logKylesLambda15Seconds) {
        e.logKylesLambda15Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 15);
    }
    if (mask_ & logKylesLambda30Seconds) {
        e.logKylesLambda30Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 30);
    }
    if (mask_ & logKylesLambda60Seconds) {
        e.logKylesLambda60Seconds = SingleVariableCounter::calculateLogKylesLambda(marketState.tradeRoll, 60);
    }

    if (mask_ & deltaPVT1Seconds) {
        e.deltaPVT1Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 1);
    }
    if (mask_ & deltaPVT3Seconds) {
        e.deltaPVT3Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 3);
    }
    if (mask_ & deltaPVT5Seconds) {
        e.deltaPVT5Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 5);
    }
    if (mask_ & deltaPVT10Seconds) {
        e.deltaPVT10Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 10);
    }
    if (mask_ & deltaPVT15Seconds) {
        e.deltaPVT15Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 15);
    }
    if (mask_ & deltaPVT30Seconds) {
        e.deltaPVT30Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 30);
    }
    if (mask_ & deltaPVT60Seconds) {
        e.deltaPVT60Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 60);
    }
    if (mask_ & deltaPVT600Seconds) {
        e.deltaPVT600Seconds = SingleVariableCounter::calculateDeltaPVT(marketState.tradeRoll, 600);
    }

    return e;
}
