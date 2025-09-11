#pragma once

#include "RollingDifferenceDepthStatistics.h"
#include "RollingTradeStatistics.h"
#include "enums/TradeEntry.h"
#include "OrderBook.h"

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderBook);
    double calculateBestBidPrice(const OrderBook& orderBook);
    double calculateMidPrice(const OrderBook& orderBook);

    double calculateMicroPriceDiff(const OrderBook& orderBook);
    double calculateMicroPriceImbalance(const OrderBook& orderBook);
    double calculateMicroPriceFisherImbalance(const OrderBook& orderBook);
    double calculateMicroPriceDeviation(const OrderBook& orderBook);
    double calculateMicroPriceLogRatio(const OrderBook& orderBook);

    double calculateBestBidQuantity(const OrderBook& orderBook);
    double calculateBestAskQuantity(const OrderBook& orderBook);

    double calculateBestOrderFlowDiff(const OrderBook& orderBook);
    double calculateBestOrderFlowImbalance(const OrderBook& orderBook);
    double calculateBestOrderFlowFisherImbalance(const OrderBook& orderBook);

    double calculateBestOrderFlowCKSDiff(const OrderBook& orderBook);
    double calculateBestOrderFlowCKSImbalance(const OrderBook& orderBook);
    double calculateBestOrderFlowCKSFisherImbalance(const OrderBook& orderBook);

    double calculateOrderFlowDiff(const OrderBook& orderBook);
    double calculateOrderFlowImbalance(const OrderBook& orderBook);
    double calculateOrderFlowFisherImbalance(const OrderBook& orderBook);

    double calculateQueueCountFlowDelta(const OrderBook& orderBook);
    double calculateQueueCountFlowImbalance(const OrderBook& orderBook);
    double calculateQueueCountFlowFisherImbalance(const OrderBook& orderBook);

    double calculateBestVolumeDiff(const OrderBook& orderBook);
    double calculateBestVolumeImbalance(const OrderBook& orderBook);
    double calculateBestVolumeFisherImbalance(const OrderBook& orderBook);
    double calculateBestVolumeLogRatio(const OrderBook& orderBook);
    double calculateBestVolumeSignedLogRatioXVolume(const OrderBook& orderBook);
    double calculateBestNPriceLevelsVolumeDiff(const OrderBook& orderBook, int nPriceLevels);
    double calculateBestNPriceLevelsVolumeImbalance(const OrderBook& orderBook, int nPriceLevels);
    double calculateBestNPriceLevelsVolumeLogRatio(const OrderBook& orderBook, int nPriceLevels);
    double calculateBestNPriceLevelsVolumeLogRatioXVolume(const OrderBook& orderBook, int nPriceLevels);

    double calculateVolumeDiff(const OrderBook& orderBook);
    double calculateVolumeImbalance(const OrderBook& orderBook);
    double calculateVolumeLogRatio(const OrderBook& orderBook);
    double calculateVolumeLogRatioXVolume(const OrderBook& orderBook);

    double calculateQueueDiff(const OrderBook& orderBook);
    double calculateQueueImbalance(const OrderBook& orderBook);
    double calculateQueueLogRatio(const OrderBook& orderBook);
    double calculateQueueLogRatioXVolume(const OrderBook& orderBook);

    double calculateGap(const OrderBook& orderBook);
    bool calculateIsAggressorAsk(const TradeEntry *tradeEntry);
    double calculateVwapDeviation(const OrderBook& orderBook);
    double calculateVwapLogRatio(const OrderBook& orderBook);
    double calculateSimplifiedSlopeImbalance(const OrderBook& orderBook);
    double calculateSimplifiedSlopeDiff(const OrderBook& orderBook);
    double calculateSimplifiedSlopeLogRatio(const OrderBook& orderBook);
    double calculateBgcSlopeImbalance(const OrderBook& orderBook);
    double calculateBgcSlopeDiff(const OrderBook& orderBook);
    double calculateBgcSlopeLogRatio(const OrderBook& orderBook);

    double calculateDifferenceDepthCount(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);
    double calculateTradeCount(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculateDifferenceDepthCountDiff(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);
    double calculateDifferenceDepthCountImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);
    double calculateDifferenceDepthCountFisherImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);
    double calculateDifferenceDepthCountLogRatio(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);
    double calculateDifferenceDepthCountLogRatioXEventCount(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);

    double calculateTradeCountDiff(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateTradeCountImbalance(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateTradeCountFisherImbalance(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateTradeCountLogRatio(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculateTradeVolumeDiff(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateTradeVolumeImbalance(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateTradeVolumeLogRatio(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculateAvgTradeSizeDiff(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateAvgTradeSizeImbalance(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateAvgTradeSizeLogRatio(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculateBiggestSingleBuyTradeVolume(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateBiggestSingleSellTradeVolume(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculatePriceDifference(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateRateOfReturn(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateLogReturnRatio(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculateLogKylesLambda(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

    double calculateRSI(const RollingTradeStatistics& rollingTradeStatistics, int startWindowTimeSeconds, int windowTimeSeconds);
    double calculateStochRSI(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateMacd(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

}
