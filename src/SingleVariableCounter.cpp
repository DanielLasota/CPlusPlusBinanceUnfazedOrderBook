#include <cmath>

#include "SingleVariableCounter.h"
#include <RollingStatisticsData.h>
#include "OrderBook.h"

inline double round2(const double x) {
    double y = x * 100.0;
    y += (y >= 0.0 ?  0.5 : -0.5);
    const int t = static_cast<int>(y);
    return t * 0.01;
}

inline double round5(const double x) {
    double y = x * 100'000.0;
    y += (y >= 0.0 ?  0.5 : -0.5);
    const int t = static_cast<int>(y);
    return t * 0.00001;
}

inline double round8(const double x) {
    constexpr double p = 100000000.0; // 8
    return std::round(x * p) / p;
}

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderBook) {
        return orderBook.bestAskPrice();
    }

    double calculateBestBidPrice(const OrderBook& orderBook) {
        return orderBook.bestBidPrice();
    }

    double calculateMidPrice(const OrderBook& orderBook) {
        return (orderBook.bestBidPrice() + orderBook.bestAskPrice()) * 0.5;
    }

    double calculateBestVolumeImbalance(const OrderBook& orderBook) {
        const double bestAskQuantity = orderBook.bestAskQuantity();
        const double bestBidQuantity = orderBook.bestBidQuantity();
        return round2(
            (bestBidQuantity - bestAskQuantity) / (bestBidQuantity + bestAskQuantity)
            );
    }

    double calculateBestVolumeRatio(const OrderBook& orderBook) {
        const double bestAskQuantity = orderBook.bestAskQuantity();
        const double bestBidQuantity = orderBook.bestBidQuantity();
        return round2(
            bestBidQuantity/bestAskQuantity
            );
    }

    double calculateBestTwoVolumeImbalance(const OrderBook& orderBook) {
        const double cumulativeSumTopTwoBidsQuantities = orderBook.cumulativeSumTopNBidQuantities(2);
        const double cumulativeSumTopTwoAsksQuantities = orderBook.cumulativeSumTopNAskQuantity(2);
        return round2(
            (cumulativeSumTopTwoBidsQuantities - cumulativeSumTopTwoAsksQuantities)
            / (cumulativeSumTopTwoBidsQuantities + cumulativeSumTopTwoAsksQuantities)
            );
    }

    double calculateBestThreeVolumeImbalance(const OrderBook& orderBook) {
        const double cumulativeSumTopThreeBidsQuantities = orderBook.cumulativeSumTopNBidQuantities(3);
        const double cumulativeSumTopThreeAsksQuantities = orderBook.cumulativeSumTopNAskQuantity(3);
        return round2(
            (cumulativeSumTopThreeBidsQuantities - cumulativeSumTopThreeAsksQuantities)
            / (cumulativeSumTopThreeBidsQuantities + cumulativeSumTopThreeAsksQuantities)
            );
    }

    double calculateBestFiveVolumeImbalance(const OrderBook& orderBook) {
        const double cumulativeSumTopFiveBidsQuantities = orderBook.cumulativeSumTopNBidQuantities(5);
        const double cumulativeSumTopFiveAsksQuantities = orderBook.cumulativeSumTopNAskQuantity(5);
        return round2(
            (cumulativeSumTopFiveBidsQuantities - cumulativeSumTopFiveAsksQuantities)
            / (cumulativeSumTopFiveBidsQuantities + cumulativeSumTopFiveAsksQuantities)
            );
    }

    double calculateVolumeImbalance(const OrderBook& orderBook) {
        return round2(
            (orderBook.sumBidQuantity() - orderBook.sumAskQuantity())
            / (orderBook.sumBidQuantity() + orderBook.sumAskQuantity())
            );
    }

    double calculateQueueImbalance(const OrderBook& orderBook) {
        return round2(
            static_cast<double>(orderBook.bidCount() - orderBook.askCount())
            / static_cast<double>(orderBook.bidCount() + orderBook.askCount())
            );
    }

    double calculateGap(const OrderBook& orderBook) {
        const double bestBidPrice = orderBook.bestBidPrice();
        const double bestAskPrice = orderBook.bestAskPrice();
        const double secondBidPrice = orderBook.secondBidPrice();
        const double secondAskPrice = orderBook.secondAskPrice();
        return round2(
            secondBidPrice + secondAskPrice - bestBidPrice - bestAskPrice
            );
    }

    bool calculateIsAggressorAsk(const TradeEntry *tradeEntry) {
        return tradeEntry->isBuyerMarketMaker;
    }

    double calculateVwapDeviation(const OrderBook& orderBook) {
        const double sumBidsAsksQuantity = orderBook.sumAskQuantity() + orderBook.sumBidQuantity();
        const double vwap = orderBook.sumPriceQty() / sumBidsAsksQuantity;
        const double midPrice = calculateMidPrice(orderBook);
        return (vwap - midPrice) / midPrice;
    }

    double calculateSimplifiedSlopeImbalance(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeSumTopNAskQuantity(5);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeSumTopNBidQuantities(5);

        const double bestFifthAskPrice = orderBook.askPriceAtDepth(K);
        const double bestFifthBidPrice = orderBook.bidPriceAtDepth(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return round2(
            (bidSlope - askSlope) / (bidSlope + askSlope)
            );
    }

    double calculateTradeCountImbalance(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds)
    {
        const size_t buy_trade_count_1_s = rollingStatisticsData.buyTradeCount(windowTimeSeconds);
        const size_t sell_trade_count_1_s = rollingStatisticsData.sellTradeCount(windowTimeSeconds);
        const size_t trade_count_sum_1_s = buy_trade_count_1_s + sell_trade_count_1_s;

        return trade_count_sum_1_s == 0
        ? 0.0
        : round2(
            static_cast<double>(
                static_cast<int64_t>(buy_trade_count_1_s) - static_cast<int64_t>(sell_trade_count_1_s))
            / static_cast<double>(trade_count_sum_1_s)
            );
    }

    double calculateCumulativeDelta(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds)
    {
        const double bid = rollingStatisticsData.buyTradeVolume(windowTimeSeconds);
        const double ask = rollingStatisticsData.sellTradeVolume(windowTimeSeconds);
        return round2(bid - ask);
    }

    double calculatePriceDifference(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds)
    {
        return rollingStatisticsData.priceDifference(windowTimeSeconds);
    }

    double calculateRateOfReturn(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds) {
        const double priceDifference = rollingStatisticsData.priceDifference(windowTimeSeconds);

        const double oldestPrice = rollingStatisticsData.oldestPrice(windowTimeSeconds);
        if (oldestPrice == 0.0) return 0.0;

        return round5(priceDifference / oldestPrice);
    }

    double calculateDifferenceDepthVolatilityImbalance(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds)
    {
        const size_t bidDifferenceDepthEntryCount = rollingStatisticsData.bidDifferenceDepthEntryCount(windowTimeSeconds);
        const size_t askDifferenceDepthEntryCount = rollingStatisticsData.askDifferenceDepthEntryCount(windowTimeSeconds);
        const size_t differenceDepthEntryCount = bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;

        return differenceDepthEntryCount == 0
        ? 0.0
        : round2(
            static_cast<double>(
                static_cast<int64_t>(bidDifferenceDepthEntryCount) - static_cast<int64_t>(askDifferenceDepthEntryCount))
            / static_cast<double>(differenceDepthEntryCount)
            );
    }

}
