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

inline double finiteAtanh(double x) {
    constexpr double eps = 1e-12;
    x = std::clamp(x, -1.0 + eps, 1.0 - eps);
    return std::atanh(x);
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

    double calculateMicroPriceDiff(const OrderBook& orderBook){
        const double microPrice =
            (orderBook.bestAskPrice() * orderBook.bestBidQuantity() + orderBook.bestBidPrice() * orderBook.bestAskQuantity()) / (orderBook.bestBidQuantity() + orderBook.bestAskQuantity());
        const double midPrice = calculateMidPrice(orderBook);
        return microPrice - midPrice;
    }

    double calculateMicroPriceImbalance(const OrderBook& orderBook){
        const double microPrice =
            (orderBook.bestAskPrice() * orderBook.bestBidQuantity() + orderBook.bestBidPrice() * orderBook.bestAskQuantity()) / (orderBook.bestBidQuantity() + orderBook.bestAskQuantity());
        const double midPrice = calculateMidPrice(orderBook);
        return (microPrice - midPrice) / (microPrice + midPrice);
    }

    double calculateMicroPriceFisherImbalance(const OrderBook& orderBook){
        return std::atanh(calculateMicroPriceImbalance(orderBook));
    }

    double calculateMicroPriceDeviation(const OrderBook& orderBook){
        const double microPrice =
            (orderBook.bestAskPrice() * orderBook.bestBidQuantity() + orderBook.bestBidPrice() * orderBook.bestAskQuantity()) / (orderBook.bestBidQuantity() + orderBook.bestAskQuantity());
        const double midPrice = calculateMidPrice(orderBook);
        return (microPrice - midPrice) * 100 / midPrice;
    }

    double calculateMicroPriceLogRatio(const OrderBook& orderBook){
        const double microPrice =
            (orderBook.bestAskPrice() * orderBook.bestBidQuantity() + orderBook.bestBidPrice() * orderBook.bestAskQuantity()) / (orderBook.bestBidQuantity() + orderBook.bestAskQuantity());
        const double midPrice = calculateMidPrice(orderBook);
        return std::log(microPrice / midPrice);
    }

    double calculateBestBidQuantity(const OrderBook& orderBook){
        return orderBook.bestBidQuantity();
    }

    double calculateBestAskQuantity(const OrderBook& orderBook){
        return orderBook.bestAskQuantity();
    }

    double calculateBestOrderFlowDiff(const OrderBook& orderBook) {
        return orderBook.deltaBestBidQuantity() - orderBook.deltaBestAskQuantity();
    }

    double calculateBestOrderFlowImbalance(const OrderBook& orderBook) {
        const double den = std::abs(orderBook.deltaBestBidQuantity()) + std::abs(orderBook.deltaBestAskQuantity());
        if (den <= 0.0) return 0.0;
        return (orderBook.deltaBestBidQuantity() - orderBook.deltaBestAskQuantity()) / den;
    }

    double calculateBestOrderFlowFisherImbalance(const OrderBook& orderBook) {
        return finiteAtanh(calculateBestOrderFlowImbalance(orderBook));
    }

    void computeBestOFIDeltasCKS(const OrderBook& orderBook, double& dQbid_net, double& dQask_net, double& dQbid_pos, double& dQask_pos){
        const double pBid  = orderBook.bestBidPrice();
        const double pAsk  = orderBook.bestAskPrice();
        const double qBid  = orderBook.bestBidQuantity();
        const double qAsk  = orderBook.bestAskQuantity();

        const double pBid0 = orderBook.bestBidPrice() - orderBook.deltaBestBidPrice();
        const double pAsk0 = orderBook.bestAskPrice() - orderBook.deltaBestAskPrice();
        const double qBid0 = orderBook.bestBidQuantity() - orderBook.deltaBestBidQuantity();
        const double qAsk0 = orderBook.bestAskQuantity() - orderBook.deltaBestAskQuantity();

        if (pBid > pBid0)       dQbid_net = +qBid;              // nowy wyższy bid
        else if (pBid < pBid0)  dQbid_net = -qBid0;             // bid się cofnął
        else                    dQbid_net = (qBid - qBid0);     // zmiana kolejki

        if (pAsk < pAsk0)       dQask_net = +qAsk;              // nowy niższy ask
        else if (pAsk > pAsk0)  dQask_net = -qAsk0;             // ask się cofnął
        else                    dQask_net = (qAsk - qAsk0);     // zmiana kolejki

        if (pBid > pBid0)       dQbid_pos = qBid;
        else if (pBid < pBid0)  dQbid_pos = 0.0;
        else                    dQbid_pos = std::max(0.0, qBid - qBid0);

        if (pAsk < pAsk0)       dQask_pos = qAsk;
        else if (pAsk > pAsk0)  dQask_pos = 0.0;
        else                    dQask_pos = std::max(0.0, qAsk - qAsk0);
    }

    double calculateBestOrderFlowCKSDiff(const OrderBook& orderBook){
        double dQbid, dQask, bpos, apos;
        computeBestOFIDeltasCKS(orderBook, dQbid, dQask, bpos, apos);
        return dQbid - dQask;
    }

    double calculateBestOrderFlowCKSImbalance(const OrderBook& orderBook){
        double dQbid, dQask, bpos, apos;
        computeBestOFIDeltasCKS(orderBook, dQbid, dQask, bpos, apos);
        const double den = std::abs(dQbid) + std::abs(dQask);
        if (den <= 0.0) return 0.0;
        return (dQbid - dQask) / den;
    }

    double calculateBestOrderFlowCKSFisherImbalance(const OrderBook& orderBook){
        return finiteAtanh(calculateBestOrderFlowCKSImbalance(orderBook));
    }

    double calculateOrderFlowDiff(const OrderBook& orderBook) {
        return orderBook.deltaSumBidQuantity() - orderBook.deltaSumAskQuantity();
    }

    double calculateOrderFlowImbalance(const OrderBook& orderBook) {
        const double den = std::abs(orderBook.deltaSumBidQuantity()) + std::abs(orderBook.deltaSumAskQuantity());
        if (den <= 0.0) return 0.0;
        return (orderBook.deltaSumBidQuantity() - orderBook.deltaSumAskQuantity()) / den;
    }

    double calculateOrderFlowFisherImbalance(const OrderBook& orderBook) {
        return finiteAtanh(calculateOrderFlowImbalance(orderBook));
    }

    double calculateQueueCountFlowDelta(const OrderBook& orderBook) {
        const auto bidQueueCount = static_cast<double>(orderBook.deltaBidCount());
        const auto askQueueCount = static_cast<double>(orderBook.deltaAskCount());
        return bidQueueCount - askQueueCount;
    }

    double calculateQueueCountFlowImbalance(const OrderBook& orderBook) {
        const auto bidQueueCount = static_cast<double>(orderBook.deltaBidCount());
        const auto askQueueCount = static_cast<double>(orderBook.deltaAskCount());
        const auto sum = bidQueueCount + askQueueCount;

        if (sum <= 0.0) return 0.0;

        return (bidQueueCount - askQueueCount) / sum;
    }

    double calculateQueueCountFlowFisherImbalance(const OrderBook& orderBook) {
        return finiteAtanh(calculateQueueCountFlowImbalance(orderBook));
    }

    double calculateBestVolumeDiff(const OrderBook& orderBook) {
        const double bestAskQuantity = orderBook.bestAskQuantity();
        const double bestBidQuantity = orderBook.bestBidQuantity();
        return bestBidQuantity - bestAskQuantity;
    }

    double calculateBestVolumeImbalance(const OrderBook& orderBook) {
        const double bestAskQuantity = orderBook.bestAskQuantity();
        const double bestBidQuantity = orderBook.bestBidQuantity();
        return (bestBidQuantity - bestAskQuantity) / (bestBidQuantity + bestAskQuantity);
    }

    double calculateBestVolumeFisherImbalance(const OrderBook& orderBook) {
        return std::atanh(calculateBestVolumeImbalance(orderBook));
    }

    double calculateBestVolumeLogRatio(const OrderBook& orderBook) {
        const double bestAskQuantity = orderBook.bestAskQuantity();
        const double bestBidQuantity = orderBook.bestBidQuantity();
        return std::log(bestBidQuantity/bestAskQuantity);
    }

    double calculateBestVolumeSignedLogRatioXVolume(const OrderBook& orderBook) {
        const double bestBidQuantity = orderBook.bestBidQuantity();
        const double bestAskQuantity = orderBook.bestAskQuantity();
        return calculateBestVolumeLogRatio(orderBook) * (bestBidQuantity + bestAskQuantity);
    }

    double calculateBestNPriceLevelsVolumeImbalance(const OrderBook& orderBook, const int nPriceLevels) {
        const double cumulativeSumTopNBidsQuantities = orderBook.cumulativeQuantityOfTopNBids(nPriceLevels);
        const double cumulativeSumTopNAsksQuantities = orderBook.cumulativeQuantityOfTopNAsks(nPriceLevels);
        return (cumulativeSumTopNBidsQuantities - cumulativeSumTopNAsksQuantities)
            / (cumulativeSumTopNBidsQuantities + cumulativeSumTopNAsksQuantities);
    }

    double calculateBestNPriceLevelsVolumeDiff(const OrderBook& orderBook, const int nPriceLevels) {
        return orderBook.cumulativeQuantityOfTopNBids(nPriceLevels) - orderBook.cumulativeQuantityOfTopNAsks(nPriceLevels);
    }

    double calculateBestNPriceLevelsVolumeLogRatio(const OrderBook& orderBook, const int nPriceLevels) {
        return std::log(
            orderBook.cumulativeQuantityOfTopNBids(nPriceLevels) / orderBook.cumulativeQuantityOfTopNAsks(nPriceLevels)
            );
    }

    double calculateBestNPriceLevelsVolumeLogRatioXVolume(const OrderBook& orderBook, const int nPriceLevels) {
        const double cumulativeQuantityOfTopNLevels =
            orderBook.cumulativeQuantityOfTopNBids(nPriceLevels) + orderBook.cumulativeQuantityOfTopNAsks(nPriceLevels);
        return calculateBestNPriceLevelsVolumeLogRatio(orderBook, nPriceLevels) * cumulativeQuantityOfTopNLevels;
    }

    double calculateVolumeImbalance(const OrderBook& orderBook) {
        return (orderBook.sumBidQuantity() - orderBook.sumAskQuantity())
            / (orderBook.sumBidQuantity() + orderBook.sumAskQuantity());
    }

    double calculateVolumeDiff(const OrderBook& orderBook) {
        return orderBook.sumBidQuantity() - orderBook.sumAskQuantity();
    }

    double calculateVolumeLogRatio(const OrderBook& orderBook) {
        return std::log(
            orderBook.sumBidQuantity() / orderBook.sumAskQuantity()
            );
    }

    double calculateVolumeLogRatioXVolume(const OrderBook& orderBook) {
        return calculateVolumeLogRatio(orderBook) * orderBook.sumTotalAskBidQuantity();
    }

    double calculateQueueImbalance(const OrderBook& orderBook) {
        return static_cast<double>(static_cast<int>(orderBook.bidCount()) - static_cast<int>(orderBook.askCount()))
            / static_cast<double>(static_cast<int>(orderBook.bidCount()) + static_cast<int>(orderBook.askCount()));
    }

    double calculateQueueDiff(const OrderBook& orderBook) {
        return static_cast<double>(orderBook.bidCount()) - static_cast<double>(orderBook.askCount());
    }

    double calculateQueueLogRatio(const OrderBook& orderBook) {
        return std::log(
            static_cast<double>(orderBook.bidCount()) / static_cast<double>(orderBook.askCount())
            );
    }

    double calculateQueueLogRatioXVolume(const OrderBook& orderBook) {
        return calculateQueueLogRatio(orderBook) * orderBook.sumTotalAskBidQuantity();
    }

    double calculateGap(const OrderBook& orderBook) {
        const double bestBidPrice = orderBook.bestBidPrice();
        const double bestAskPrice = orderBook.bestAskPrice();
        const double secondBidPrice = orderBook.secondBidPrice();
        const double secondAskPrice = orderBook.secondAskPrice();
        return secondBidPrice + secondAskPrice - bestBidPrice - bestAskPrice;
    }

    bool calculateIsAggressorAsk(const TradeEntry *tradeEntry) {
        return tradeEntry->isBuyerMarketMaker;
    }

    double calculateVwapDeviation(const OrderBook& orderBook) {
        const double sumBidsAsksQuantity = orderBook.sumAskQuantity() + orderBook.sumBidQuantity();
        const double vwap = orderBook.sumOfPriceTimesQuantity() / sumBidsAsksQuantity;
        const double midPrice = calculateMidPrice(orderBook);
        return (vwap - midPrice) / midPrice;
    }

    double calculateVwapLogRatio(const OrderBook& orderBook) {
        const double sumBidsAsksQuantity = orderBook.sumAskQuantity() + orderBook.sumBidQuantity();
        const double vwap = orderBook.sumOfPriceTimesQuantity() / sumBidsAsksQuantity;
        const double midPrice = calculateMidPrice(orderBook);
        return std::log(midPrice / vwap);
    }

    double calculateSimplifiedSlopeImbalance(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return (bidSlope - askSlope) / (bidSlope + askSlope);
    }

    double calculateSimplifiedSlopeDiff(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return bidSlope - askSlope;
    }

    double calculateSimplifiedSlopeLogRatio(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        const double askSlope = bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);

        return std::log(bidSlope/askSlope);
    }

    double calculateBgcSlopeImbalance(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = (midPrice - bestFifthBidPrice) / bestFiveBidsQuantityCumulativeSum;
        const double askSlope = (bestFifthAskPrice - midPrice) / bestFiveAsksQuantityCumulativeSum;

        return (bidSlope - askSlope) / (bidSlope + askSlope);
    }

    double calculateBgcSlopeDiff(const OrderBook& orderBook)
    {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = (midPrice - bestFifthBidPrice) / bestFiveBidsQuantityCumulativeSum;
        const double askSlope = (bestFifthAskPrice - midPrice) / bestFiveAsksQuantityCumulativeSum;

        return bidSlope - askSlope;
    }

    double calculateBgcSlopeLogRatio(const OrderBook& orderBook) {
        constexpr size_t K = 5;

        const double bestFiveAsksQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNAsks(K);
        const double bestFiveBidsQuantityCumulativeSum = orderBook.cumulativeQuantityOfTopNBids(K);

        const double bestFifthAskPrice = orderBook.bestNthAskPrice(K);
        const double bestFifthBidPrice = orderBook.bestNthBidPrice(K);

        const double midPrice = calculateMidPrice(orderBook);

        const double bidSlope = (midPrice - bestFifthBidPrice) / bestFiveBidsQuantityCumulativeSum;
        const double askSlope = (bestFifthAskPrice - midPrice) / bestFiveAsksQuantityCumulativeSum;

        return std::log(bidSlope/askSlope);
    }

    double calculateDifferenceDepthCount(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds){
        const auto bidDifferenceDepthEntryCount  = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(windowTimeSeconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(windowTimeSeconds));
        return bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;
    }

    double calculateTradeCount(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const auto buyTradeCount  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sellTradeCount = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));
        return buyTradeCount + sellTradeCount;
    }

    double calculateDifferenceDepthCountDiff(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds){
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(windowTimeSeconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(windowTimeSeconds));

        return bidDifferenceDepthEntryCount - askDifferenceDepthEntryCount;
    }

    double calculateDifferenceDepthCountImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds){
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(windowTimeSeconds));
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(windowTimeSeconds));
        const double total = bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;

        if (total == 0.0) return 0.0;
        return (bidDifferenceDepthEntryCount - askDifferenceDepthEntryCount) / total;
    }

    double calculateDifferenceDepthCountFisherImbalance(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds){
        return finiteAtanh(calculateDifferenceDepthCountImbalance(rollingDifferenceDepthStatistics, windowTimeSeconds));
    }

    double calculateDifferenceDepthCountLogRatio(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds)
    {
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(windowTimeSeconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(windowTimeSeconds));

        constexpr double eps = 1e-12;
        return std::log((bidDifferenceDepthEntryCount + eps) / (askDifferenceDepthEntryCount + eps));
    }

    double calculateDifferenceDepthCountLogRatioXEventCount(const RollingDifferenceDepthStatistics& rollingDifferenceDepthStatistics, const int windowTimeSeconds)
    {
        const auto bidDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.bidDifferenceDepthEntryCount(windowTimeSeconds));
        const auto askDifferenceDepthEntryCount = static_cast<double>(rollingDifferenceDepthStatistics.askDifferenceDepthEntryCount(windowTimeSeconds));
        const auto total = bidDifferenceDepthEntryCount + askDifferenceDepthEntryCount;

        constexpr double eps = 1e-12;

        return std::log((bidDifferenceDepthEntryCount + eps) / (askDifferenceDepthEntryCount + eps)) * total;
    }

    double calculateTradeCountDiff(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const auto buyTradeCount  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sellTradeCount = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));

        return buyTradeCount - sellTradeCount;
    }

    double calculateTradeCountImbalance(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const auto buys  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sells = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));

        const double total = buys + sells;
        if (total <= 0.0) return 0.0;

        return (buys - sells) / total;
    }

    double calculateTradeCountFisherImbalance(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        return finiteAtanh(calculateTradeCountImbalance(rollingTradeStatistics, windowTimeSeconds));
    }

    double calculateTradeCountLogRatio(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const auto buys  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sells = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));

        constexpr double eps = 1e-12;

        return std::log((buys + eps) / (sells + eps));
    }

    double calculateTradeVolumeDiff(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const double buyTradeVolume = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);
        return buyTradeVolume - sellTradeVolume;
    }

    double calculateTradeVolumeImbalance(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const auto buyTradeVolume  = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const auto sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);

        const double total = buyTradeVolume + sellTradeVolume;
        if (total <= 0.0) return 0.0;

        return (buyTradeVolume - sellTradeVolume) / total;
    }

    double calculateTradeVolumeLogRatio(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        constexpr double eps = 1e-12;
        const double buyTradeVolume = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);

        return std::log((buyTradeVolume + eps) / (sellTradeVolume + eps));
    }

    double calculateAvgTradeSizeDiff(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const double buyTradeVolume = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);

        const auto buyTradeCount  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sellTradeCount = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));
        const double avgTradeSizeBid = buyTradeCount > 0.0 ? buyTradeVolume / buyTradeCount : 0.0;
        const double avgTradeSizeAsk = sellTradeCount > 0.0 ? sellTradeVolume / sellTradeCount : 0.0;

        return avgTradeSizeBid - avgTradeSizeAsk;
    }

    double calculateAvgTradeSizeImbalance(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const double buyTradeVolume = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);

        const auto buyTradeCount  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sellTradeCount = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));
        const double avgTradeSizeBid = buyTradeCount > 0.0 ? buyTradeVolume / buyTradeCount : 0.0;
        const double avgTradeSizeAsk = sellTradeCount > 0.0 ? sellTradeVolume / sellTradeCount : 0.0;

        const double den = avgTradeSizeAsk + avgTradeSizeBid;
        return den == 0.0 ? 0.0 : (avgTradeSizeBid - avgTradeSizeAsk) / den;
    }

    double calculateAvgTradeSizeLogRatio(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        const double buyTradeVolume = rollingTradeStatistics.buyTradeVolume(windowTimeSeconds);
        const double sellTradeVolume = rollingTradeStatistics.sellTradeVolume(windowTimeSeconds);

        const auto buyTradeCount  = static_cast<double>(rollingTradeStatistics.buyTradeCount(windowTimeSeconds));
        const auto sellTradeCount = static_cast<double>(rollingTradeStatistics.sellTradeCount(windowTimeSeconds));
        const double avgTradeSizeBid = buyTradeCount > 0.0 ? buyTradeVolume / buyTradeCount : 0.0;
        const double avgTradeSizeAsk = sellTradeCount > 0.0 ? sellTradeVolume / sellTradeCount : 0.0;

        constexpr double eps = 1e-12;
        return std::log((avgTradeSizeBid + eps) / (avgTradeSizeAsk + eps));
    }

    double calculateBiggestSingleBuyTradeVolume(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        return rollingTradeStatistics.biggestBuyTradeNSeconds(windowTimeSeconds);
    }

    double calculateBiggestSingleSellTradeVolume(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        return rollingTradeStatistics.biggestSellTradeNSeconds(windowTimeSeconds);
    }

    double calculatePriceDifference(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        return rollingTradeStatistics.priceDifference(windowTimeSeconds);
    }

    double calculateRateOfReturn(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds) {
        const double priceDifference = rollingTradeStatistics.priceDifference(windowTimeSeconds);

        const double oldestPrice = rollingTradeStatistics.oldestPrice(windowTimeSeconds);

        if (oldestPrice == 0.0) return 0.0;

        return priceDifference * 100 / oldestPrice;
    }

    double calculateLogReturnRatio(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds){
        constexpr double eps = 1e-12;
        const double oldestPrice = rollingTradeStatistics.oldestPrice(windowTimeSeconds);
        const double lastTradePrice = rollingTradeStatistics.lastTradePrice();

        return std::log((lastTradePrice + eps)/(oldestPrice + eps));
    }

    double calculateLogKylesLambda(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds) {
        constexpr double eps = 1e-12;
        const double priceChange = std::abs(rollingTradeStatistics.priceDifference(windowTimeSeconds));
        const double totalVolume = std::abs(
            rollingTradeStatistics.buyTradeVolume(windowTimeSeconds) -
            rollingTradeStatistics.sellTradeVolume(windowTimeSeconds)
            );

        return std::log((priceChange + eps) / (totalVolume + eps));
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
        return rsi;
    }

    double calculateStochRSI(const RollingTradeStatistics& rollingTradeStatistics, const int windowTimeSeconds) {
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
        return (curr - minR) / (maxR - minR);
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
