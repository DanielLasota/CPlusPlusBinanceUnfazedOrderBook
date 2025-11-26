#pragma once

#include "OrderBook.h"
#include "RollingDifferenceDepthStatistics.h"
#include "TradeRoll.h"
#include "enums/TradeEntry.h"

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook &ob);
    double calculateBestBidPrice(const OrderBook &ob);
    double calculateMidPrice(const OrderBook &ob);

    double calculateMicroPriceDiff(const OrderBook &ob);
    double calculateMicroPriceImbalance(const OrderBook &ob);
    double calculateMicroPriceFisherImbalance(const OrderBook &ob);
    double calculateMicroPriceDeviation(const OrderBook &ob);
    double calculateMicroPriceLogRatio(const OrderBook &ob);

    double calculateBestBidQuantity(const OrderBook &ob);
    double calculateBestAskQuantity(const OrderBook &ob);

    double calculateBestOrderFlowDiff(const OrderBook &ob);
    double calculateBestOrderFlowImbalance(const OrderBook &ob);
    double calculateBestOrderFlowFisherImbalance(const OrderBook &ob);

    double calculateBestOrderFlowCKSDiff(const OrderBook &ob);
    double calculateBestOrderFlowCKSImbalance(const OrderBook &ob);
    double calculateBestOrderFlowCKSFisherImbalance(const OrderBook &ob);

    double calculateOrderFlowDiff(const OrderBook &ob);
    double calculateOrderFlowImbalance(const OrderBook &ob);
    double calculateOrderFlowFisherImbalance(const OrderBook &ob);

    int calculateQueueCountFlowDiff(const OrderBook& ob);
    double calculateQueueCountFlowImbalance(const OrderBook &ob);
    double calculateQueueCountFlowFisherImbalance(const OrderBook &ob);

    double calculateBestVolumeDiff(const OrderBook &ob);
    double calculateBestVolumeImbalance(const OrderBook &ob);
    double calculateBestVolumeFisherImbalance(const OrderBook &ob);
    double calculateBestVolumeLogRatio(const OrderBook &ob);
    double calculateBestVolumeSignedLogRatioXVolume(const OrderBook &ob);
    double calculateBestNPriceLevelsVolumeDiff(const OrderBook &ob, int n);
    double calculateBestNPriceLevelsVolumeImbalance(const OrderBook &ob, int n);
    double calculateBestNPriceLevelsVolumeLogRatio(const OrderBook &ob, int n);
    double calculateBestNPriceLevelsVolumeLogRatioXVolume(const OrderBook &ob, int n);

    double calculateVolumeDiff(const OrderBook &ob);
    double calculateVolumeImbalance(const OrderBook &ob);
    double calculateVolumeLogRatio(const OrderBook &ob);
    double calculateVolumeLogRatioXVolume(const OrderBook &ob);

    int calculateQueueDiff(const OrderBook& ob);
    double calculateQueueImbalance(const OrderBook &ob);
    double calculateQueueLogRatio(const OrderBook &ob);
    double calculateQueueLogRatioXVolume(const OrderBook &ob);

    double calculateGap(const OrderBook &ob);
    bool calculateIsAggressorAsk(const TradeEntry *te);
    double calculateVwapDeviation(const OrderBook &ob);
    double calculateVwapLogRatio(const OrderBook &ob);
    double calculateSimplifiedSlopeImbalance(const OrderBook &ob);
    double calculateSimplifiedSlopeDiff(const OrderBook &ob);
    double calculateSimplifiedSlopeLogRatio(const OrderBook &ob);
    double calculateBgcSlopeImbalance(const OrderBook &ob);
    double calculateBgcSlopeDiff(const OrderBook &ob);
    double calculateBgcSlopeLogRatio(const OrderBook &ob);

    double calculateSinTime24h(uint64_t timestamp);
    double calculateLongSinTime24h(uint64_t timestamp);
    double calculateCosTime24h(uint64_t timestamp);
    double calculateIsAsiaTime(uint64_t timestamp);
    double calculateIsEuropeTime(uint64_t timestamp);
    double calculateIsUSATime(uint64_t timestamp);

    double calculateDifferenceDepthCount(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, size_t seconds);
    double calculateDifferenceDepthCountDiff(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, size_t seconds);
    double calculateDifferenceDepthCountImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, size_t seconds);
    double calculateDifferenceDepthCountFisherImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, size_t seconds);
    double calculateDifferenceDepthCountLogRatio(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, size_t seconds);
    double calculateDifferenceDepthCountLogRatioXEventCount(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, size_t seconds);

    int calculateTradeCount(const TradeRoll &tr, size_t seconds);
    int calculateTradeCountDiff(const TradeRoll &tr, size_t seconds);
    double calculateTradeCountImbalance(const TradeRoll &tr, size_t seconds);
    double calculateTradeCountFisherImbalance(const TradeRoll &tr, size_t seconds);
    double calculateTradeCountLogRatio(const TradeRoll &tr, size_t seconds);

    double calculateTradeVolumeSum(const TradeRoll &tr, size_t seconds);
    double calculateTradeVolumeDiff(const TradeRoll &tr, size_t seconds);
    double calculateTradeVolumeImbalance(const TradeRoll &tr, size_t seconds);
    double calculateTradeVolumeLogRatio(const TradeRoll &tr, size_t seconds);

    double calculateBiggestSingleBuyTradeVolume(const TradeRoll &tr, size_t seconds);
    double calculateBiggestSingleSellTradeVolume(const TradeRoll &tr, size_t seconds);

    double calculateAvgTradeSizeDiff(const TradeRoll &tr, size_t seconds);
    double calculateAvgTradeSizeImbalance(const TradeRoll &tr, size_t seconds);
    double calculateAvgTradeSizeLogRatio(const TradeRoll &tr, size_t seconds);

    double calculatePriceDifference(const TradeRoll &tr, size_t seconds);
    double calculateRateOfReturn(const TradeRoll &tr, size_t seconds);
    double calculateLogReturnRatio(const TradeRoll &tr, size_t seconds);

    double calculateSMALogRatio(const TradeRoll &tr, size_t seconds);
    double calculateHLC3TLogRatio(const TradeRoll &tr, size_t seconds);
    double calculateBollingerBandsDistance(const TradeRoll &tr, size_t seconds);

    double calculatePriceDistance(const TradeRoll &tr, size_t seconds);
    double calculateLogKylesLambda(const TradeRoll &tr, size_t seconds);
    double calculateDeltaPVT(const TradeRoll &tr, size_t seconds);
}
