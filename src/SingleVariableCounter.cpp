#include <cmath>
#include <algorithm>

#include "SingleVariableCounter.h"
#include "RollingDifferenceDepthStatistics.h"
#include "RollingTradeStatistics.h"
#include "OrderBook.h"

inline double round2(const double x) {
    const double y = x * 1e2;
    const double adjusted = y + (y >= 0.0 ? 0.5 : -0.5);
    const int64_t t = static_cast<int64_t>(adjusted);
    return t / 1e2;
}

inline double round5(const double x) {
    const double y = x * 1e5;
    const double adjusted = y + (y >= 0.0 ? 0.5 : -0.5);
    const int64_t t = static_cast<int64_t>(adjusted);
    return t / 1e5;
}

inline double round8(const double x) {
    constexpr double p = 1e8; // 8
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

    double calculateBestDepthVolumeRatio(const OrderBook& orderBook) {
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
            static_cast<double>(static_cast<int>(orderBook.bidCount()) - static_cast<int>(orderBook.askCount()))
            / static_cast<double>(static_cast<int>(orderBook.bidCount()) + static_cast<int>(orderBook.askCount()))
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

    double calculateTradeCountImbalance(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds)
    {
        const size_t buy_trade_count_1_s = rollingTradeStatistics.buyTradeCount(windowTimeSeconds);
        const size_t sell_trade_count_1_s = rollingTradeStatistics.sellTradeCount(windowTimeSeconds);
        const size_t trade_count_sum_1_s = buy_trade_count_1_s + sell_trade_count_1_s;

        return trade_count_sum_1_s == 0
        ? 0.0
        : round2(
            static_cast<double>(
                static_cast<int64_t>(buy_trade_count_1_s) - static_cast<int64_t>(sell_trade_count_1_s))
            / static_cast<double>(trade_count_sum_1_s)
            );
    }

    double calculateCumulativeDelta(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds)
    {
        const double buyTradeVolume = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);
        return round8(buyTradeVolume - sellTradeVolume);
    }

    double calculatePriceDifference(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds)
    {
        return round8(rollingTradeStatistics.priceDifference(windowTimeSeconds));
    }

    double calculateRateOfReturn(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds) {
        const double priceDifference = rollingTradeStatistics.priceDifference(windowTimeSeconds);

        const double oldestPrice = rollingTradeStatistics.oldestPrice(windowTimeSeconds);
        if (oldestPrice == 0.0) return 0.0;

        const double result = priceDifference / oldestPrice;
        return round2(result * 100);
    }

    double calculateDifferenceDepthVolatilityImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds)
    {
        const size_t bidDifferenceDepthEntryCount = rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(windowTimeSeconds);
        const size_t askDifferenceDepthEntryCount = rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(windowTimeSeconds);
        const size_t differenceDepthEntryCount = bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;

        return differenceDepthEntryCount == 0
        ? 0.0
        : round2(
            static_cast<double>(
                static_cast<int64_t>(bidDifferenceDepthEntryCount) - static_cast<int64_t>(askDifferenceDepthEntryCount))
            / static_cast<double>(differenceDepthEntryCount)
            );
    }

    double calculateRSI(const RollingTradeStatistics& rollingTradeStatistics, const int startWindowTimeSeconds, const int windowTimeSeconds)
    {
        const int periods = 14;
        double gainSum = 0.0, lossSum = 0.0;

        for (int i = 1; i <= periods; ++i) {
            int t1 = startWindowTimeSeconds + i * windowTimeSeconds;
            int t0 = startWindowTimeSeconds + (i - 1) * windowTimeSeconds;

            double diff_i = rollingTradeStatistics.priceDifference(t1) - rollingTradeStatistics.priceDifference(t0);
            if (diff_i > 0)
            {
                gainSum += diff_i;
            }
            else
            {
                lossSum += -diff_i;
            }
        }

        const double avgGain = gainSum / periods;
        const double avgLoss = lossSum / periods;

        if (avgGain + avgLoss == 0.0) {
            return 50.0;  // brak ruchu ⇒ RSI na poziomie środka skali
        }

        double rs = (avgLoss == 0.0)
                  ? std::numeric_limits<double>::infinity()
                  : (avgGain / avgLoss);
        const double rsi = 100.0 - (100.0 / (1.0 + rs));
        return round2(rsi);
    }

    double calculateStochRSI(
        const RollingTradeStatistics& rollingTradeStatistics,
        const int windowTimeSeconds
    ) {
        constexpr int periods = 14;
        std::array<double, periods> rsiValues;

        // rsiValues[0] = RSI na ostatnich 14 świecach
        // rsiValues[1] = RSI od świecy -1 do -14 itd.
        for (int i = 0; i < periods; ++i) {
            const int offset = i * windowTimeSeconds;
            rsiValues[i] = calculateRSI(rollingTradeStatistics, offset, windowTimeSeconds);
        }

        const double minR = *std::ranges::min_element(rsiValues);
        const double maxR = *std::ranges::max_element(rsiValues);
        const double curr = rsiValues[0];

        if (maxR == minR) return 0.0;
        return round2((curr - minR) / (maxR - minR));
    }

    double calculateMacd(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds)
    {
        constexpr int shortPeriod = 12;
        constexpr int longPeriod  = 26;

        const double emaShort = rollingTradeStatistics.simpleMovingAverage(windowTimeSeconds * shortPeriod);
        const double emaLong  = rollingTradeStatistics.simpleMovingAverage(windowTimeSeconds * longPeriod);

        return round8(emaShort - emaLong);
    }

}
