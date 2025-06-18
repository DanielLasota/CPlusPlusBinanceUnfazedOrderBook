#pragma once

#include "enums/TradeEntry.h"
#include "OrderBook.h"
#include <RollingStatisticsData.h>

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderBook);
    double calculateBestBidPrice(const OrderBook& orderBook);
    double calculateMidPrice(const OrderBook& orderBook);

    double calculateBestVolumeImbalance(const OrderBook& orderBook);
    double calculateBestVolumeRatio(const OrderBook& orderBook);
    double calculateBestTwoVolumeImbalance(const OrderBook& orderBook);
    double calculateBestThreeVolumeImbalance(const OrderBook& orderBook);
    double calculateBestFiveVolumeImbalance(const OrderBook& orderBook);
    double calculateVolumeImbalance(const OrderBook& orderBook);
    double calculateQueueImbalance(const OrderBook& orderBook);
    double calculateGap(const OrderBook& orderBook);
    bool calculateIsAggressorAsk(const TradeEntry *tradeEntry);
    double calculateVwapDeviation(const OrderBook& orderBook);
    double calculateSimplifiedSlopeImbalance(const OrderBook& orderBook);

    double calculateTradeCountImbalance(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds);
    double calculateCumulativeDelta(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds);
    double calculatePriceDifference(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds);
    double calculateRateOfReturn(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds);
    double calculateDifferenceDepthVolatilityImbalance(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds);

}
