#include "MetricsCalculator.h"
#include "FeatureCalculator.h"

namespace FC = FeatureCalculator;

std::optional<MetricsEntry> MetricsCalculator::calc(const MarketState &ms) const {
    if (!ms.getHasLastTrade() || ms.ob.askCount() < 2 || ms.ob.bidCount() < 2){
        // std::cout
        // << "marketState.getHasLastTrade()=" << marketState.getHasLastTrade()
        // << " marketState.orderBook.askCount()=" << marketState.orderBook.askCount()
        // << " marketState.orderBook.bidCount()=" << marketState.orderBook.bidCount()
        // << std::endl;
        return std::nullopt;
    }

    MetricsEntry e;
    if (mask & timestampOfReceive) {
        e.timestampOfReceive = ms.getLastTimestampOfReceive();
    }
    if (mask & market) {
        e.market = static_cast<uint8_t>(ms.getMarket());
    }
    if (mask & symbol) {
        e.symbol = static_cast<uint8_t>(ms.getSymbol());
    }
    if (mask & streamType) {
        e.streamType = static_cast<uint8_t>(ms.getLastStreamType());
    }
    if (mask & bestAskPrice) {
        e.bestAskPrice = FC::calculateBestAskPrice(ms.ob);
    }
    if (mask & bestBidPrice) {
        e.bestBidPrice = FC::calculateBestBidPrice(ms.ob);
    }
    if (mask & midPrice) {
        e.midPrice = FC::calculateMidPrice(ms.ob);
    }

    if (mask & microPriceDiff){
        e.microPriceDiff = FC::calculateMicroPriceDiff(ms.ob);
    }
    if (mask & microPriceImbalance){
        e.microPriceImbalance = FC::calculateMicroPriceImbalance(ms.ob);
    }
    if (mask & microPriceFisherImbalance){
        e.microPriceFisherImbalance = FC::calculateMicroPriceFisherImbalance(ms.ob);
    }
    if (mask & microPriceDeviation){
        e.microPriceDeviation = FC::calculateMicroPriceDeviation(ms.ob);
    }
    if (mask & microPriceLogRatio){
        e.microPriceLogRatio = FC::calculateMicroPriceLogRatio(ms.ob);
    }

    if (mask & bestBidQuantity){
        e.bestBidQuantity = FC::calculateBestBidQuantity(ms.ob);
    }
    if (mask & bestAskQuantity){
        e.bestAskQuantity = FC::calculateBestAskQuantity(ms.ob);
    }

    if (mask & bestOrderFlowDiff){
        e.bestOrderFlowDiff = FC::calculateBestOrderFlowDiff(ms.ob);
    }
    if (mask & bestOrderFlowImbalance){
        e.bestOrderFlowImbalance = FC::calculateBestOrderFlowImbalance(ms.ob);
    }
    if (mask & bestOrderFlowFisherImbalance){
        e.bestOrderFlowFisherImbalance = FC::calculateBestOrderFlowFisherImbalance(ms.ob);
    }

    if (mask & bestOrderFlowCKSDiff){
        e.bestOrderFlowCKSDiff = FC::calculateBestOrderFlowCKSDiff(ms.ob);
    }
    if (mask & bestOrderFlowCKSImbalance){
        e.bestOrderFlowCKSImbalance = FC::calculateBestOrderFlowCKSImbalance(ms.ob);
    }
    if (mask & bestOrderFlowCKSFisherImbalance){
        e.bestOrderFlowCKSFisherImbalance = FC::calculateBestOrderFlowCKSFisherImbalance(ms.ob);
    }

    if (mask & orderFlowDiff){
        e.orderFlowDiff = FC::calculateOrderFlowDiff(ms.ob);
    }
    if (mask & orderFlowImbalance){
        e.orderFlowImbalance = FC::calculateOrderFlowImbalance(ms.ob);
    }
    if (mask & orderFlowFisherImbalance){
        e.orderFlowFisherImbalance = FC::calculateOrderFlowFisherImbalance(ms.ob);
    }

    if (mask & queueCountFlowDiff){
        e.queueCountFlowDiff = FC::calculateQueueCountFlowDiff(ms.ob);
    }
    if (mask & queueCountFlowImbalance){
        e.queueCountFlowImbalance = FC::calculateQueueCountFlowImbalance(ms.ob);
    }
    if (mask & queueCountFlowFisherImbalance){
        e.queueCountFlowFisherImbalance = FC::calculateQueueCountFlowFisherImbalance(ms.ob);
    }

    if (mask & bestVolumeDiff) {
        e.bestVolumeDiff = FC::calculateBestVolumeDiff(ms.ob);
    }
    if (mask & bestVolumeImbalance) {
        e.bestVolumeImbalance = FC::calculateBestVolumeImbalance(ms.ob);
    }
    if (mask & bestVolumeFisherImbalance) {
        e.bestVolumeFisherImbalance = FC::calculateBestVolumeFisherImbalance(ms.ob);
    }
    if (mask & bestVolumeLogRatio) {
        e.bestVolumeLogRatio = FC::calculateBestVolumeLogRatio(ms.ob);
    }
    if (mask & bestVolumeSignedLogRatioXVolume) {
        e.bestVolumeSignedLogRatioXVolume = FC::calculateBestVolumeSignedLogRatioXVolume(ms.ob);
    }

    if (mask & bestTwoVolumeDiff){
        e.bestTwoVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 2);
    }
    if (mask & bestThreeVolumeDiff){
        e.bestThreeVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 3);
    }
    if (mask & bestFiveVolumeDiff){
        e.bestFiveVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 5);
    }
    if (mask & bestTenVolumeDiff){
        e.bestTenVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 10);
    }
    if (mask & bestFifteenVolumeDiff){
        e.bestFifteenVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 15);
    }
    if (mask & bestTwentyVolumeDiff){
        e.bestTwentyVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 20);
    }
    if (mask & bestThirtyVolumeDiff){
        e.bestThirtyVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 30);
    }
    if (mask & bestFiftyVolumeDiff){
        e.bestFiftyVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 50);
    }
    if (mask & bestHundredVolumeDiff){
        e.bestHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 100);
    }
    if (mask & bestThreeHundredVolumeDiff){
        e.bestThreeHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 300);
    }
    if (mask & bestFiveHundredVolumeDiff){
        e.bestFiveHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 500);
    }
    if (mask & bestSevenHundredVolumeDiff){
        e.bestSevenHundredVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 700);
    }
    if (mask & bestThousandVolumeDiff){
        e.bestThousandVolumeDiff = FC::calculateBestNPriceLevelsVolumeDiff(ms.ob, 1000);
    }

    if (mask & bestTwoVolumeImbalance){
        e.bestTwoVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 2);
    }
    if (mask & bestThreeVolumeImbalance){
        e.bestThreeVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 3);
    }
    if (mask & bestFiveVolumeImbalance){
        e.bestFiveVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 5);
    }
    if (mask & bestTenVolumeImbalance){
        e.bestTenVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 10);
    }
    if (mask & bestFifteenVolumeImbalance){
        e.bestFifteenVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 15);
    }
    if (mask & bestTwentyVolumeImbalance){
        e.bestTwentyVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 20);
    }
    if (mask & bestThirtyVolumeImbalance){
        e.bestThirtyVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 30);
    }
    if (mask & bestFiftyVolumeImbalance){
        e.bestFiftyVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 50);
    }
    if (mask & bestHundredVolumeImbalance){
        e.bestHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 100);
    }
    if (mask & bestThreeHundredVolumeImbalance){
        e.bestThreeHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 300);
    }
    if (mask & bestFiveHundredVolumeImbalance){
        e.bestFiveHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 500);
    }
    if (mask & bestSevenHundredVolumeImbalance){
        e.bestSevenHundredVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 700);
    }
    if (mask & bestThousandVolumeImbalance){
        e.bestThousandVolumeImbalance = FC::calculateBestNPriceLevelsVolumeImbalance(ms.ob, 1000);
    }

    if (mask & bestTwoVolumeLogRatio){
        e.bestTwoVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 2);
    }
    if (mask & bestThreeVolumeLogRatio){
        e.bestThreeVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 3);
    }
    if (mask & bestFiveVolumeLogRatio){
        e.bestFiveVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 5);
    }
    if (mask & bestTenVolumeLogRatio){
        e.bestTenVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 10);
    }
    if (mask & bestFifteenVolumeLogRatio){
        e.bestFifteenVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 15);
    }
    if (mask & bestTwentyVolumeLogRatio){
        e.bestTwentyVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 20);
    }
    if (mask & bestThirtyVolumeLogRatio){
        e.bestThirtyVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 30);
    }
    if (mask & bestFiftyVolumeLogRatio){
        e.bestFiftyVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 50);
    }
    if (mask & bestHundredVolumeLogRatio){
        e.bestHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 100);
    }
    if (mask & bestThreeHundredVolumeLogRatio){
        e.bestThreeHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 300);
    }
    if (mask & bestFiveHundredVolumeLogRatio){
        e.bestFiveHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 500);
    }
    if (mask & bestSevenHundredVolumeLogRatio){
        e.bestSevenHundredVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 700);
    }
    if (mask & bestThousandVolumeLogRatio){
        e.bestThousandVolumeLogRatio = FC::calculateBestNPriceLevelsVolumeLogRatio(ms.ob, 1000);
    }

    if (mask & bestFiveVolumeLogRatioXVolume){
        e.bestFiveVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(ms.ob, 5);
    }
    if (mask & bestFiftyVolumeLogRatioXVolume){
        e.bestFiftyVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(ms.ob, 50);
    }
    if (mask & bestHundredVolumeLogRatioXVolume){
        e.bestHundredVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(ms.ob, 100);
    }
    if (mask & bestFiveHundredVolumeLogRatioXVolume){
        e.bestFiveHundredVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(ms.ob, 500);
    }
    if (mask & bestThousandVolumeLogRatioXVolume){
        e.bestThousandVolumeLogRatioXVolume = FC::calculateBestNPriceLevelsVolumeLogRatioXVolume(ms.ob, 1000);
    }

    if (mask & volumeDiff) {
        e.volumeDiff = FC::calculateVolumeDiff(ms.ob);
    }
    if (mask & volumeImbalance) {
        e.volumeImbalance = FC::calculateVolumeImbalance(ms.ob);
    }
    if (mask & volumeLogRatio) {
        e.volumeLogRatio = FC::calculateVolumeLogRatio(ms.ob);
    }
    if (mask & volumeLogRatioXVolume) {
        e.volumeLogRatioXVolume = FC::calculateVolumeLogRatioXVolume(ms.ob);
    }

    if (mask & queueDiff) {
        e.queueDiff = FC::calculateQueueDiff(ms.ob);
    }
    if (mask & queueImbalance) {
        e.queueImbalance = FC::calculateQueueImbalance(ms.ob);
    }
    if (mask & queueLogRatio) {
        e.queueLogRatio = FC::calculateQueueLogRatio(ms.ob);
    }
    if (mask & queueLogRatioXVolume) {
        e.queueLogRatioXVolume = FC::calculateQueueLogRatioXVolume(ms.ob);
    }

    if (mask & gap) {
        e.gap = FC::calculateGap(ms.ob);
    }
    if (mask & isAggressorAsk) {
        e.isAggressorAsk = FC::calculateIsAggressorAsk(&ms.getLastTrade());
    }

    if (mask & vwapDeviation) {
        e.vwapDeviation = FC::calculateVwapDeviation(ms.ob);
    }
    if (mask & vwapLogRatio) {
        e.vwapLogRatio = FC::calculateVwapLogRatio(ms.ob);
    }

    if (mask & simplifiedSlopeDiff) {
        e.simplifiedSlopeDiff = FC::calculateSimplifiedSlopeDiff(ms.ob);
    }
    if (mask & simplifiedSlopeImbalance) {
        e.simplifiedSlopeImbalance = FC::calculateSimplifiedSlopeImbalance(ms.ob);
    }
    if (mask & simplifiedSlopeLogRatio) {
        e.simplifiedSlopeLogRatio = FC::calculateSimplifiedSlopeLogRatio(ms.ob);
    }
    if (mask & bgcSlopeDiff) {
        e.bgcSlopeDiff = FC::calculateBgcSlopeDiff(ms.ob);
    }
    if (mask & bgcSlopeImbalance) {
        e.bgcSlopeImbalance = FC::calculateBgcSlopeImbalance(ms.ob);
    }
    if (mask & bgcSlopeLogRatio) {
        e.bgcSlopeLogRatio = FC::calculateBgcSlopeLogRatio(ms.ob);
    }

    if (mask & sinTime24h) {
        e.sinTime24h = FC::calculateSinTime24h(ms.getLastTimestampOfReceive());
    }
    if (mask & longSinTime24h) {
        e.longSinTime24h = FC::calculateLongSinTime24h(ms.getLastTimestampOfReceive());
    }
    if (mask & cosTime24h) {
        e.cosTime24h = FC::calculateCosTime24h(ms.getLastTimestampOfReceive());
    }
    if (mask & isAsiaTime) {
        e.isAsiaTime = FC::calculateIsAsiaTime(ms.getLastTimestampOfReceive());
    }
    if (mask & isEuropeTime) {
        e.isEuropeTime = FC::calculateIsEuropeTime(ms.getLastTimestampOfReceive());
    }
    if (mask & isUSATime) {
        e.isUSATime = FC::calculateIsUSATime(ms.getLastTimestampOfReceive());
    }

    if (mask & depthCount1Seconds) {
        e.depthCount1Seconds = FC::calculateDepthCount(ms.depthRoll, 1);
    }
    if (mask & depthCount3Seconds) {
        e.depthCount3Seconds = FC::calculateDepthCount(ms.depthRoll, 3);
    }
    if (mask & depthCount5Seconds) {
        e.depthCount5Seconds = FC::calculateDepthCount(ms.depthRoll, 5);
    }
    if (mask & depthCount10Seconds) {
        e.depthCount10Seconds = FC::calculateDepthCount(ms.depthRoll, 10);
    }
    if (mask & depthCount15Seconds) {
        e.depthCount15Seconds = FC::calculateDepthCount(ms.depthRoll, 15);
    }
    if (mask & depthCount30Seconds) {
        e.depthCount30Seconds = FC::calculateDepthCount(ms.depthRoll, 30);
    }
    if (mask & depthCount60Seconds) {
        e.depthCount60Seconds = FC::calculateDepthCount(ms.depthRoll, 60);
    }

    if (mask & depthCountDiff1Seconds) {
        e.depthCountDiff1Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 1);
    }
    if (mask & depthCountDiff3Seconds) {
        e.depthCountDiff3Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 3);
    }
    if (mask & depthCountDiff5Seconds) {
        e.depthCountDiff5Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 5);
    }
    if (mask & depthCountDiff10Seconds) {
        e.depthCountDiff10Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 10);
    }
    if (mask & depthCountDiff15Seconds) {
        e.depthCountDiff15Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 15);
    }
    if (mask & depthCountDiff30Seconds) {
        e.depthCountDiff30Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 30);
    }
    if (mask & depthCountDiff60Seconds) {
        e.depthCountDiff60Seconds = FC::calculateDepthCountDiff(ms.depthRoll, 60);
    }

    if (mask & depthCountImbalance1Seconds) {
        e.depthCountImbalance1Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 1);
    }
    if (mask & depthCountImbalance3Seconds) {
        e.depthCountImbalance3Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 3);
    }
    if (mask & depthCountImbalance5Seconds) {
        e.depthCountImbalance5Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 5);
    }
    if (mask & depthCountImbalance10Seconds) {
        e.depthCountImbalance10Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 10);
    }
    if (mask & depthCountImbalance15Seconds) {
        e.depthCountImbalance15Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 15);
    }
    if (mask & depthCountImbalance30Seconds) {
        e.depthCountImbalance30Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 30);
    }
    if (mask & depthCountImbalance60Seconds) {
        e.depthCountImbalance60Seconds = FC::calculateDepthCountImbalance(ms.depthRoll, 60);
    }

    if (mask & depthCountFisherImbalance1Seconds) {
        e.depthCountFisherImbalance1Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 1);
    }
    if (mask & depthCountFisherImbalance3Seconds) {
        e.depthCountFisherImbalance3Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 3);
    }
    if (mask & depthCountFisherImbalance5Seconds) {
        e.depthCountFisherImbalance5Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 5);
    }
    if (mask & depthCountFisherImbalance10Seconds) {
        e.depthCountFisherImbalance10Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 10);
    }
    if (mask & depthCountFisherImbalance15Seconds) {
        e.depthCountFisherImbalance15Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 15);
    }
    if (mask & depthCountFisherImbalance30Seconds) {
        e.depthCountFisherImbalance30Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 30);
    }
    if (mask & depthCountFisherImbalance60Seconds) {
        e.depthCountFisherImbalance60Seconds = FC::calculateDepthCountFisherImbalance(ms.depthRoll, 60);
    }

    if (mask & depthCountLogRatio1Seconds) {
        e.depthCountLogRatio1Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 1);
    }
    if (mask & depthCountLogRatio3Seconds) {
        e.depthCountLogRatio3Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 3);
    }
    if (mask & depthCountLogRatio5Seconds) {
        e.depthCountLogRatio5Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 5);
    }
    if (mask & depthCountLogRatio10Seconds) {
        e.depthCountLogRatio10Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 10);
    }
    if (mask & depthCountLogRatio15Seconds) {
        e.depthCountLogRatio15Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 15);
    }
    if (mask & depthCountLogRatio30Seconds) {
        e.depthCountLogRatio30Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 30);
    }
    if (mask & depthCountLogRatio60Seconds) {
        e.depthCountLogRatio60Seconds = FC::calculateDepthCountLogRatio(ms.depthRoll, 60);
    }

    if (mask & depthCountLogRatioXEventCount1Seconds) {
        e.depthCountLogRatioXEventCount1Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 1);
    }
    if (mask & depthCountLogRatioXEventCount3Seconds) {
        e.depthCountLogRatioXEventCount3Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 3);
    }
    if (mask & depthCountLogRatioXEventCount5Seconds) {
        e.depthCountLogRatioXEventCount5Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 5);
    }
    if (mask & depthCountLogRatioXEventCount10Seconds) {
        e.depthCountLogRatioXEventCount10Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 10);
    }
    if (mask & depthCountLogRatioXEventCount15Seconds) {
        e.depthCountLogRatioXEventCount15Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 15);
    }
    if (mask & depthCountLogRatioXEventCount30Seconds) {
        e.depthCountLogRatioXEventCount30Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 30);
    }
    if (mask & depthCountLogRatioXEventCount60Seconds) {
        e.depthCountLogRatioXEventCount60Seconds = FC::calculateDepthCountLogRatioXEventCount(ms.depthRoll, 60);
    }

    if (mask & tradeCount1Seconds) {
        e.tradeCount1Seconds = FC::calculateTradeCount(ms.tradeRoll, 1);
    }
    if (mask & tradeCount3Seconds) {
        e.tradeCount3Seconds = FC::calculateTradeCount(ms.tradeRoll, 3);
    }
    if (mask & tradeCount5Seconds) {
        e.tradeCount5Seconds = FC::calculateTradeCount(ms.tradeRoll, 5);
    }
    if (mask & tradeCount10Seconds) {
        e.tradeCount10Seconds = FC::calculateTradeCount(ms.tradeRoll, 10);
    }
    if (mask & tradeCount15Seconds) {
        e.tradeCount15Seconds = FC::calculateTradeCount(ms.tradeRoll, 15);
    }
    if (mask & tradeCount30Seconds) {
        e.tradeCount30Seconds = FC::calculateTradeCount(ms.tradeRoll, 30);
    }
    if (mask & tradeCount60Seconds) {
        e.tradeCount60Seconds = FC::calculateTradeCount(ms.tradeRoll, 60);
    }

    if (mask & tradeCountDiff1Seconds) {
        e.tradeCountDiff1Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 1);
    }
    if (mask & tradeCountDiff3Seconds) {
        e.tradeCountDiff3Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 3);
    }
    if (mask & tradeCountDiff5Seconds) {
        e.tradeCountDiff5Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 5);
    }
    if (mask & tradeCountDiff10Seconds) {
        e.tradeCountDiff10Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 10);
    }
    if (mask & tradeCountDiff15Seconds) {
        e.tradeCountDiff15Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 15);
    }
    if (mask & tradeCountDiff30Seconds) {
        e.tradeCountDiff30Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 30);
    }
    if (mask & tradeCountDiff60Seconds) {
        e.tradeCountDiff60Seconds = FC::calculateTradeCountDiff(ms.tradeRoll, 60);
    }

    if (mask & tradeCountImbalance1Seconds) {
        e.tradeCountImbalance1Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 1);
    }
    if (mask & tradeCountImbalance3Seconds) {
        e.tradeCountImbalance3Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 3);
    }
    if (mask & tradeCountImbalance5Seconds) {
        e.tradeCountImbalance5Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 5);
    }
    if (mask & tradeCountImbalance10Seconds) {
        e.tradeCountImbalance10Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 10);
    }
    if (mask & tradeCountImbalance15Seconds) {
        e.tradeCountImbalance15Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 15);
    }
    if (mask & tradeCountImbalance30Seconds) {
        e.tradeCountImbalance30Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 30);
    }
    if (mask & tradeCountImbalance60Seconds) {
        e.tradeCountImbalance60Seconds = FC::calculateTradeCountImbalance(ms.tradeRoll, 60);
    }

    if (mask & tradeCountFisherImbalance1Seconds) {
        e.tradeCountFisherImbalance1Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 1);
    }
    if (mask & tradeCountFisherImbalance3Seconds) {
        e.tradeCountFisherImbalance3Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 3);
    }
    if (mask & tradeCountFisherImbalance5Seconds) {
        e.tradeCountFisherImbalance5Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 5);
    }
    if (mask & tradeCountFisherImbalance10Seconds) {
        e.tradeCountFisherImbalance10Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 10);
    }
    if (mask & tradeCountFisherImbalance15Seconds) {
        e.tradeCountFisherImbalance15Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 15);
    }
    if (mask & tradeCountFisherImbalance30Seconds) {
        e.tradeCountFisherImbalance30Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 30);
    }
    if (mask & tradeCountFisherImbalance60Seconds) {
        e.tradeCountFisherImbalance60Seconds = FC::calculateTradeCountFisherImbalance(ms.tradeRoll, 60);
    }

    if (mask & tradeCountLogRatio1Seconds) {
        e.tradeCountLogRatio1Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 1);
    }
    if (mask & tradeCountLogRatio3Seconds) {
        e.tradeCountLogRatio3Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 3);
    }
    if (mask & tradeCountLogRatio5Seconds) {
        e.tradeCountLogRatio5Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 5);
    }
    if (mask & tradeCountLogRatio10Seconds) {
        e.tradeCountLogRatio10Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 10);
    }
    if (mask & tradeCountLogRatio15Seconds) {
        e.tradeCountLogRatio15Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 15);
    }
    if (mask & tradeCountLogRatio30Seconds) {
        e.tradeCountLogRatio30Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 30);
    }
    if (mask & tradeCountLogRatio60Seconds) {
        e.tradeCountLogRatio60Seconds = FC::calculateTradeCountLogRatio(ms.tradeRoll, 60);
    }

    if (mask & tradeVolumeSum1Seconds) {
        e.tradeVolumeSum1Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 1);
    }
    if (mask & tradeVolumeSum3Seconds) {
        e.tradeVolumeSum3Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 3);
    }
    if (mask & tradeVolumeSum5Seconds) {
        e.tradeVolumeSum5Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 5);
    }
    if (mask & tradeVolumeSum10Seconds) {
        e.tradeVolumeSum10Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 10);
    }
    if (mask & tradeVolumeSum15Seconds) {
        e.tradeVolumeSum15Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 15);
    }
    if (mask & tradeVolumeSum30Seconds) {
        e.tradeVolumeSum30Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 30);
    }
    if (mask & tradeVolumeSum60Seconds) {
        e.tradeVolumeSum60Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 60);
    }
    if (mask & tradeVolumeSum600Seconds) {
        e.tradeVolumeSum600Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 600);
    }
    if (mask & tradeVolumeSum1200Seconds) {
        e.tradeVolumeSum1200Seconds = FC::calculateTradeVolumeSum(ms.tradeRoll, 1200);
    }

    if (mask & tradeVolumeDiff1Seconds) {
        e.tradeVolumeDiff1Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 1);
    }
    if (mask & tradeVolumeDiff3Seconds) {
        e.tradeVolumeDiff3Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 3);
    }
    if (mask & tradeVolumeDiff5Seconds) {
        e.tradeVolumeDiff5Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 5);
    }
    if (mask & tradeVolumeDiff10Seconds) {
        e.tradeVolumeDiff10Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 10);
    }
    if (mask & tradeVolumeDiff15Seconds) {
        e.tradeVolumeDiff15Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 15);
    }
    if (mask & tradeVolumeDiff30Seconds) {
        e.tradeVolumeDiff30Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 30);
    }
    if (mask & tradeVolumeDiff60Seconds) {
        e.tradeVolumeDiff60Seconds = FC::calculateTradeVolumeDiff(ms.tradeRoll, 60);
    }

    if (mask & tradeVolumeImbalance1Seconds) {
        e.tradeVolumeImbalance1Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 1);
    }
    if (mask & tradeVolumeImbalance3Seconds) {
        e.tradeVolumeImbalance3Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 3);
    }
    if (mask & tradeVolumeImbalance5Seconds) {
        e.tradeVolumeImbalance5Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 5);
    }
    if (mask & tradeVolumeImbalance10Seconds) {
        e.tradeVolumeImbalance10Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 10);
    }
    if (mask & tradeVolumeImbalance15Seconds) {
        e.tradeVolumeImbalance15Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 15);
    }
    if (mask & tradeVolumeImbalance30Seconds) {
        e.tradeVolumeImbalance30Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 30);
    }
    if (mask & tradeVolumeImbalance60Seconds) {
        e.tradeVolumeImbalance60Seconds = FC::calculateTradeVolumeImbalance(ms.tradeRoll, 60);
    }

    if (mask & tradeVolumeLogRatio1Seconds) {
        e.tradeVolumeLogRatio1Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 1);
    }
    if (mask & tradeVolumeLogRatio3Seconds) {
        e.tradeVolumeLogRatio3Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 3);
    }
    if (mask & tradeVolumeLogRatio5Seconds) {
        e.tradeVolumeLogRatio5Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 5);
    }
    if (mask & tradeVolumeLogRatio10Seconds) {
        e.tradeVolumeLogRatio10Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 10);
    }
    if (mask & tradeVolumeLogRatio15Seconds) {
        e.tradeVolumeLogRatio15Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 15);
    }
    if (mask & tradeVolumeLogRatio30Seconds) {
        e.tradeVolumeLogRatio30Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 30);
    }
    if (mask & tradeVolumeLogRatio60Seconds) {
        e.tradeVolumeLogRatio60Seconds = FC::calculateTradeVolumeLogRatio(ms.tradeRoll, 60);
    }

    if (mask & biggestSingleBuyTradeVolume1Seconds) {
        e.biggestSingleBuyTradeVolume1Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 1);
    }
    if (mask & biggestSingleBuyTradeVolume3Seconds) {
        e.biggestSingleBuyTradeVolume3Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 3);
    }
    if (mask & biggestSingleBuyTradeVolume5Seconds) {
        e.biggestSingleBuyTradeVolume5Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 5);
    }
    if (mask & biggestSingleBuyTradeVolume10Seconds) {
        e.biggestSingleBuyTradeVolume10Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 10);
    }
    if (mask & biggestSingleBuyTradeVolume15Seconds) {
        e.biggestSingleBuyTradeVolume15Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 15);
    }
    if (mask & biggestSingleBuyTradeVolume30Seconds) {
        e.biggestSingleBuyTradeVolume30Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 30);
    }
    if (mask & biggestSingleBuyTradeVolume60Seconds) {
        e.biggestSingleBuyTradeVolume60Seconds = FC::calculateBiggestSingleBuyTradeVolume(ms.tradeRoll, 60);
    }

    if (mask & biggestSingleSellTradeVolume1Seconds) {
        e.biggestSingleSellTradeVolume1Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 1);
    }
    if (mask & biggestSingleSellTradeVolume3Seconds) {
        e.biggestSingleSellTradeVolume3Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 3);
    }
    if (mask & biggestSingleSellTradeVolume5Seconds) {
        e.biggestSingleSellTradeVolume5Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 5);
    }
    if (mask & biggestSingleSellTradeVolume10Seconds) {
        e.biggestSingleSellTradeVolume10Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 10);
    }
    if (mask & biggestSingleSellTradeVolume15Seconds) {
        e.biggestSingleSellTradeVolume15Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 15);
    }
    if (mask & biggestSingleSellTradeVolume30Seconds) {
        e.biggestSingleSellTradeVolume30Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 30);
    }
    if (mask & biggestSingleSellTradeVolume60Seconds) {
        e.biggestSingleSellTradeVolume60Seconds = FC::calculateBiggestSingleSellTradeVolume(ms.tradeRoll, 60);
    }

    if (mask & avgTradeSizeDiff1Seconds) {
        e.avgTradeSizeDiff1Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 1);
    }
    if (mask & avgTradeSizeDiff3Seconds) {
        e.avgTradeSizeDiff3Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 3);
    }
    if (mask & avgTradeSizeDiff5Seconds) {
        e.avgTradeSizeDiff5Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 5);
    }
    if (mask & avgTradeSizeDiff10Seconds) {
        e.avgTradeSizeDiff10Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 10);
    }
    if (mask & avgTradeSizeDiff15Seconds) {
        e.avgTradeSizeDiff15Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 15);
    }
    if (mask & avgTradeSizeDiff30Seconds) {
        e.avgTradeSizeDiff30Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 30);
    }
    if (mask & avgTradeSizeDiff60Seconds) {
        e.avgTradeSizeDiff60Seconds = FC::calculateAvgTradeSizeDiff(ms.tradeRoll, 60);
    }

    if (mask & avgTradeSizeImbalance1Seconds) {
        e.avgTradeSizeImbalance1Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 1);
    }
    if (mask & avgTradeSizeImbalance3Seconds) {
        e.avgTradeSizeImbalance3Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 3);
    }
    if (mask & avgTradeSizeImbalance5Seconds) {
        e.avgTradeSizeImbalance5Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 5);
    }
    if (mask & avgTradeSizeImbalance10Seconds) {
        e.avgTradeSizeImbalance10Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 10);
    }
    if (mask & avgTradeSizeImbalance15Seconds) {
        e.avgTradeSizeImbalance15Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 15);
    }
    if (mask & avgTradeSizeImbalance30Seconds) {
        e.avgTradeSizeImbalance30Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 30);
    }
    if (mask & avgTradeSizeImbalance60Seconds) {
        e.avgTradeSizeImbalance60Seconds = FC::calculateAvgTradeSizeImbalance(ms.tradeRoll, 60);
    }

    if (mask & avgTradeSizeLogRatio1Seconds) {
        e.avgTradeSizeLogRatio1Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 1);
    }
    if (mask & avgTradeSizeLogRatio3Seconds) {
        e.avgTradeSizeLogRatio3Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 3);
    }
    if (mask & avgTradeSizeLogRatio5Seconds) {
        e.avgTradeSizeLogRatio5Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 5);
    }
    if (mask & avgTradeSizeLogRatio10Seconds) {
        e.avgTradeSizeLogRatio10Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 10);
    }
    if (mask & avgTradeSizeLogRatio15Seconds) {
        e.avgTradeSizeLogRatio15Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 15);
    }
    if (mask & avgTradeSizeLogRatio30Seconds) {
        e.avgTradeSizeLogRatio30Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 30);
    }
    if (mask & avgTradeSizeLogRatio60Seconds) {
        e.avgTradeSizeLogRatio60Seconds = FC::calculateAvgTradeSizeLogRatio(ms.tradeRoll, 60);
    }

    if (mask & priceDifference1Seconds) {
        e.priceDifference1Seconds = FC::calculatePriceDifference(ms.tradeRoll, 1);
    }
    if (mask & priceDifference3Seconds) {
        e.priceDifference3Seconds = FC::calculatePriceDifference(ms.tradeRoll, 3);
    }
    if (mask & priceDifference5Seconds) {
        e.priceDifference5Seconds = FC::calculatePriceDifference(ms.tradeRoll, 5);
    }
    if (mask & priceDifference10Seconds) {
        e.priceDifference10Seconds = FC::calculatePriceDifference(ms.tradeRoll, 10);
    }
    if (mask & priceDifference15Seconds) {
        e.priceDifference15Seconds = FC::calculatePriceDifference(ms.tradeRoll, 15);
    }
    if (mask & priceDifference30Seconds) {
        e.priceDifference30Seconds = FC::calculatePriceDifference(ms.tradeRoll, 30);
    }
    if (mask & priceDifference60Seconds) {
        e.priceDifference60Seconds = FC::calculatePriceDifference(ms.tradeRoll, 60);
    }
    if (mask & priceDifference600Seconds) {
        e.priceDifference600Seconds = FC::calculatePriceDifference(ms.tradeRoll, 600);
    }
    if (mask & priceDifference1200Seconds) {
        e.priceDifference1200Seconds = FC::calculatePriceDifference(ms.tradeRoll, 1200);
    }

    if (mask & rateOfReturn1Seconds) {
        e.rateOfReturn1Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 1);
    }
    if (mask & rateOfReturn3Seconds) {
        e.rateOfReturn3Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 3);
    }
    if (mask & rateOfReturn5Seconds) {
        e.rateOfReturn5Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 5);
    }
    if (mask & rateOfReturn10Seconds) {
        e.rateOfReturn10Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 10);
    }
    if (mask & rateOfReturn15Seconds) {
        e.rateOfReturn15Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 15);
    }
    if (mask & rateOfReturn30Seconds) {
        e.rateOfReturn30Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 30);
    }
    if (mask & rateOfReturn60Seconds) {
        e.rateOfReturn60Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 60);
    }
    if (mask & rateOfReturn600Seconds) {
        e.rateOfReturn600Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 600);
    }
    if (mask & rateOfReturn1200Seconds) {
        e.rateOfReturn1200Seconds = FC::calculateRateOfReturn(ms.tradeRoll, 1200);
    }

    if (mask & logReturnRatio1Seconds) {
        e.logReturnRatio1Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 1);
    }
    if (mask & logReturnRatio3Seconds) {
        e.logReturnRatio3Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 3);
    }
    if (mask & logReturnRatio5Seconds) {
        e.logReturnRatio5Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 5);
    }
    if (mask & logReturnRatio10Seconds) {
        e.logReturnRatio10Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 10);
    }
    if (mask & logReturnRatio15Seconds) {
        e.logReturnRatio15Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 15);
    }
    if (mask & logReturnRatio30Seconds) {
        e.logReturnRatio30Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 30);
    }
    if (mask & logReturnRatio60Seconds) {
        e.logReturnRatio60Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 60);
    }
    if (mask & logReturnRatio600Seconds) {
        e.logReturnRatio600Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 600);
    }
    if (mask & logReturnRatio1200Seconds) {
        e.logReturnRatio1200Seconds = FC::calculateLogReturnRatio(ms.tradeRoll, 1200);
    }

    if (mask & smaLogRatio3Seconds) {
        e.smaLogRatio3Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 3);
    }
    if (mask & smaLogRatio5Seconds) {
        e.smaLogRatio5Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 5);
    }
    if (mask & smaLogRatio10Seconds) {
        e.smaLogRatio10Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 10);
    }
    if (mask & smaLogRatio15Seconds) {
        e.smaLogRatio15Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 15);
    }
    if (mask & smaLogRatio30Seconds) {
        e.smaLogRatio30Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 30);
    }
    if (mask & smaLogRatio60Seconds) {
        e.smaLogRatio60Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 60);
    }
    if (mask & smaLogRatio300Seconds) {
        e.smaLogRatio300Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 300);
    }
    if (mask & smaLogRatio600Seconds) {
        e.smaLogRatio600Seconds = FC::calculateSMALogRatio(ms.tradeRoll, 600);
    }

    if (mask & hlc3tLogRatio300Seconds) {
        e.hlc3tLogRatio300Seconds = FC::calculateHLC3TLogRatio(ms.tradeRoll, 300);
    }
    if (mask & hlc3tLogRatio600Seconds) {
        e.hlc3tLogRatio600Seconds = FC::calculateHLC3TLogRatio(ms.tradeRoll, 600);
    }
    if (mask & hlc3tLogRatio1200Seconds) {
        e.hlc3tLogRatio1200Seconds = FC::calculateHLC3TLogRatio(ms.tradeRoll, 1200);
    }

    if (mask & bollingerBandsDistance300Seconds) {
        e.bollingerBandsDistance300Seconds = FC::calculateBollingerBandsDistance(ms.tradeRoll, 300);
    }
    if (mask & bollingerBandsDistance600Seconds) {
        e.bollingerBandsDistance600Seconds = FC::calculateBollingerBandsDistance(ms.tradeRoll, 600);
    }
    if (mask & bollingerBandsDistance1200Seconds) {
        e.bollingerBandsDistance1200Seconds = FC::calculateBollingerBandsDistance(ms.tradeRoll, 1200);
    }

    if (mask & rfwmaLogRatio5Seconds1i) {
        e.rfwmaLogRatio5Seconds1i = FC::calculateRFWMALogRatio(ms.tradeRoll, 5, 1);
    }
    if (mask & rfwmaLogRatio30Seconds1i) {
        e.rfwmaLogRatio30Seconds1i = FC::calculateRFWMALogRatio(ms.tradeRoll, 30, 1);
    }
    if (mask & rfwmaLogRatio60Seconds1i) {
        e.rfwmaLogRatio60Seconds1i = FC::calculateRFWMALogRatio(ms.tradeRoll, 60, 1);
    }
    if (mask & rfwmaLogRatio120Seconds2i) {
        e.rfwmaLogRatio120Seconds2i = FC::calculateRFWMALogRatio(ms.tradeRoll, 120, 2);
    }
    if (mask & rfwmaLogRatio120Seconds10i) {
        e.rfwmaLogRatio120Seconds10i = FC::calculateRFWMALogRatio(ms.tradeRoll, 120, 10);
    }
    if (mask & rfwmaLogRatio300Seconds5i) {
        e.rfwmaLogRatio300Seconds5i = FC::calculateRFWMALogRatio(ms.tradeRoll, 300, 5);
    }
    if (mask & rfwmaLogRatio600Seconds10i) {
        e.rfwmaLogRatio600Seconds10i = FC::calculateRFWMALogRatio(ms.tradeRoll, 600, 10);
    }

    if (mask & fwmaLogRatio5Seconds1i) {
        e.fwmaLogRatio5Seconds1i = FC::calculateFWMALogRatio(ms.tradeRoll, 5, 1);
    }
    if (mask & fwmaLogRatio30Seconds1i) {
        e.fwmaLogRatio30Seconds1i = FC::calculateFWMALogRatio(ms.tradeRoll, 30, 1);
    }
    if (mask & fwmaLogRatio60Seconds1i) {
        e.fwmaLogRatio60Seconds1i = FC::calculateFWMALogRatio(ms.tradeRoll, 60, 1);
    }
    if (mask & fwmaLogRatio120Seconds2i) {
        e.fwmaLogRatio120Seconds2i = FC::calculateFWMALogRatio(ms.tradeRoll, 120, 2);
    }
    if (mask & fwmaLogRatio120Seconds10i) {
        e.fwmaLogRatio120Seconds10i = FC::calculateFWMALogRatio(ms.tradeRoll, 120, 10);
    }
    if (mask & fwmaLogRatio300Seconds5i) {
        e.fwmaLogRatio300Seconds5i = FC::calculateFWMALogRatio(ms.tradeRoll, 300, 5);
    }
    if (mask & fwmaLogRatio600Seconds10i) {
        e.fwmaLogRatio600Seconds10i = FC::calculateFWMALogRatio(ms.tradeRoll, 600, 10);
    }

    if (mask & trendTVal5Seconds || mask & trendCoef5Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 0);
        if (mask & trendTVal5Seconds) {e.trendTVal5Seconds = tVal;}
        if (mask & trendCoef5Seconds) {e.trendCoef5Seconds = coef;}
    }
    if (mask & trendTVal8Seconds || mask & trendCoef8Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 1);
        if (mask & trendTVal8Seconds) {e.trendTVal8Seconds = tVal;}
        if (mask & trendCoef8Seconds) {e.trendCoef8Seconds = coef;}
    }
    if (mask & trendTVal13Seconds || mask & trendCoef13Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 2);
        if (mask & trendTVal13Seconds) {e.trendTVal13Seconds = tVal;}
        if (mask & trendCoef13Seconds) {e.trendCoef13Seconds = coef;}
    }
    if (mask & trendTVal21Seconds || mask & trendCoef21Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 3);
        if (mask & trendTVal21Seconds) {e.trendTVal21Seconds = tVal;}
        if (mask & trendCoef21Seconds) {e.trendCoef21Seconds = coef;}
    }
    if (mask & trendTVal89Seconds || mask & trendCoef89Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 6);
        if (mask & trendTVal89Seconds) {e.trendTVal89Seconds = tVal;}
        if (mask & trendCoef89Seconds) {e.trendCoef89Seconds = coef;}
    }
    if (mask & trendTVal144Seconds || mask & trendCoef144Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 7);
        if (mask & trendTVal144Seconds) {e.trendTVal144Seconds = tVal;}
        if (mask & trendCoef144Seconds) {e.trendCoef144Seconds = coef;}
    }
    if (mask & trendTVal987Seconds || mask & trendCoef987Seconds) {
        const auto [tVal, coef] = FC::calculateTrendCoefAndTVal(ms.tradeRoll, 11);
        if (mask & trendTVal987Seconds) {e.trendTVal987Seconds = tVal;}
        if (mask & trendCoef987Seconds) {e.trendCoef987Seconds = coef;}
    }

    if (mask & cValBestTVal || mask & cValBestCoef || mask & cValBestTrendLength) {
        const auto [bestTVal, bestCoef, bestTrendLen] = FC::calculateCVal(ms.tradeRoll);
        if (mask & cValBestTVal) e.cValBestTVal = bestTVal;
        if (mask & cValBestCoef) e.cValBestCoef = bestCoef;
        if (mask & cValBestTrendLength) e.cValBestTrendLength = bestTrendLen;
    }

    if (mask & priceDistance1Seconds) {
        e.priceDistance1Seconds = FC::calculatePriceDistance(ms.tradeRoll, 1);
    }
    if (mask & priceDistance3Seconds) {
        e.priceDistance3Seconds = FC::calculatePriceDistance(ms.tradeRoll, 3);
    }
    if (mask & priceDistance5Seconds) {
        e.priceDistance5Seconds = FC::calculatePriceDistance(ms.tradeRoll, 5);
    }
    if (mask & priceDistance10Seconds) {
        e.priceDistance10Seconds = FC::calculatePriceDistance(ms.tradeRoll, 10);
    }
    if (mask & priceDistance15Seconds) {
        e.priceDistance15Seconds = FC::calculatePriceDistance(ms.tradeRoll, 15);
    }
    if (mask & priceDistance30Seconds) {
        e.priceDistance30Seconds = FC::calculatePriceDistance(ms.tradeRoll, 30);
    }
    if (mask & priceDistance60Seconds) {
        e.priceDistance60Seconds = FC::calculatePriceDistance(ms.tradeRoll, 60);
    }
    if (mask & priceDistance300Seconds) {
        e.priceDistance300Seconds = FC::calculatePriceDistance(ms.tradeRoll, 300);
    }
    if (mask & priceDistance600Seconds) {
        e.priceDistance600Seconds = FC::calculatePriceDistance(ms.tradeRoll, 600);
    }

    if (mask & logKylesLambda1Seconds) {
        e.logKylesLambda1Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 1);
    }
    if (mask & logKylesLambda3Seconds) {
        e.logKylesLambda3Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 3);
    }
    if (mask & logKylesLambda5Seconds) {
        e.logKylesLambda5Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 5);
    }
    if (mask & logKylesLambda10Seconds) {
        e.logKylesLambda10Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 10);
    }
    if (mask & logKylesLambda15Seconds) {
        e.logKylesLambda15Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 15);
    }
    if (mask & logKylesLambda30Seconds) {
        e.logKylesLambda30Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 30);
    }
    if (mask & logKylesLambda60Seconds) {
        e.logKylesLambda60Seconds = FC::calculateLogKylesLambda(ms.tradeRoll, 60);
    }

    if (mask & deltaPVT1Seconds) {
        e.deltaPVT1Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 1);
    }
    if (mask & deltaPVT3Seconds) {
        e.deltaPVT3Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 3);
    }
    if (mask & deltaPVT5Seconds) {
        e.deltaPVT5Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 5);
    }
    if (mask & deltaPVT10Seconds) {
        e.deltaPVT10Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 10);
    }
    if (mask & deltaPVT15Seconds) {
        e.deltaPVT15Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 15);
    }
    if (mask & deltaPVT30Seconds) {
        e.deltaPVT30Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 30);
    }
    if (mask & deltaPVT60Seconds) {
        e.deltaPVT60Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 60);
    }
    if (mask & deltaPVT600Seconds) {
        e.deltaPVT600Seconds = FC::calculateDeltaPVT(ms.tradeRoll, 600);
    }

    return e;
}
