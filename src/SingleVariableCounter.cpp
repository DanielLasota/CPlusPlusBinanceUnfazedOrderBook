#include <algorithm>
#include <cmath>

#include "SingleVariableCounter.h"
#include "OrderBook.h"
#include "RollingDifferenceDepthStatistics.h"
#include "TradeRoll.h"

inline double finiteAtanh(double x) {
    constexpr double eps = 1e-12;
    x = std::clamp(x, -1.0 + eps, 1.0 - eps);
    return std::atanh(x);
}

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook &ob) {
        return ob.bestAskPrice();
    }

    double calculateBestBidPrice(const OrderBook &ob) {
        return ob.bestBidPrice();
    }

    double calculateMidPrice(const OrderBook &ob) {
        return (ob.bestBidPrice() + ob.bestAskPrice()) * 0.5;
    }

    double calculateMicroPriceDiff(const OrderBook &ob){
        const double microPrice =
            (ob.bestAskPrice() * ob.bestBidQuantity() + ob.bestBidPrice() * ob.bestAskQuantity()) / (ob.bestBidQuantity() + ob.bestAskQuantity());
        const double midPrice = calculateMidPrice(ob);
        return microPrice - midPrice;
    }

    double calculateMicroPriceImbalance(const OrderBook &ob){
        const double microPrice =
            (ob.bestAskPrice() * ob.bestBidQuantity() + ob.bestBidPrice() * ob.bestAskQuantity())
            / (ob.bestBidQuantity() + ob.bestAskQuantity());
        const double midPrice = calculateMidPrice(ob);
        return (microPrice - midPrice) / (microPrice + midPrice);
    }

    double calculateMicroPriceFisherImbalance(const OrderBook &ob){
        return finiteAtanh(calculateMicroPriceImbalance(ob));
    }

    double calculateMicroPriceDeviation(const OrderBook &ob){
        const double microPrice =
            (ob.bestAskPrice() * ob.bestBidQuantity() + ob.bestBidPrice() * ob.bestAskQuantity())
            / (ob.bestBidQuantity() + ob.bestAskQuantity());
        const double midPrice = calculateMidPrice(ob);
        return (microPrice - midPrice) * 100 / midPrice;
    }

    double calculateMicroPriceLogRatio(const OrderBook &ob){
        const double microPrice =
            (ob.bestAskPrice() * ob.bestBidQuantity() + ob.bestBidPrice() * ob.bestAskQuantity())
            / (ob.bestBidQuantity() + ob.bestAskQuantity());
        const double midPrice = calculateMidPrice(ob);
        return std::log(microPrice / midPrice);
    }

    double calculateBestBidQuantity(const OrderBook &ob){
        return ob.bestBidQuantity();
    }

    double calculateBestAskQuantity(const OrderBook &ob){
        return ob.bestAskQuantity();
    }

    double calculateBestOrderFlowDiff(const OrderBook &ob) {
        return ob.deltaBestBidQuantity() - ob.deltaBestAskQuantity();
    }

    double calculateBestOrderFlowImbalance(const OrderBook &ob) {
        const double den = std::abs(ob.deltaBestBidQuantity()) + std::abs(ob.deltaBestAskQuantity());
        if (den <= 0.0) return 0.0;
        return (ob.deltaBestBidQuantity() - ob.deltaBestAskQuantity()) / den;
    }

    double calculateBestOrderFlowFisherImbalance(const OrderBook &ob) {
        return finiteAtanh(calculateBestOrderFlowImbalance(ob));
    }

    void computeBestOFIDeltasCKS(const OrderBook &ob, double &dQBidNet, double &dQAskNet, double &dQBidPos, double &dQAskPos){
        const double pBid  = ob.bestBidPrice();
        const double pAsk  = ob.bestAskPrice();
        const double qBid  = ob.bestBidQuantity();
        const double qAsk  = ob.bestAskQuantity();

        const double pBid0 = ob.bestBidPrice() - ob.deltaBestBidPrice();
        const double pAsk0 = ob.bestAskPrice() - ob.deltaBestAskPrice();
        const double qBid0 = ob.bestBidQuantity() - ob.deltaBestBidQuantity();
        const double qAsk0 = ob.bestAskQuantity() - ob.deltaBestAskQuantity();

        if (pBid > pBid0)       dQBidNet = +qBid;
        else if (pBid < pBid0)  dQBidNet = -qBid0;
        else                    dQBidNet = qBid - qBid0;

        if (pAsk < pAsk0)       dQAskNet = +qAsk;
        else if (pAsk > pAsk0)  dQAskNet = -qAsk0;
        else                    dQAskNet = qAsk - qAsk0;

        if (pBid > pBid0)       dQBidPos = qBid;
        else if (pBid < pBid0)  dQBidPos = 0.0;
        else                    dQBidPos = std::max(0.0, qBid - qBid0);

        if (pAsk < pAsk0)       dQAskPos = qAsk;
        else if (pAsk > pAsk0)  dQAskPos = 0.0;
        else                    dQAskPos = std::max(0.0, qAsk - qAsk0);
    }

    double calculateBestOrderFlowCKSDiff(const OrderBook &ob){
        double dQBid, dQAsk, bPos, aPos;
        computeBestOFIDeltasCKS(ob, dQBid, dQAsk, bPos, aPos);
        return dQBid - dQAsk;
    }

    double calculateBestOrderFlowCKSImbalance(const OrderBook &ob){
        double dQBid, dQAsk, bPos, aPos;
        computeBestOFIDeltasCKS(ob, dQBid, dQAsk, bPos, aPos);
        const double den = std::abs(dQBid) + std::abs(dQAsk);
        if (den <= 0.0) return 0.0;
        return (dQBid - dQAsk) / den;
    }

    double calculateBestOrderFlowCKSFisherImbalance(const OrderBook &ob){
        return finiteAtanh(calculateBestOrderFlowCKSImbalance(ob));
    }

    double calculateOrderFlowDiff(const OrderBook &ob) {
        return ob.deltaSumBidQuantity() - ob.deltaSumAskQuantity();
    }

    double calculateOrderFlowImbalance(const OrderBook &ob) {
        const double den = std::abs(ob.deltaSumBidQuantity()) + std::abs(ob.deltaSumAskQuantity());
        if (den <= 0.0) return 0.0;
        return (ob.deltaSumBidQuantity() - ob.deltaSumAskQuantity()) / den;
    }

    double calculateOrderFlowFisherImbalance(const OrderBook &ob) {
        return finiteAtanh(calculateOrderFlowImbalance(ob));
    }

    int calculateQueueCountFlowDiff(const OrderBook& ob) {
        return ob.deltaBidCount() - ob.deltaAskCount();
    }

    double calculateQueueCountFlowImbalance(const OrderBook &ob) {
        const auto bidQueueCount = static_cast<double>(ob.deltaBidCount());
        const auto askQueueCount = static_cast<double>(ob.deltaAskCount());
        const auto sum = bidQueueCount + askQueueCount;

        if (sum <= 0.0) return 0.0;

        return (bidQueueCount - askQueueCount) / sum;
    }

    double calculateQueueCountFlowFisherImbalance(const OrderBook &ob) {
        return finiteAtanh(calculateQueueCountFlowImbalance(ob));
    }

    double calculateBestVolumeDiff(const OrderBook &ob) {
        return ob.bestAskQuantity() - ob.bestBidQuantity();
    }

    double calculateBestVolumeImbalance(const OrderBook &ob) {
        const double bestAskQuantity = ob.bestAskQuantity();
        const double bestBidQuantity = ob.bestBidQuantity();
        return (bestBidQuantity - bestAskQuantity) / (bestBidQuantity + bestAskQuantity);
    }

    double calculateBestVolumeFisherImbalance(const OrderBook &ob) {
        return std::atanh(calculateBestVolumeImbalance(ob));
    }

    double calculateBestVolumeLogRatio(const OrderBook &ob) {
        const double bestAskQuantity = ob.bestAskQuantity();
        const double bestBidQuantity = ob.bestBidQuantity();
        return std::log(bestBidQuantity/bestAskQuantity);
    }

    double calculateBestVolumeSignedLogRatioXVolume(const OrderBook &ob) {
        const double bestBidQuantity = ob.bestBidQuantity();
        const double bestAskQuantity = ob.bestAskQuantity();
        return calculateBestVolumeLogRatio(ob) * (bestBidQuantity + bestAskQuantity);
    }

    double calculateBestNPriceLevelsVolumeImbalance(const OrderBook &ob, const int n) {
        const double cumulativeSumTopNBidsQuantities = ob.cumulativeQuantityOfTopNBids(n);
        const double cumulativeSumTopNAsksQuantities = ob.cumulativeQuantityOfTopNAsks(n);
        return (cumulativeSumTopNBidsQuantities - cumulativeSumTopNAsksQuantities)
            / (cumulativeSumTopNBidsQuantities + cumulativeSumTopNAsksQuantities);
    }

    double calculateBestNPriceLevelsVolumeDiff(const OrderBook &ob, const int n) {
        return ob.cumulativeQuantityOfTopNBids(n) - ob.cumulativeQuantityOfTopNAsks(n);
    }

    double calculateBestNPriceLevelsVolumeLogRatio(const OrderBook &ob, const int n) {
        return std::log(ob.cumulativeQuantityOfTopNBids(n) / ob.cumulativeQuantityOfTopNAsks(n));
    }

    double calculateBestNPriceLevelsVolumeLogRatioXVolume(const OrderBook &ob, const int n) {
        const double cumulativeQuantityOfTopNLevels =
            ob.cumulativeQuantityOfTopNBids(n) + ob.cumulativeQuantityOfTopNAsks(n);
        return calculateBestNPriceLevelsVolumeLogRatio(ob, n) * cumulativeQuantityOfTopNLevels;
    }

    double calculateVolumeImbalance(const OrderBook &ob) {
        return (ob.sumBidQuantity() - ob.sumAskQuantity()) / (ob.sumBidQuantity() + ob.sumAskQuantity());
    }

    double calculateVolumeDiff(const OrderBook &ob) {
        return ob.sumBidQuantity() - ob.sumAskQuantity();
    }

    double calculateVolumeLogRatio(const OrderBook &ob) {
        return std::log(ob.sumBidQuantity() / ob.sumAskQuantity());
    }

    double calculateVolumeLogRatioXVolume(const OrderBook &ob) {
        return calculateVolumeLogRatio(ob) * ob.sumTotalAskBidQuantity();
    }

    int calculateQueueDiff(const OrderBook& ob) {
        return ob.bidCount() - ob.askCount();
    }

    double calculateQueueImbalance(const OrderBook &ob) {
        const auto bid = static_cast<double>(ob.bidCount());
        const auto ask = static_cast<double>(ob.askCount());
        return (bid - ask) / (bid + ask);
    }

    double calculateQueueLogRatio(const OrderBook &ob) {
        return std::log(static_cast<double>(ob.bidCount()) / static_cast<double>(ob.askCount()));
    }

    double calculateQueueLogRatioXVolume(const OrderBook &ob) {
        return calculateQueueLogRatio(ob) * ob.sumTotalAskBidQuantity();
    }

    double calculateGap(const OrderBook &ob) {
        const double bestBidPrice = ob.bestBidPrice();
        const double bestAskPrice = ob.bestAskPrice();
        const double secondBidPrice = ob.secondBidPrice();
        const double secondAskPrice = ob.secondAskPrice();
        return secondBidPrice + secondAskPrice - bestBidPrice - bestAskPrice;
    }

    bool calculateIsAggressorAsk(const TradeEntry *te) {
        return te->isBuyerMarketMaker;
    }

    double calculateVwapDeviation(const OrderBook &ob) {
        const double sumBidsAsksQuantity = ob.sumAskQuantity() + ob.sumBidQuantity();
        const double vwap = ob.sumOfPriceTimesQuantity() / sumBidsAsksQuantity;
        const double midPrice = calculateMidPrice(ob);
        return (vwap - midPrice) / midPrice;
    }

    double calculateVwapLogRatio(const OrderBook &ob) {
        const double sumBidsAsksQuantity = ob.sumAskQuantity() + ob.sumBidQuantity();
        const double vwap = ob.sumOfPriceTimesQuantity() / sumBidsAsksQuantity;
        const double midPrice = calculateMidPrice(ob);
        return std::log(midPrice / vwap);
    }

    double calculateSimplifiedSlopeImbalance(const OrderBook &ob)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = ob.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = ob.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = ob.bestNthAskPrice(K);
        const double bestFifthBidPrice = ob.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(ob);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return (bidSlope - askSlope) / (bidSlope + askSlope);
    }

    double calculateSimplifiedSlopeDiff(const OrderBook &ob)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = ob.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = ob.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = ob.bestNthAskPrice(K);
        const double bestFifthBidPrice = ob.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(ob);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return bidSlope - askSlope;
    }

    double calculateSimplifiedSlopeLogRatio(const OrderBook &ob)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = ob.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = ob.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = ob.bestNthAskPrice(K);
        const double bestFifthBidPrice = ob.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(ob);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return std::log(bidSlope/askSlope);
    }

    double calculateBgcSlopeImbalance(const OrderBook &ob)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = ob.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = ob.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = ob.bestNthAskPrice(K);
        const double bestFifthBidPrice = ob.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(ob);

        const double bidSlope = (midPrice - bestFifthBidPrice) / bestFiveBidsQuantityCumulativeSum;
        const double askSlope = (bestFifthAskPrice - midPrice) / bestFiveAsksQuantityCumulativeSum;

        return (bidSlope - askSlope) / (bidSlope + askSlope);
    }

    double calculateBgcSlopeDiff(const OrderBook &ob)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = ob.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = ob.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = ob.bestNthAskPrice(K);
        const double bestFifthBidPrice = ob.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(ob);

        const double bidSlope = (midPrice - bestFifthBidPrice) / bestFiveBidsQuantityCumulativeSum;
        const double askSlope = (bestFifthAskPrice - midPrice) / bestFiveAsksQuantityCumulativeSum;

        return bidSlope - askSlope;
    }

    double calculateBgcSlopeLogRatio(const OrderBook &ob) {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = ob.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = ob.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = ob.bestNthAskPrice(K);
        const double bestFifthBidPrice = ob.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(ob);

        const double bidSlope = (midPrice - bestFifthBidPrice) / bestFiveBidsQuantityCumulativeSum;
        const double askSlope = (bestFifthAskPrice - midPrice) / bestFiveAsksQuantityCumulativeSum;

        return std::log(bidSlope/askSlope);
    }

    double calculateSinTime24h(const uint64_t timestamp){
        constexpr double PI = 3.14159265358979323846;
        constexpr double DAY_US = 86'400'000'000.0;
        const double frac_of_day = std::fmod(static_cast<double>(timestamp), DAY_US) / DAY_US;
        return std::sin(2.0 * PI * frac_of_day);
    }

    double calculateLongSinTime24h(const uint64_t timestamp){
        constexpr double PI = 3.14159265358979323846;
        constexpr double PI_2 = 1.57079632679489661923;
        constexpr double DAY_US = 86'400'000'000.0;

        const double frac_of_day = std::fmod(static_cast<double>(timestamp), DAY_US) / DAY_US;

        return std::sin(PI * frac_of_day - PI_2);
    }

    double calculateCosTime24h(const uint64_t timestamp){
        constexpr double PI = 3.14159265358979323846;
        constexpr double DAY_US = 86'400'000'000.0;
        const double frac_of_day = std::fmod(static_cast<double>(timestamp), DAY_US) / DAY_US;
        return std::cos(2.0 * PI * frac_of_day);
    }

    double calculateIsAsiaTime(const uint64_t timestamp) {
        constexpr double DAY_US = 86'400'000'000.0;
        const double seconds_in_day = std::fmod(static_cast<double>(timestamp), DAY_US) / 1e6;
        const double hours = seconds_in_day / 3600.0;
        return hours >= 0.0 && hours < 8.0 ? 1.0 : 0.0;
    }

    double calculateIsEuropeTime(const uint64_t timestamp) {
        constexpr double DAY_US = 86'400'000'000.0;
        const double seconds_in_day = std::fmod(static_cast<double>(timestamp), DAY_US) / 1e6;
        const double hours = seconds_in_day / 3600.0;
        return hours >= 8.0 && hours < 16.0 ? 1.0 : 0.0;
    }

    double calculateIsUSATime(const uint64_t timestamp) {
        constexpr double DAY_US = 86'400'000'000.0;
        const double seconds_in_day = std::fmod(static_cast<double>(timestamp), DAY_US) / 1e6;
        const double hours = seconds_in_day / 3600.0;
        return hours >= 16.0 && hours < 24.0 ? 1.0 : 0.0;
    }

    double calculateDifferenceDepthCount(const RollingDifferenceDepthStatistics &rollingDifferenceDepthStatistics, const size_t seconds){
        const auto bidDifferenceDepthEntryCount  = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(seconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(seconds));
        return bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;
    }

    double calculateDifferenceDepthCountDiff(const RollingDifferenceDepthStatistics &rollingDifferenceDepthStatistics, const size_t seconds){
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(seconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(seconds));

        return bidDifferenceDepthEntryCount - askDifferenceDepthEntryCount;
    }

    double calculateDifferenceDepthCountImbalance(const RollingDifferenceDepthStatistics &rollingDifferenceDepthStatistics, const size_t seconds){
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(seconds));
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(seconds));
        const double total = bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;

        if (total == 0.0) return 0.0;
        return (bidDifferenceDepthEntryCount - askDifferenceDepthEntryCount) / total;
    }

    double calculateDifferenceDepthCountFisherImbalance(const RollingDifferenceDepthStatistics &rollingDifferenceDepthStatistics, const size_t seconds){
        return finiteAtanh(calculateDifferenceDepthCountImbalance(rollingDifferenceDepthStatistics, seconds));
    }

    double calculateDifferenceDepthCountLogRatio(const RollingDifferenceDepthStatistics &rollingDifferenceDepthStatistics, const size_t seconds)
    {
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(seconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(seconds));

        constexpr double eps = 1e-12;
        return std::log((bidDifferenceDepthEntryCount + eps) / (askDifferenceDepthEntryCount + eps));
    }

    double calculateDifferenceDepthCountLogRatioXEventCount(const RollingDifferenceDepthStatistics &rollingDifferenceDepthStatistics, const size_t seconds)
    {
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(seconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(seconds));
        const auto total = bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;

        constexpr double eps = 1e-12;

        return std::log((bidDifferenceDepthEntryCount + eps) / (askDifferenceDepthEntryCount + eps)) * total;
    }

    int calculateTradeCount(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getBuySellTradeCount(seconds);
        return buy + sell;
    }

    int calculateTradeCountDiff(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getBuySellTradeCount(seconds);
        return buy - sell;
    }

    double calculateTradeCountImbalance(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getBuySellTradeCount(seconds);

        const auto buyD  = static_cast<double>(buy);
        const auto sellD = static_cast<double>(sell);
        const double sum = buyD + sellD;

        if (sum == 0.0) [[unlikely]] return 0.0;

        return (buyD - sellD) / sum;
    }

    double calculateTradeCountFisherImbalance(const TradeRoll &tr, const size_t seconds){
        return finiteAtanh(calculateTradeCountImbalance(tr, seconds));
    }

    double calculateTradeCountLogRatio(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getBuySellTradeCount(seconds);

        const auto buyD  = static_cast<double>(buy);
        const auto sellD = static_cast<double>(sell);
        constexpr double eps = 1e-12;

        return std::log((buyD + eps) / (sellD + eps));
    }

    double calculateTradeVolumeSum(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getCumBuySellQuantity(seconds);
        return buy + sell;
    }

    double calculateTradeVolumeDiff(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getCumBuySellQuantity(seconds);
        return buy - sell;
    }

    double calculateTradeVolumeImbalance(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getCumBuySellQuantity(seconds);
        const double sum = buy + sell;

        if (sum == 0.0) [[unlikely]] return 0.0;

        return (buy - sell) / sum;
    }

    double calculateTradeVolumeLogRatio(const TradeRoll &tr, const size_t seconds){
        const auto [buy, sell] = tr.getCumBuySellQuantity(seconds);
        constexpr double eps = 1e-12;

        return std::log((buy + eps) / (sell + eps));
    }

    double calculateBiggestSingleBuyTradeVolume(const TradeRoll &tr, const size_t seconds){
        return tr.getBiggestBuySell(seconds).buy;
    }

    double calculateBiggestSingleSellTradeVolume(const TradeRoll &tr, const size_t seconds){
        return tr.getBiggestBuySell(seconds).sell;
    }

    double calculateAvgTradeSizeDiff(const TradeRoll &tr, const size_t seconds){
        const auto [buyQuantity, sellQuantity] = tr.getCumBuySellQuantity(seconds);
        const auto [buyCount, sellCount] = tr.getBuySellTradeCount(seconds);

        if (buyCount == 0 && sellCount == 0) [[unlikely]] return 0.0;

        const double avgBuyQuantity  = buyCount  > 0 ? buyQuantity  / static_cast<double>(buyCount)  : 0.0;
        const double avgSellQuantity = sellCount > 0 ? sellQuantity / static_cast<double>(sellCount) : 0.0;

        return avgBuyQuantity - avgSellQuantity;
    }

    double calculateAvgTradeSizeImbalance(const TradeRoll &tr, const size_t seconds){
        const auto [buyQuantity, sellQuantity] = tr.getCumBuySellQuantity(seconds);
        const auto [buyCount, sellCount] = tr.getBuySellTradeCount(seconds);

        if (buyCount == 0 && sellCount == 0) [[unlikely]] return 0.0;

        const double avgBuyQuantity  = buyCount  > 0 ? buyQuantity  / static_cast<double>(buyCount)  : 0.0;
        const double avgSellQuantity = sellCount > 0 ? sellQuantity / static_cast<double>(sellCount) : 0.0;

        const double sum = buyQuantity + sellQuantity;
        if (sum == 0.0) [[unlikely]] return 0.0;

        return (avgBuyQuantity - avgSellQuantity) / sum;
    }

    double calculateAvgTradeSizeLogRatio(const TradeRoll &tr, const size_t seconds){
        const auto [buyQuantity, sellQuantity] = tr.getCumBuySellQuantity(seconds);
        const auto [buyCount, sellCount] = tr.getBuySellTradeCount(seconds);

        if (buyCount == 0 && sellCount == 0) [[unlikely]] return 0.0;

        const double avgBuyQuantity  = buyCount  > 0 ? buyQuantity  / static_cast<double>(buyCount)  : 0.0;
        const double avgSellQuantity = sellCount > 0 ? sellQuantity / static_cast<double>(sellCount) : 0.0;

        constexpr double eps = 1e-12;

        return std::log((avgBuyQuantity + eps) / (avgSellQuantity + eps));
    }

    double calculatePriceDifference(const TradeRoll &tr, const size_t seconds){
        return tr.getPriceDifference(seconds);
    }

    double calculateRateOfReturn(const TradeRoll &tr, const size_t seconds) {
        const double newestPrice = tr.getNewestPrice();
        const double oldestPrice = tr.getOldestPrice(seconds);

        if (oldestPrice == 0.0) [[unlikely]] return 0.0;

        return (newestPrice - oldestPrice) / oldestPrice;
    }

    double calculateLogReturnRatio(const TradeRoll &tr, const size_t seconds){
        const double oldestPrice = tr.getOldestPrice(seconds);
        const double newestPrice = tr.getNewestPrice();

        constexpr double eps = 1e-12;

        return std::log((newestPrice + eps)/(oldestPrice + eps));
    }

    double calculateSMALogRatio(const TradeRoll &tr, const size_t seconds){
        constexpr double eps = 1e-12;
        const double sma = tr.getSMA(seconds);
        const double newestPrice = tr.getNewestPrice();

        return std::log((newestPrice + eps) / (sma + eps));
    }

    double calculateHLC3TLogRatio(const TradeRoll &tr, const size_t seconds){
        const double hlc3 = tr.getHLC3(seconds);
        const double newestPrice = tr.getNewestPrice();

        constexpr double eps = 1e-12;

        return std::log((newestPrice + eps) / (hlc3 + eps));
    }

    double calculateBollingerBandsDistance(const TradeRoll &tr, const size_t seconds){
        return tr.getBollingerBandsDistance(seconds);
    }

    double calculatePriceDistance(const TradeRoll &tr, const size_t seconds){
        return tr.getPriceDistance(seconds);
    }

    double calculateLogKylesLambda(const TradeRoll &tr, const size_t seconds) {
        constexpr double eps = 1e-12;
        const double priceChange = std::abs(tr.getPriceDifference(seconds));
        const auto [cumBuyQuantity, cumSellQuantity] = tr.getCumBuySellQuantity(seconds);
        const double cumQuantityAbsDiff = std::abs(cumBuyQuantity - cumSellQuantity);

        return std::log((priceChange + eps) / (cumQuantityAbsDiff + eps));
    }

    double calculateDeltaPVT(const TradeRoll &tr, const size_t seconds){
        return calculateRateOfReturn(tr, seconds) * calculateTradeVolumeSum(tr, seconds);
    }
}
