#include <algorithm>
#include <iostream>

#include "RollingTradeStatistics.h"

void RollingTradeStatistics::Bucket::reset() {
    buyCount                = 0;
    sellCount               = 0;
    cumBuyQuantity          = 0.0;
    cumSellQuantity         = 0.0;
    openPrice               = 0.0;
    closePrice              = 0.0;
    biggestBuyQuantity      = 0.0;
    biggestSellQuantity     = 0.0;
    highestPrice            = 0.0;
    lowestPrice             = std::numeric_limits<double>::infinity();
    hasData                 = false;
}

void RollingTradeStatistics::update(const TradeEntry& trade) {
    const uint64_t timestampOfReceiveUS = trade.timestampOfReceive;
    const uint64_t timestampOfReceiveS = timestampOfReceiveUS / 1'000'000;
    if (timestampOfReceiveUS < lastTradeTimestampUS) [[unlikely]] return;

    rewind(timestampOfReceiveS);

    lastTradeTimestampUS = timestampOfReceiveUS;
    newestPrice = trade.price;
    currentBucketId = getBucketId(timestampOfReceiveS);

    Bucket& bucket = buckets[currentBucketId];
    bucket.startTimeSeconds = timestampOfReceiveS;

    if (!bucket.hasData){
        bucket.hasData = true;
        bucket.openPrice = trade.price;
        bucket.highestPrice = trade.price;
        bucket.lowestPrice = trade.price;
    }

    bucket.closePrice = trade.price;
    bucket.highestPrice = std::max(bucket.highestPrice, trade.price);
    bucket.lowestPrice = std::min(bucket.lowestPrice, trade.price);

    if (!trade.isBuyerMarketMaker) {
        ++bucket.buyCount;
        bucket.cumBuyQuantity += trade.quantity;
        bucket.biggestBuyQuantity = std::max(bucket.biggestBuyQuantity, trade.quantity);
    } else {
        ++bucket.sellCount;
        bucket.cumSellQuantity += trade.quantity;
        bucket.biggestSellQuantity = std::max(bucket.biggestSellQuantity, trade.quantity);
    }
}

void RollingTradeStatistics::rewind(const uint64_t newTimestampS) {
    if (newTimestampS <= lastBucketTimestampS) [[unlikely]] return;
    if (lastBucketTimestampS == 0) [[unlikely]] {
        lastBucketTimestampS = newTimestampS;
        return;
    }

    const uint64_t bucketsToOverride = std::min(newTimestampS - lastBucketTimestampS, MAX_BUCKETS);
    for (uint64_t i = 1; i <= bucketsToOverride; ++i) {
        const size_t id = getBucketId(lastBucketTimestampS + i);
        buckets[id].reset();
        buckets[id].startTimeSeconds = lastBucketTimestampS + i;

        buckets[id].openPrice = newestPrice;
        buckets[id].highestPrice = newestPrice;
        buckets[id].lowestPrice = newestPrice;
        buckets[id].closePrice = newestPrice;
    }
    lastBucketTimestampS = newTimestampS;
}

size_t RollingTradeStatistics::getBucketId(const uint64_t timestampS) {
    return timestampS & MASK;
}


uint64_t RollingTradeStatistics::getLastTradeTimestamp() const {
    return lastTradeTimestampUS;
}

double RollingTradeStatistics::getNewestPrice() const {
    return newestPrice;
}

RollingTradeStatistics::BuySellCount RollingTradeStatistics::getBuySellTradeCount(const size_t seconds) const {
    int buy = 0;
    int sell = 0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket& b = buckets[currentBucketId - i & MASK];
        buy += b.buyCount;
        sell += b.sellCount;
    }

    return {buy, sell};
}

RollingTradeStatistics::HiLo RollingTradeStatistics::getHiLoPrice(const size_t seconds) const{
    double hi = 0.0;
    double lo = std::numeric_limits<double>::infinity();

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket& b = buckets[currentBucketId - i & MASK];
        hi = std::max(hi, b.highestPrice);
        lo = std::min(lo, b.lowestPrice);
    }

    return {hi, lo};
}

RollingTradeStatistics::BuySell RollingTradeStatistics::getCumBuySellQuantity(const size_t seconds) const {
    double buy = 0;
    double sell = 0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket& b = buckets[currentBucketId - i & MASK];
        buy += b.cumBuyQuantity;
        sell += b.cumSellQuantity;
    }

    return {buy, sell};
}

double RollingTradeStatistics::getOldestPrice(const size_t seconds) const {
    const Bucket& b = buckets[currentBucketId - seconds + 1 & MASK];
    return b.openPrice;
}

double RollingTradeStatistics::getPriceDifference(const size_t seconds) const {
    const Bucket& b = buckets[currentBucketId - seconds + 1 & MASK];
    return newestPrice - b.openPrice;
}

double RollingTradeStatistics::getClosePrice(const size_t seconds) const {
    const size_t offset = std::min(seconds, MASK);
    const Bucket& b = buckets[currentBucketId - offset & MASK];
    return b.closePrice;
}

RollingTradeStatistics::BuySell RollingTradeStatistics::getBiggestBuySell(const size_t seconds) const {
    double biggestBuyQuantity = 0.0;
    double biggestSellQuantity = 0.0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket& b = buckets[currentBucketId - i & MASK];

        biggestBuyQuantity = std::max(biggestBuyQuantity, b.biggestBuyQuantity);
        biggestSellQuantity = std::max(biggestSellQuantity, b.biggestSellQuantity);
    }

    return {biggestBuyQuantity, biggestSellQuantity};
}

double RollingTradeStatistics::getSMA(const size_t seconds) const {
    double sumPrices = 0.0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket& bucket = buckets[currentBucketId - i & MASK];
        sumPrices += bucket.closePrice;
    }

    return sumPrices / static_cast<double>(seconds);
}

double RollingTradeStatistics::getHLC3(const size_t seconds) const {
    const HiLo hl = getHiLoPrice(seconds);
    return (hl.h + hl.l + newestPrice) / 3.0;
}

double RollingTradeStatistics::getPriceDistance(const size_t seconds) const {
    const size_t bucketsToScan = std::min(seconds, MASK);
    const HiLo hl = getHiLoPrice(bucketsToScan);
    const double o = getOldestPrice(bucketsToScan);
    const double cPrevious = getClosePrice(seconds);

    return 2.0 * (hl.h - hl.l) - std::abs(newestPrice - o) + std::abs(o - cPrevious);
}

double RollingTradeStatistics::getBollingerBandsDistance(const size_t seconds) const {
    double sumPrices = 0.0;
    double sumSquares = 0.0;
    constexpr double eps = 1e-12;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket& b = buckets[currentBucketId - i & MASK];

        const double p = b.closePrice;

        sumPrices += p;
        sumSquares += p*p;
    }

    const double invN = 1.0 / static_cast<double>(seconds);
    const double mean = sumPrices * invN;
    double variance = sumSquares * invN - mean * mean;
    if (variance < 0.0) [[unlikely]] variance = 0.0;
    const double sd = std::sqrt(variance);

    return (newestPrice - mean) / (sd + eps);
}
