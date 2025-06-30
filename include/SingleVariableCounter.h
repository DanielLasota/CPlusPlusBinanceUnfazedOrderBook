#pragma once

#include "RollingDifferenceDepthStatistics.h"
#include "RollingTradeStatistics.h"
#include "enums/TradeEntry.h"
#include "OrderBook.h"

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderBook);
    double calculateBestBidPrice(const OrderBook& orderBook);
    double calculateMidPrice(const OrderBook& orderBook);

    double calculateBestVolumeImbalance(const OrderBook& orderBook);
    double calculateBestDepthVolumeRatio(const OrderBook& orderBook);
    double calculateBestNPriceLevelsVolumeImbalance(const OrderBook& orderBook, int nPriceLevels);
    double calculateVolumeImbalance(const OrderBook& orderBook);
    double calculateQueueImbalance(const OrderBook& orderBook);
    double calculateGap(const OrderBook& orderBook);
    bool calculateIsAggressorAsk(const TradeEntry *tradeEntry);
    double calculateVwapDeviation(const OrderBook& orderBook);
    double calculateSimplifiedSlopeImbalance(const OrderBook& orderBook);

    double calculateTradeCountImbalance(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateCumulativeDelta(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculatePriceDifference(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateRateOfReturn(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateDifferenceDepthVolatilityImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, int windowTimeSeconds);

    double calculateRSI(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateStochRSI(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);
    double calculateMacd(const RollingTradeStatistics& rollingTradeStatistics, int windowTimeSeconds);

}
