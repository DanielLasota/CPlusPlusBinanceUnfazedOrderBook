#include "MetricsCalculator.h"
#include "FeatureCalculator.h"

namespace FC = FeatureCalculator;

std::optional<MetricsEntry> MetricsCalculator::calc(const MarketState& marketState) const {
    if (!marketState.getHasLastTrade() || marketState.orderBook.askCount() < 2 || marketState.orderBook.bidCount() < 2){
        // std::cout
        // << "marketState.getHasLastTrade()=" << marketState.getHasLastTrade()
        // << " marketState.orderBook.askCount()=" << marketState.orderBook.askCount()
        // << " marketState.orderBook.bidCount()=" << marketState.orderBook.bidCount()
        // << std::endl;
        return std::nullopt;
    }

    MetricsEntry e;
    if (mask & timestampOfReceive) {
        e.timestampOfReceive = marketState.getLastTimestampOfReceive();
    }
    if (mask & market) {
        e.market = static_cast<uint8_t>(marketState.getMarket());
    }
    if (mask & symbol) {
        e.symbol = static_cast<uint8_t>(marketState.getSymbol());
    }
    if (mask & streamType) {
        e.streamType = static_cast<uint8_t>(marketState.getLastStreamType());
    }
    if (mask & bestAskPrice) {
        e.bestAskPrice = FC::calculateBestAskPrice(marketState.orderBook);
    }
    if (mask & bestBidPrice) {
        e.bestBidPrice = FC::calculateBestBidPrice(marketState.orderBook);
    }
    if (mask & midPrice) {
        e.midPrice = FC::calculateMidPrice(marketState.orderBook);
    }

    if (mask & microPriceDiff){
        e.microPriceDiff = FC::calculateMicroPriceDiff(marketState.orderBook);
    }
    if (mask & microPriceImbalance){
        e.microPriceImbalance = FC::calculateMicroPriceImbalance(marketState.orderBook);
    }
    if (mask & microPriceFisherImbalance){
        e.microPriceFisherImbalance = FC::calculateMicroPriceFisherImbalance(marketState.orderBook);
    }
    if (mask & microPriceDeviation){
        e.microPriceDeviation = FC::calculateMicroPriceDeviation(marketState.orderBook);
    }
    if (mask & microPriceLogRatio){
        e.microPriceLogRatio = FC::calculateMicroPriceLogRatio(marketState.orderBook);
    }

    if (mask & bestBidQuantity){
        e.bestBidQuantity = FC::calculateBestBidQuantity(marketState.orderBook);
    }
    if (mask & bestAskQuantity){
        e.bestAskQuantity = FC::calculateBestAskQuantity(marketState.orderBook);
    }

    if (mask & bestOrderFlowDiff){
        e.bestOrderFlowDiff = FC::calculateBestOrderFlowDiff(marketState.orderBook);
    }
    if (mask & bestOrderFlowImbalance){
        e.bestOrderFlowImbalance = FC::calculateBestOrderFlowImbalance(marketState.orderBook);
    }
    if (mask & bestOrderFlowFisherImbalance){
        e.bestOrderFlowFisherImbalance = FC::calculateBestOrderFlowFisherImbalance(marketState.orderBook);
    }

    if (mask & bestOrderFlowCKSDiff){
        e.bestOrderFlowCKSDiff = FC::calculateBestOrderFlowCKSDiff(marketState.orderBook);
    }
    if (mask & bestOrderFlowCKSImbalance){
        e.bestOrderFlowCKSImbalance = FC::calculateBestOrderFlowCKSImbalance(marketState.orderBook);
    }
    if (mask & bestOrderFlowCKSFisherImbalance){
        e.bestOrderFlowCKSFisherImbalance = FC::calculateBestOrderFlowCKSFisherImbalance(marketState.orderBook);
    }

    if (mask & orderFlowDiff){
        e.orderFlowDiff = FC::calculateOrderFlowDiff(marketState.orderBook);
    }
    if (mask & orderFlowImbalance){
        e.orderFlowImbalance = FC::calculateOrderFlowImbalance(marketState.orderBook);
    }
    if (mask & orderFlowFisherImbalance){
        e.orderFlowFisherImbalance = FC::calculateOrderFlowFisherImbalance(marketState.orderBook);
    }

    if (mask & queueCountFlowDiff){
        e.queueCountFlowDiff = FC::calculateQueueCountFlowDiff(marketState.orderBook);
    }
    if (mask & queueCountFlowImbalance){
        e.queueCountFlowImbalance = FC::calculateQueueCountFlowImbalance(marketState.orderBook);
    }
    if (mask & queueCountFlowFisherImbalance){
        e.queueCountFlowFisherImbalance = FC::calculateQueueCountFlowFisherImbalance(marketState.orderBook);
    }

    if (mask & bestVolumeDiff) {
        e.bestVolumeDiff = FC::calculateBestVolumeDiff(marketState.orderBook);
    }
    if (mask & bestVolumeImbalance) {
        e.bestVolumeImbalance = FC::calculateBestVolumeImbalance(marketState.orderBook);
    }
    if (mask & bestVolumeFisherImbalance) {
        e.bestVolumeFisherImbalance = FC::calculateBestVolumeFisherImbalance(marketState.orderBook);
    }
    if (mask & bestVolumeLogRatio) {
        e.bestVolumeLogRatio = FC::calculateBestVolumeLogRatio(marketState.orderBook);
    }
    if (mask & bestVolumeSignedLogRatioXVolume) {
        e.bestVolumeSignedLogRatioXVolume = FC::calculateBestVolumeSignedLogRatioXVolume(marketState.orderBook);
    }

    if (mask & bestTwoVolumeDiff){
        e.bestTwoVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 2);
    }
    if (mask & bestThreeVolumeDiff){
        e.bestThreeVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 3);
    }
    if (mask & bestFiveVolumeDiff){
        e.bestFiveVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 5);
    }
    if (mask & bestTenVolumeDiff){
        e.bestTenVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 10);
    }
    if (mask & bestFifteenVolumeDiff){
        e.bestFifteenVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 15);
    }
    if (mask & bestTwentyVolumeDiff){
        e.bestTwentyVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 20);
    }
    if (mask & bestThirtyVolumeDiff){
        e.bestThirtyVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 30);
    }
    if (mask & bestFiftyVolumeDiff){
        e.bestFiftyVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 50);
    }
    if (mask & bestHundredVolumeDiff){
        e.bestHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 100);
    }
    if (mask & bestThreeHundredVolumeDiff){
        e.bestThreeHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 300);
    }
    if (mask & bestFiveHundredVolumeDiff){
        e.bestFiveHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 500);
    }
    if (mask & bestSevenHundredVolumeDiff){
        e.bestSevenHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 700);
    }
    if (mask & bestThousandVolumeDiff){
        e.bestThousandVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(marketState.orderBook, 1000);
    }

    if (mask & bestTwoVolumeImbalance){
        e.bestTwoVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 2);
    }
    if (mask & bestThreeVolumeImbalance){
        e.bestThreeVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 3);
    }
    if (mask & bestFiveVolumeImbalance){
        e.bestFiveVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 5);
    }
    if (mask & bestTenVolumeImbalance){
        e.bestTenVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 10);
    }
    if (mask & bestFifteenVolumeImbalance){
        e.bestFifteenVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 15);
    }
    if (mask & bestTwentyVolumeImbalance){
        e.bestTwentyVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 20);
    }
    if (mask & bestThirtyVolumeImbalance){
        e.bestThirtyVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 30);
    }
    if (mask & bestFiftyVolumeImbalance){
        e.bestFiftyVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 50);
    }
    if (mask & bestHundredVolumeImbalance){
        e.bestHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 100);
    }
    if (mask & bestThreeHundredVolumeImbalance){
        e.bestThreeHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 300);
    }
    if (mask & bestFiveHundredVolumeImbalance){
        e.bestFiveHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 500);
    }
    if (mask & bestSevenHundredVolumeImbalance){
        e.bestSevenHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 700);
    }
    if (mask & bestThousandVolumeImbalance){
        e.bestThousandVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(marketState.orderBook, 1000);
    }

    if (mask & bestTwoVolumeLogRatio){
        e.bestTwoVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 2);
    }
    if (mask & bestThreeVolumeLogRatio){
        e.bestThreeVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 3);
    }
    if (mask & bestFiveVolumeLogRatio){
        e.bestFiveVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 5);
    }
    if (mask & bestTenVolumeLogRatio){
        e.bestTenVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 10);
    }
    if (mask & bestFifteenVolumeLogRatio){
        e.bestFifteenVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 15);
    }
    if (mask & bestTwentyVolumeLogRatio){
        e.bestTwentyVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 20);
    }
    if (mask & bestThirtyVolumeLogRatio){
        e.bestThirtyVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 30);
    }
    if (mask & bestFiftyVolumeLogRatio){
        e.bestFiftyVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 50);
    }
    if (mask & bestHundredVolumeLogRatio){
        e.bestHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 100);
    }
    if (mask & bestThreeHundredVolumeLogRatio){
        e.bestThreeHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 300);
    }
    if (mask & bestFiveHundredVolumeLogRatio){
        e.bestFiveHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 500);
    }
    if (mask & bestSevenHundredVolumeLogRatio){
        e.bestSevenHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 700);
    }
    if (mask & bestThousandVolumeLogRatio){
        e.bestThousandVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(marketState.orderBook, 1000);
    }

    if (mask & bestFiveVolumeLogRatioXVolume){
        e.bestFiveVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 5);
    }
    if (mask & bestFiftyVolumeLogRatioXVolume){
        e.bestFiftyVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 50);
    }
    if (mask & bestHundredVolumeLogRatioXVolume){
        e.bestHundredVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 100);
    }
    if (mask & bestFiveHundredVolumeLogRatioXVolume){
        e.bestFiveHundredVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 500);
    }
    if (mask & bestThousandVolumeLogRatioXVolume){
        e.bestThousandVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(marketState.orderBook, 1000);
    }

    if (mask & volumeDiff) {
        e.volumeDiff = FC::calculateVolumeDiff(marketState.orderBook);
    }
    if (mask & volumeImbalance) {
        e.volumeImbalance = FC::calculateVolumeImbalance(marketState.orderBook);
    }
    if (mask & volumeLogRatio) {
        e.volumeLogRatio = FC::calculateVolumeLogRatio(marketState.orderBook);
    }
    if (mask & volumeLogRatioXVolume) {
        e.volumeLogRatioXVolume = FC::calculateVolumeLogRatioXVolume(marketState.orderBook);
    }

    if (mask & queueDiff) {
        e.queueDiff = FC::calculateQueueDiff(marketState.orderBook);
    }
    if (mask & queueImbalance) {
        e.queueImbalance = FC::calculateQueueImbalance(marketState.orderBook);
    }
    if (mask & queueLogRatio) {
        e.queueLogRatio = FC::calculateQueueLogRatio(marketState.orderBook);
    }
    if (mask & queueLogRatioXVolume) {
        e.queueLogRatioXVolume = FC::calculateQueueLogRatioXVolume(marketState.orderBook);
    }

    if (mask & gap) {
        e.gap = FC::calculateGap(marketState.orderBook);
    }
    if (mask & isAggressorAsk) {
        e.isAggressorAsk = FC::calculateIsAggressorAsk(&marketState.getLastTrade());
    }

    if (mask & vwapDeviation) {
        e.vwapDeviation = FC::calculateVwapDeviation(marketState.orderBook);
    }
    if (mask & vwapLogRatio) {
        e.vwapLogRatio = FC::calculateVwapLogRatio(marketState.orderBook);
    }

    if (mask & simplifiedSlopeDiff) {
        e.simplifiedSlopeDiff = FC::calculateSimplifiedSlopeDiff(marketState.orderBook);
    }
    if (mask & simplifiedSlopeImbalance) {
        e.simplifiedSlopeImbalance = FC::calculateSimplifiedSlopeImbalance(marketState.orderBook);
    }
    if (mask & simplifiedSlopeLogRatio) {
        e.simplifiedSlopeLogRatio = FC::calculateSimplifiedSlopeLogRatio(marketState.orderBook);
    }
    if (mask & bgcSlopeDiff) {
        e.bgcSlopeDiff = FC::calculateBgcSlopeDiff(marketState.orderBook);
    }
    if (mask & bgcSlopeImbalance) {
        e.bgcSlopeImbalance = FC::calculateBgcSlopeImbalance(marketState.orderBook);
    }
    if (mask & bgcSlopeLogRatio) {
        e.bgcSlopeLogRatio = FC::calculateBgcSlopeLogRatio(marketState.orderBook);
    }

    if (mask & sinTime24h) {
        e.sinTime24h = FC::calculateSinTime24h(marketState.getLastTimestampOfReceive());
    }
    if (mask & longSinTime24h) {
        e.longSinTime24h = FC::calculateLongSinTime24h(marketState.getLastTimestampOfReceive());
    }
    if (mask & cosTime24h) {
        e.cosTime24h = FC::calculateCosTime24h(marketState.getLastTimestampOfReceive());
    }
    if (mask & isAsiaTime) {
        e.isAsiaTime = FC::calculateIsAsiaTime(marketState.getLastTimestampOfReceive());
    }
    if (mask & isEuropeTime) {
        e.isEuropeTime = FC::calculateIsEuropeTime(marketState.getLastTimestampOfReceive());
    }
    if (mask & isUSATime) {
        e.isUSATime = FC::calculateIsUSATime(marketState.getLastTimestampOfReceive());
    }

    if (mask & depthCount1Seconds) {
        e.depthCount1Seconds = FC::calculateDepthCount(marketState.depthRoll, 1);
    }
    if (mask & depthCount3Seconds) {
        e.depthCount3Seconds = FC::calculateDepthCount(marketState.depthRoll, 3);
    }
    if (mask & depthCount5Seconds) {
        e.depthCount5Seconds = FC::calculateDepthCount(marketState.depthRoll, 5);
    }
    if (mask & depthCount10Seconds) {
        e.depthCount10Seconds = FC::calculateDepthCount(marketState.depthRoll, 10);
    }
    if (mask & depthCount15Seconds) {
        e.depthCount15Seconds = FC::calculateDepthCount(marketState.depthRoll, 15);
    }
    if (mask & depthCount30Seconds) {
        e.depthCount30Seconds = FC::calculateDepthCount(marketState.depthRoll, 30);
    }
    if (mask & depthCount60Seconds) {
        e.depthCount60Seconds = FC::calculateDepthCount(marketState.depthRoll, 60);
    }

    if (mask & depthCountDiff1Seconds) {
        e.depthCountDiff1Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 1);
    }
    if (mask & depthCountDiff3Seconds) {
        e.depthCountDiff3Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 3);
    }
    if (mask & depthCountDiff5Seconds) {
        e.depthCountDiff5Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 5);
    }
    if (mask & depthCountDiff10Seconds) {
        e.depthCountDiff10Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 10);
    }
    if (mask & depthCountDiff15Seconds) {
        e.depthCountDiff15Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 15);
    }
    if (mask & depthCountDiff30Seconds) {
        e.depthCountDiff30Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 30);
    }
    if (mask & depthCountDiff60Seconds) {
        e.depthCountDiff60Seconds = FC::calculateDepthCountDiff(marketState.depthRoll, 60);
    }

    if (mask & depthCountImbalance1Seconds) {
        e.depthCountImbalance1Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 1);
    }
    if (mask & depthCountImbalance3Seconds) {
        e.depthCountImbalance3Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 3);
    }
    if (mask & depthCountImbalance5Seconds) {
        e.depthCountImbalance5Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 5);
    }
    if (mask & depthCountImbalance10Seconds) {
        e.depthCountImbalance10Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 10);
    }
    if (mask & depthCountImbalance15Seconds) {
        e.depthCountImbalance15Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 15);
    }
    if (mask & depthCountImbalance30Seconds) {
        e.depthCountImbalance30Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 30);
    }
    if (mask & depthCountImbalance60Seconds) {
        e.depthCountImbalance60Seconds = FC::calculateDepthCountImbalance(marketState.depthRoll, 60);
    }

    if (mask & depthCountFisherImbalance1Seconds) {
        e.depthCountFisherImbalance1Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 1);
    }
    if (mask & depthCountFisherImbalance3Seconds) {
        e.depthCountFisherImbalance3Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 3);
    }
    if (mask & depthCountFisherImbalance5Seconds) {
        e.depthCountFisherImbalance5Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 5);
    }
    if (mask & depthCountFisherImbalance10Seconds) {
        e.depthCountFisherImbalance10Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 10);
    }
    if (mask & depthCountFisherImbalance15Seconds) {
        e.depthCountFisherImbalance15Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 15);
    }
    if (mask & depthCountFisherImbalance30Seconds) {
        e.depthCountFisherImbalance30Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 30);
    }
    if (mask & depthCountFisherImbalance60Seconds) {
        e.depthCountFisherImbalance60Seconds = FC::calculateDepthCountFisherImbalance(marketState.depthRoll, 60);
    }

    if (mask & depthCountLogRatio1Seconds) {
        e.depthCountLogRatio1Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 1);
    }
    if (mask & depthCountLogRatio3Seconds) {
        e.depthCountLogRatio3Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 3);
    }
    if (mask & depthCountLogRatio5Seconds) {
        e.depthCountLogRatio5Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 5);
    }
    if (mask & depthCountLogRatio10Seconds) {
        e.depthCountLogRatio10Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 10);
    }
    if (mask & depthCountLogRatio15Seconds) {
        e.depthCountLogRatio15Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 15);
    }
    if (mask & depthCountLogRatio30Seconds) {
        e.depthCountLogRatio30Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 30);
    }
    if (mask & depthCountLogRatio60Seconds) {
        e.depthCountLogRatio60Seconds = FC::calculateDepthCountLogRatio(marketState.depthRoll, 60);
    }

    if (mask & depthCountLogRatioXEventCount1Seconds) {
        e.depthCountLogRatioXEventCount1Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 1);
    }
    if (mask & depthCountLogRatioXEventCount3Seconds) {
        e.depthCountLogRatioXEventCount3Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 3);
    }
    if (mask & depthCountLogRatioXEventCount5Seconds) {
        e.depthCountLogRatioXEventCount5Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 5);
    }
    if (mask & depthCountLogRatioXEventCount10Seconds) {
        e.depthCountLogRatioXEventCount10Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 10);
    }
    if (mask & depthCountLogRatioXEventCount15Seconds) {
        e.depthCountLogRatioXEventCount15Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 15);
    }
    if (mask & depthCountLogRatioXEventCount30Seconds) {
        e.depthCountLogRatioXEventCount30Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 30);
    }
    if (mask & depthCountLogRatioXEventCount60Seconds) {
        e.depthCountLogRatioXEventCount60Seconds = FC::calculateDepthCountLogRatioXEventCount(marketState.depthRoll, 60);
    }

    if (mask & tradeCount1Seconds) {
        e.tradeCount1Seconds = FC::calculateTradeCount(marketState.tradeRoll, 1);
    }
    if (mask & tradeCount3Seconds) {
        e.tradeCount3Seconds = FC::calculateTradeCount(marketState.tradeRoll, 3);
    }
    if (mask & tradeCount5Seconds) {
        e.tradeCount5Seconds = FC::calculateTradeCount(marketState.tradeRoll, 5);
    }
    if (mask & tradeCount10Seconds) {
        e.tradeCount10Seconds = FC::calculateTradeCount(marketState.tradeRoll, 10);
    }
    if (mask & tradeCount15Seconds) {
        e.tradeCount15Seconds = FC::calculateTradeCount(marketState.tradeRoll, 15);
    }
    if (mask & tradeCount30Seconds) {
        e.tradeCount30Seconds = FC::calculateTradeCount(marketState.tradeRoll, 30);
    }
    if (mask & tradeCount60Seconds) {
        e.tradeCount60Seconds = FC::calculateTradeCount(marketState.tradeRoll, 60);
    }

    if (mask & tradeCountDiff1Seconds) {
        e.tradeCountDiff1Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 1);
    }
    if (mask & tradeCountDiff3Seconds) {
        e.tradeCountDiff3Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 3);
    }
    if (mask & tradeCountDiff5Seconds) {
        e.tradeCountDiff5Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 5);
    }
    if (mask & tradeCountDiff10Seconds) {
        e.tradeCountDiff10Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 10);
    }
    if (mask & tradeCountDiff15Seconds) {
        e.tradeCountDiff15Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 15);
    }
    if (mask & tradeCountDiff30Seconds) {
        e.tradeCountDiff30Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 30);
    }
    if (mask & tradeCountDiff60Seconds) {
        e.tradeCountDiff60Seconds = FC::calculateTradeCountDiff(marketState.tradeRoll, 60);
    }

    if (mask & tradeCountImbalance1Seconds) {
        e.tradeCountImbalance1Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 1);
    }
    if (mask & tradeCountImbalance3Seconds) {
        e.tradeCountImbalance3Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 3);
    }
    if (mask & tradeCountImbalance5Seconds) {
        e.tradeCountImbalance5Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 5);
    }
    if (mask & tradeCountImbalance10Seconds) {
        e.tradeCountImbalance10Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 10);
    }
    if (mask & tradeCountImbalance15Seconds) {
        e.tradeCountImbalance15Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 15);
    }
    if (mask & tradeCountImbalance30Seconds) {
        e.tradeCountImbalance30Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 30);
    }
    if (mask & tradeCountImbalance60Seconds) {
        e.tradeCountImbalance60Seconds = FC::calculateTradeCountImbalance(marketState.tradeRoll, 60);
    }

    if (mask & tradeCountFisherImbalance1Seconds) {
        e.tradeCountFisherImbalance1Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 1);
    }
    if (mask & tradeCountFisherImbalance3Seconds) {
        e.tradeCountFisherImbalance3Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 3);
    }
    if (mask & tradeCountFisherImbalance5Seconds) {
        e.tradeCountFisherImbalance5Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 5);
    }
    if (mask & tradeCountFisherImbalance10Seconds) {
        e.tradeCountFisherImbalance10Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 10);
    }
    if (mask & tradeCountFisherImbalance15Seconds) {
        e.tradeCountFisherImbalance15Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 15);
    }
    if (mask & tradeCountFisherImbalance30Seconds) {
        e.tradeCountFisherImbalance30Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 30);
    }
    if (mask & tradeCountFisherImbalance60Seconds) {
        e.tradeCountFisherImbalance60Seconds = FC::calculateTradeCountFisherImbalance(marketState.tradeRoll, 60);
    }

    if (mask & tradeCountLogRatio1Seconds) {
        e.tradeCountLogRatio1Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 1);
    }
    if (mask & tradeCountLogRatio3Seconds) {
        e.tradeCountLogRatio3Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 3);
    }
    if (mask & tradeCountLogRatio5Seconds) {
        e.tradeCountLogRatio5Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 5);
    }
    if (mask & tradeCountLogRatio10Seconds) {
        e.tradeCountLogRatio10Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 10);
    }
    if (mask & tradeCountLogRatio15Seconds) {
        e.tradeCountLogRatio15Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 15);
    }
    if (mask & tradeCountLogRatio30Seconds) {
        e.tradeCountLogRatio30Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 30);
    }
    if (mask & tradeCountLogRatio60Seconds) {
        e.tradeCountLogRatio60Seconds = FC::calculateTradeCountLogRatio(marketState.tradeRoll, 60);
    }

    if (mask & tradeVolumeSum1Seconds) {
        e.tradeVolumeSum1Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 1);
    }
    if (mask & tradeVolumeSum3Seconds) {
        e.tradeVolumeSum3Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 3);
    }
    if (mask & tradeVolumeSum5Seconds) {
        e.tradeVolumeSum5Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 5);
    }
    if (mask & tradeVolumeSum10Seconds) {
        e.tradeVolumeSum10Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 10);
    }
    if (mask & tradeVolumeSum15Seconds) {
        e.tradeVolumeSum15Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 15);
    }
    if (mask & tradeVolumeSum30Seconds) {
        e.tradeVolumeSum30Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 30);
    }
    if (mask & tradeVolumeSum60Seconds) {
        e.tradeVolumeSum60Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 60);
    }
    if (mask & tradeVolumeSum600Seconds) {
        e.tradeVolumeSum600Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 600);
    }
    if (mask & tradeVolumeSum1200Seconds) {
        e.tradeVolumeSum1200Seconds = FC::calculateTradeVolumeSum(marketState.tradeRoll, 1200);
    }

    if (mask & tradeVolumeDiff1Seconds) {
        e.tradeVolumeDiff1Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 1);
    }
    if (mask & tradeVolumeDiff3Seconds) {
        e.tradeVolumeDiff3Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 3);
    }
    if (mask & tradeVolumeDiff5Seconds) {
        e.tradeVolumeDiff5Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 5);
    }
    if (mask & tradeVolumeDiff10Seconds) {
        e.tradeVolumeDiff10Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 10);
    }
    if (mask & tradeVolumeDiff15Seconds) {
        e.tradeVolumeDiff15Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 15);
    }
    if (mask & tradeVolumeDiff30Seconds) {
        e.tradeVolumeDiff30Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 30);
    }
    if (mask & tradeVolumeDiff60Seconds) {
        e.tradeVolumeDiff60Seconds = FC::calculateTradeVolumeDiff(marketState.tradeRoll, 60);
    }

    if (mask & tradeVolumeImbalance1Seconds) {
        e.tradeVolumeImbalance1Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 1);
    }
    if (mask & tradeVolumeImbalance3Seconds) {
        e.tradeVolumeImbalance3Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 3);
    }
    if (mask & tradeVolumeImbalance5Seconds) {
        e.tradeVolumeImbalance5Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 5);
    }
    if (mask & tradeVolumeImbalance10Seconds) {
        e.tradeVolumeImbalance10Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 10);
    }
    if (mask & tradeVolumeImbalance15Seconds) {
        e.tradeVolumeImbalance15Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 15);
    }
    if (mask & tradeVolumeImbalance30Seconds) {
        e.tradeVolumeImbalance30Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 30);
    }
    if (mask & tradeVolumeImbalance60Seconds) {
        e.tradeVolumeImbalance60Seconds = FC::calculateTradeVolumeImbalance(marketState.tradeRoll, 60);
    }

    if (mask & tradeVolumeLogRatio1Seconds) {
        e.tradeVolumeLogRatio1Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 1);
    }
    if (mask & tradeVolumeLogRatio3Seconds) {
        e.tradeVolumeLogRatio3Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 3);
    }
    if (mask & tradeVolumeLogRatio5Seconds) {
        e.tradeVolumeLogRatio5Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 5);
    }
    if (mask & tradeVolumeLogRatio10Seconds) {
        e.tradeVolumeLogRatio10Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 10);
    }
    if (mask & tradeVolumeLogRatio15Seconds) {
        e.tradeVolumeLogRatio15Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 15);
    }
    if (mask & tradeVolumeLogRatio30Seconds) {
        e.tradeVolumeLogRatio30Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 30);
    }
    if (mask & tradeVolumeLogRatio60Seconds) {
        e.tradeVolumeLogRatio60Seconds = FC::calculateTradeVolumeLogRatio(marketState.tradeRoll, 60);
    }

    if (mask & biggestSingleBuyTradeVolume1Seconds) {
        e.biggestSingleBuyTradeVolume1Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 1);
    }
    if (mask & biggestSingleBuyTradeVolume3Seconds) {
        e.biggestSingleBuyTradeVolume3Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 3);
    }
    if (mask & biggestSingleBuyTradeVolume5Seconds) {
        e.biggestSingleBuyTradeVolume5Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 5);
    }
    if (mask & biggestSingleBuyTradeVolume10Seconds) {
        e.biggestSingleBuyTradeVolume10Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 10);
    }
    if (mask & biggestSingleBuyTradeVolume15Seconds) {
        e.biggestSingleBuyTradeVolume15Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 15);
    }
    if (mask & biggestSingleBuyTradeVolume30Seconds) {
        e.biggestSingleBuyTradeVolume30Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 30);
    }
    if (mask & biggestSingleBuyTradeVolume60Seconds) {
        e.biggestSingleBuyTradeVolume60Seconds = FC::calculateBiggestSingleBuyTradeVolume(marketState.tradeRoll, 60);
    }

    if (mask & biggestSingleSellTradeVolume1Seconds) {
        e.biggestSingleSellTradeVolume1Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 1);
    }
    if (mask & biggestSingleSellTradeVolume3Seconds) {
        e.biggestSingleSellTradeVolume3Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 3);
    }
    if (mask & biggestSingleSellTradeVolume5Seconds) {
        e.biggestSingleSellTradeVolume5Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 5);
    }
    if (mask & biggestSingleSellTradeVolume10Seconds) {
        e.biggestSingleSellTradeVolume10Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 10);
    }
    if (mask & biggestSingleSellTradeVolume15Seconds) {
        e.biggestSingleSellTradeVolume15Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 15);
    }
    if (mask & biggestSingleSellTradeVolume30Seconds) {
        e.biggestSingleSellTradeVolume30Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 30);
    }
    if (mask & biggestSingleSellTradeVolume60Seconds) {
        e.biggestSingleSellTradeVolume60Seconds = FC::calculateBiggestSingleSellTradeVolume(marketState.tradeRoll, 60);
    }

    if (mask & avgTradeSizeDiff1Seconds) {
        e.avgTradeSizeDiff1Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 1);
    }
    if (mask & avgTradeSizeDiff3Seconds) {
        e.avgTradeSizeDiff3Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 3);
    }
    if (mask & avgTradeSizeDiff5Seconds) {
        e.avgTradeSizeDiff5Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 5);
    }
    if (mask & avgTradeSizeDiff10Seconds) {
        e.avgTradeSizeDiff10Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 10);
    }
    if (mask & avgTradeSizeDiff15Seconds) {
        e.avgTradeSizeDiff15Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 15);
    }
    if (mask & avgTradeSizeDiff30Seconds) {
        e.avgTradeSizeDiff30Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 30);
    }
    if (mask & avgTradeSizeDiff60Seconds) {
        e.avgTradeSizeDiff60Seconds = FC::calculateAvgTradeSizeDiff(marketState.tradeRoll, 60);
    }

    if (mask & avgTradeSizeImbalance1Seconds) {
        e.avgTradeSizeImbalance1Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 1);
    }
    if (mask & avgTradeSizeImbalance3Seconds) {
        e.avgTradeSizeImbalance3Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 3);
    }
    if (mask & avgTradeSizeImbalance5Seconds) {
        e.avgTradeSizeImbalance5Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 5);
    }
    if (mask & avgTradeSizeImbalance10Seconds) {
        e.avgTradeSizeImbalance10Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 10);
    }
    if (mask & avgTradeSizeImbalance15Seconds) {
        e.avgTradeSizeImbalance15Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 15);
    }
    if (mask & avgTradeSizeImbalance30Seconds) {
        e.avgTradeSizeImbalance30Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 30);
    }
    if (mask & avgTradeSizeImbalance60Seconds) {
        e.avgTradeSizeImbalance60Seconds = FC::calculateAvgTradeSizeImbalance(marketState.tradeRoll, 60);
    }

    if (mask & avgTradeSizeLogRatio1Seconds) {
        e.avgTradeSizeLogRatio1Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 1);
    }
    if (mask & avgTradeSizeLogRatio3Seconds) {
        e.avgTradeSizeLogRatio3Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 3);
    }
    if (mask & avgTradeSizeLogRatio5Seconds) {
        e.avgTradeSizeLogRatio5Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 5);
    }
    if (mask & avgTradeSizeLogRatio10Seconds) {
        e.avgTradeSizeLogRatio10Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 10);
    }
    if (mask & avgTradeSizeLogRatio15Seconds) {
        e.avgTradeSizeLogRatio15Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 15);
    }
    if (mask & avgTradeSizeLogRatio30Seconds) {
        e.avgTradeSizeLogRatio30Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 30);
    }
    if (mask & avgTradeSizeLogRatio60Seconds) {
        e.avgTradeSizeLogRatio60Seconds = FC::calculateAvgTradeSizeLogRatio(marketState.tradeRoll, 60);
    }

    if (mask & priceDifference1Seconds) {
        e.priceDifference1Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 1);
    }
    if (mask & priceDifference3Seconds) {
        e.priceDifference3Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 3);
    }
    if (mask & priceDifference5Seconds) {
        e.priceDifference5Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 5);
    }
    if (mask & priceDifference10Seconds) {
        e.priceDifference10Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 10);
    }
    if (mask & priceDifference15Seconds) {
        e.priceDifference15Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 15);
    }
    if (mask & priceDifference30Seconds) {
        e.priceDifference30Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 30);
    }
    if (mask & priceDifference60Seconds) {
        e.priceDifference60Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 60);
    }
    if (mask & priceDifference600Seconds) {
        e.priceDifference600Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 600);
    }
    if (mask & priceDifference1200Seconds) {
        e.priceDifference1200Seconds = FC::calculatePriceDifference(marketState.tradeRoll, 1200);
    }

    if (mask & rateOfReturn1Seconds) {
        e.rateOfReturn1Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 1);
    }
    if (mask & rateOfReturn3Seconds) {
        e.rateOfReturn3Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 3);
    }
    if (mask & rateOfReturn5Seconds) {
        e.rateOfReturn5Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 5);
    }
    if (mask & rateOfReturn10Seconds) {
        e.rateOfReturn10Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 10);
    }
    if (mask & rateOfReturn15Seconds) {
        e.rateOfReturn15Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 15);
    }
    if (mask & rateOfReturn30Seconds) {
        e.rateOfReturn30Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 30);
    }
    if (mask & rateOfReturn60Seconds) {
        e.rateOfReturn60Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 60);
    }
    if (mask & rateOfReturn600Seconds) {
        e.rateOfReturn600Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 600);
    }
    if (mask & rateOfReturn1200Seconds) {
        e.rateOfReturn1200Seconds = FC::calculateRateOfReturn(marketState.tradeRoll, 1200);
    }

    if (mask & logReturnRatio1Seconds) {
        e.logReturnRatio1Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 1);
    }
    if (mask & logReturnRatio3Seconds) {
        e.logReturnRatio3Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 3);
    }
    if (mask & logReturnRatio5Seconds) {
        e.logReturnRatio5Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 5);
    }
    if (mask & logReturnRatio10Seconds) {
        e.logReturnRatio10Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 10);
    }
    if (mask & logReturnRatio15Seconds) {
        e.logReturnRatio15Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 15);
    }
    if (mask & logReturnRatio30Seconds) {
        e.logReturnRatio30Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 30);
    }
    if (mask & logReturnRatio60Seconds) {
        e.logReturnRatio60Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 60);
    }
    if (mask & logReturnRatio600Seconds) {
        e.logReturnRatio600Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 600);
    }
    if (mask & logReturnRatio1200Seconds) {
        e.logReturnRatio1200Seconds = FC::calculateLogReturnRatio(marketState.tradeRoll, 1200);
    }

    if (mask & smaLogRatio3Seconds) {
        e.smaLogRatio3Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 3);
    }
    if (mask & smaLogRatio5Seconds) {
        e.smaLogRatio5Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 5);
    }
    if (mask & smaLogRatio10Seconds) {
        e.smaLogRatio10Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 10);
    }
    if (mask & smaLogRatio15Seconds) {
        e.smaLogRatio15Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 15);
    }
    if (mask & smaLogRatio30Seconds) {
        e.smaLogRatio30Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 30);
    }
    if (mask & smaLogRatio60Seconds) {
        e.smaLogRatio60Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 60);
    }
    if (mask & smaLogRatio300Seconds) {
        e.smaLogRatio300Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 300);
    }
    if (mask & smaLogRatio600Seconds) {
        e.smaLogRatio600Seconds = FC::calculateSMALogRatio(marketState.tradeRoll, 600);
    }

    if (mask & hlc3tLogRatio300Seconds) {
        e.hlc3tLogRatio300Seconds = FC::calculateHLC3TLogRatio(marketState.tradeRoll, 300);
    }
    if (mask & hlc3tLogRatio600Seconds) {
        e.hlc3tLogRatio600Seconds = FC::calculateHLC3TLogRatio(marketState.tradeRoll, 600);
    }
    if (mask & hlc3tLogRatio1200Seconds) {
        e.hlc3tLogRatio1200Seconds = FC::calculateHLC3TLogRatio(marketState.tradeRoll, 1200);
    }

    if (mask & bollingerBandsDistance300Seconds) {
        e.bollingerBandsDistance300Seconds = FC::calculateBollingerBandsDistance(marketState.tradeRoll, 300);
    }
    if (mask & bollingerBandsDistance600Seconds) {
        e.bollingerBandsDistance600Seconds = FC::calculateBollingerBandsDistance(marketState.tradeRoll, 600);
    }
    if (mask & bollingerBandsDistance1200Seconds) {
        e.bollingerBandsDistance1200Seconds = FC::calculateBollingerBandsDistance(marketState.tradeRoll, 1200);
    }

    if (mask & rfwmaLogRatio5Seconds1i) {
        e.rfwmaLogRatio5Seconds1i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 5, 1);
    }
    if (mask & rfwmaLogRatio30Seconds1i) {
        e.rfwmaLogRatio30Seconds1i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 30, 1);
    }
    if (mask & rfwmaLogRatio60Seconds1i) {
        e.rfwmaLogRatio60Seconds1i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 60, 1);
    }
    if (mask & rfwmaLogRatio120Seconds2i) {
        e.rfwmaLogRatio120Seconds2i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 120, 2);
    }
    if (mask & rfwmaLogRatio120Seconds10i) {
        e.rfwmaLogRatio120Seconds10i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 120, 10);
    }
    if (mask & rfwmaLogRatio300Seconds5i) {
        e.rfwmaLogRatio300Seconds5i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 300, 5);
    }
    if (mask & rfwmaLogRatio600Seconds10i) {
        e.rfwmaLogRatio600Seconds10i = FC::calculateRFWMALogRatio(marketState.tradeRoll, 600, 10);
    }

    if (mask & fwmaLogRatio5Seconds1i) {
        e.fwmaLogRatio5Seconds1i = FC::calculateFWMALogRatio(marketState.tradeRoll, 5, 1);
    }
    if (mask & fwmaLogRatio30Seconds1i) {
        e.fwmaLogRatio30Seconds1i = FC::calculateFWMALogRatio(marketState.tradeRoll, 30, 1);
    }
    if (mask & fwmaLogRatio60Seconds1i) {
        e.fwmaLogRatio60Seconds1i = FC::calculateFWMALogRatio(marketState.tradeRoll, 60, 1);
    }
    if (mask & fwmaLogRatio120Seconds2i) {
        e.fwmaLogRatio120Seconds2i = FC::calculateFWMALogRatio(marketState.tradeRoll, 120, 2);
    }
    if (mask & fwmaLogRatio120Seconds10i) {
        e.fwmaLogRatio120Seconds10i = FC::calculateFWMALogRatio(marketState.tradeRoll, 120, 10);
    }
    if (mask & fwmaLogRatio300Seconds5i) {
        e.fwmaLogRatio300Seconds5i = FC::calculateFWMALogRatio(marketState.tradeRoll, 300, 5);
    }
    if (mask & fwmaLogRatio600Seconds10i) {
        e.fwmaLogRatio600Seconds10i = FC::calculateFWMALogRatio(marketState.tradeRoll, 600, 10);
    }

    if (mask & trendTVal5Seconds || mask & trendCoef5Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 0);
        if (mask & trendTVal5Seconds) {e.trendTVal5Seconds = tVal;}
        if (mask & trendCoef5Seconds) {e.trendCoef5Seconds = coef;}
    }
    if (mask & trendTVal8Seconds || mask & trendCoef8Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 1);
        if (mask & trendTVal8Seconds) {e.trendTVal8Seconds = tVal;}
        if (mask & trendCoef8Seconds) {e.trendCoef8Seconds = coef;}
    }
    if (mask & trendTVal13Seconds || mask & trendCoef13Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 2);
        if (mask & trendTVal13Seconds) {e.trendTVal13Seconds = tVal;}
        if (mask & trendCoef13Seconds) {e.trendCoef13Seconds = coef;}
    }
    if (mask & trendTVal21Seconds || mask & trendCoef21Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 3);
        if (mask & trendTVal21Seconds) {e.trendTVal21Seconds = tVal;}
        if (mask & trendCoef21Seconds) {e.trendCoef21Seconds = coef;}
    }
    if (mask & trendTVal89Seconds || mask & trendCoef89Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 6);
        if (mask & trendTVal89Seconds) {e.trendTVal89Seconds = tVal;}
        if (mask & trendCoef89Seconds) {e.trendCoef89Seconds = coef;}
    }
    if (mask & trendTVal144Seconds || mask & trendCoef144Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 7);
        if (mask & trendTVal144Seconds) {e.trendTVal144Seconds = tVal;}
        if (mask & trendCoef144Seconds) {e.trendCoef144Seconds = coef;}
    }
    if (mask & trendTVal987Seconds || mask & trendCoef987Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(marketState.tradeRoll, 11);
        if (mask & trendTVal987Seconds) {e.trendTVal987Seconds = tVal;}
        if (mask & trendCoef987Seconds) {e.trendCoef987Seconds = coef;}
    }

    if (mask & cValBestTVal || mask & cValBestCoef || mask & cValBestTrendLength) {
        const auto [bestTVal, bestCoef, bestTrendLen] = FC::calculateCVal(marketState.tradeRoll);
        if (mask & cValBestTVal) e.cValBestTVal = bestTVal;
        if (mask & cValBestCoef) e.cValBestCoef = bestCoef;
        if (mask & cValBestTrendLength) e.cValBestTrendLength = bestTrendLen;
    }

    if (mask & priceDistance1Seconds) {
        e.priceDistance1Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 1);
    }
    if (mask & priceDistance3Seconds) {
        e.priceDistance3Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 3);
    }
    if (mask & priceDistance5Seconds) {
        e.priceDistance5Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 5);
    }
    if (mask & priceDistance10Seconds) {
        e.priceDistance10Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 10);
    }
    if (mask & priceDistance15Seconds) {
        e.priceDistance15Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 15);
    }
    if (mask & priceDistance30Seconds) {
        e.priceDistance30Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 30);
    }
    if (mask & priceDistance60Seconds) {
        e.priceDistance60Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 60);
    }
    if (mask & priceDistance300Seconds) {
        e.priceDistance300Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 300);
    }
    if (mask & priceDistance600Seconds) {
        e.priceDistance600Seconds = FC::calculatePriceDistance(marketState.tradeRoll, 600);
    }

    if (mask & logKylesLambda1Seconds) {
        e.logKylesLambda1Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 1);
    }
    if (mask & logKylesLambda3Seconds) {
        e.logKylesLambda3Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 3);
    }
    if (mask & logKylesLambda5Seconds) {
        e.logKylesLambda5Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 5);
    }
    if (mask & logKylesLambda10Seconds) {
        e.logKylesLambda10Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 10);
    }
    if (mask & logKylesLambda15Seconds) {
        e.logKylesLambda15Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 15);
    }
    if (mask & logKylesLambda30Seconds) {
        e.logKylesLambda30Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 30);
    }
    if (mask & logKylesLambda60Seconds) {
        e.logKylesLambda60Seconds = FC::calculateLogKylesLambda(marketState.tradeRoll, 60);
    }

    if (mask & deltaPVT1Seconds) {
        e.deltaPVT1Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 1);
    }
    if (mask & deltaPVT3Seconds) {
        e.deltaPVT3Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 3);
    }
    if (mask & deltaPVT5Seconds) {
        e.deltaPVT5Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 5);
    }
    if (mask & deltaPVT10Seconds) {
        e.deltaPVT10Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 10);
    }
    if (mask & deltaPVT15Seconds) {
        e.deltaPVT15Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 15);
    }
    if (mask & deltaPVT30Seconds) {
        e.deltaPVT30Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 30);
    }
    if (mask & deltaPVT60Seconds) {
        e.deltaPVT60Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 60);
    }
    if (mask & deltaPVT600Seconds) {
        e.deltaPVT600Seconds = FC::calculateDeltaPVT(marketState.tradeRoll, 600);
    }

    return e;
}
