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

    double calculateBestNPriceLevelsVolumeImbalance(const OrderBook& orderBook, const int nPriceLevels) {
        const double cumulativeSumTopNBidsQuantities = orderBook.cumulativeQuantityOfTopNBids(nPriceLevels);
        const double cumulativeSumTopNAsksQuantities = orderBook.cumulativeQuantityOfTopNAsks(nPriceLevels);
        return round2(
            (cumulativeSumTopNBidsQuantities - cumulativeSumTopNAsksQuantities)
            / (cumulativeSumTopNBidsQuantities + cumulativeSumTopNAsksQuantities)
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
        const double vwap = orderBook.sumOfPriceTimesQuantity() / sumBidsAsksQuantity;
        const double midPrice = calculateMidPrice(orderBook);
        return round8(
            (vwap - midPrice) / midPrice
            );
    }

    double calculateSimplifiedSlopeImbalance(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(5);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(5);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

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
        const double buyTradeVolume = rollingStatisticsData.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingStatisticsData.sellTradeVolume(windowTimeSeconds);
        return buyTradeVolume - sellTradeVolume;
    }

    double calculatePriceDifference(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds)
    {
        return round8(rollingStatisticsData.priceDifference(windowTimeSeconds));
    }

    double calculateRateOfReturn(const RollingStatisticsData& rollingStatisticsData, const int windowTimeSeconds) {
        const double priceDifference = rollingStatisticsData.priceDifference(windowTimeSeconds);

        const double oldestPrice = rollingStatisticsData.oldestPrice(windowTimeSeconds);
        if (oldestPrice == 0.0) return 0.0;

        return round2(priceDifference / oldestPrice * 100);
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


    double calculateRSI(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds)
    {
        const int periods = 14;
        double gainSum = 0.0, lossSum = 0.0;

        // dla każdego z 14 okresów pobieramy różnicę cen:
        // diff_i = (price_now - price_i*window) - (price_now - price_(i-1)*window)
        for (int i = 1; i <= periods; ++i) {
            double diff_i = rollingStatisticsData.priceDifference(i * windowTimeSeconds)
                          - rollingStatisticsData.priceDifference((i - 1) * windowTimeSeconds);
            if (diff_i > 0) gainSum += diff_i;
            else           lossSum += -diff_i;
        }

        double avgGain = gainSum / periods;
        double avgLoss = lossSum / periods;

        if (avgGain + avgLoss == 0.0) {
            return 50.0;  // brak ruchu ⇒ RSI na poziomie środka skali
        }

        double rs = (avgLoss == 0.0)
                  ? std::numeric_limits<double>::infinity()
                  : (avgGain / avgLoss);
        double rsi = 100.0 - (100.0 / (1.0 + rs));
        return round2(rsi);
    }

    double calculateStochRSI(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds)
    {
        constexpr int periods = 14;
        double rsi[periods];

        for (int i = 1; i <= periods; ++i) {
            rsi[i - 1] = calculateRSI(rollingStatisticsData, windowTimeSeconds * i);
        }

        double mn = rsi[0], mx = rsi[0];
        for (int i = 1; i < periods; ++i) {
            if (rsi[i] < mn) mn = rsi[i];
            if (rsi[i] > mx) mx = rsi[i];
        }

        const double curr = rsi[periods - 1];
        return (mx == mn)
        ? 0.0
        : round2((curr - mn) / (mx - mn));
    }

    double calculateMacd(const RollingStatisticsData& rollingStatisticsData, int windowTimeSeconds)
    {
        return 0.0;
    }

}
