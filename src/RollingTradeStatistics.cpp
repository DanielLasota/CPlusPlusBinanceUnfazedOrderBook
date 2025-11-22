#include "RollingTradeStatistics.h"
#include <algorithm>
#include <cmath>

void RollingTradeStatistics::Bucket::resetTradeBucket() {
    buyCount                = 0;
    sellCount               = 0;
    cumulatedBuyVolume      = 0.0;
    cumulatedSellVolume     = 0.0;
    openPrice               = 0.0;
    closePrice              = 0.0;
    biggestBuyVolume        = 0.0;
    biggestSellVolume       = 0.0;
    highestPrice            = 0.0;
    lowestPrice             = 0.0;
    hasData                 = false;
}

void RollingTradeStatistics::update(const TradeEntry& tradeEntry) {
    const uint64_t timestampOfReceiveUS = tradeEntry.timestampOfReceive;
    const uint64_t timestampOfReceiveS = timestampOfReceiveUS / 1'000'000;
    if (timestampOfReceiveUS < lastTradeTimestampUS) [[unlikely]] return;

    rewindRingBufferTape(timestampOfReceiveS);

    lastTradeTimestampUS = timestampOfReceiveUS;
    lastTradePrice = tradeEntry.price;
    currentBucketId = getBucketIndex(timestampOfReceiveS);

    Bucket& bucket = buckets[currentBucketId];
    bucket.startTimeSeconds = timestampOfReceiveS;

    if (!bucket.hasData){
        bucket.hasData = true;
        bucket.openPrice = tradeEntry.price;
        bucket.highestPrice = tradeEntry.price;
        bucket.lowestPrice  = tradeEntry.price;
    }

    bucket.closePrice = tradeEntry.price;
    bucket.highestPrice = std::max(bucket.highestPrice, tradeEntry.price);
    bucket.lowestPrice = std::min(bucket.lowestPrice, tradeEntry.price);

    if (!tradeEntry.isBuyerMarketMaker) {
        ++bucket.buyCount;
        bucket.cumulatedBuyVolume += tradeEntry.quantity;
        bucket.biggestBuyVolume = std::max(bucket.biggestBuyVolume, tradeEntry.quantity);
    } else {
        ++bucket.sellCount;
        bucket.cumulatedSellVolume += tradeEntry.quantity;
        bucket.biggestSellVolume = std::max(bucket.biggestSellVolume, tradeEntry.quantity);
    }
}

void RollingTradeStatistics::rewindRingBufferTape(const uint64_t newTimestampS) {
    if (newTimestampS <= lastBucketTimestampS) [[unlikely]] return;

    const uint64_t bucketsToOverride = std::min(newTimestampS - lastBucketTimestampS, MAX_BUCKETS);
    for (uint64_t i = 1; i <= bucketsToOverride; ++i) {
        const size_t idOfBufferToBeOverride = getBucketIndex(lastBucketTimestampS + i);
        buckets[idOfBufferToBeOverride].resetTradeBucket();
        buckets[idOfBufferToBeOverride].startTimeSeconds = lastBucketTimestampS + i;
    }
    lastBucketTimestampS = newTimestampS;
}

size_t RollingTradeStatistics::getBucketIndex(const uint64_t timestampS) {
    return timestampS & MASK;
}


uint64_t RollingTradeStatistics::getLastTradeTimestamp() const {
    return lastTradeTimestampUS;
}

double RollingTradeStatistics::getLastTradePrice() const {
    return lastTradePrice;
}

size_t RollingTradeStatistics::getBuyTradeCount(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    size_t totalBuyTradesCount = 0;

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            totalBuyTradesCount += bucket.buyCount;
        }
    }

    return totalBuyTradesCount;
}

size_t RollingTradeStatistics::getSellTradeCount(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    size_t totalSellTradesCount = 0;

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            totalSellTradesCount += bucket.sellCount;
        }
    }

    return totalSellTradesCount;
}

double RollingTradeStatistics::getLowestPrice(const size_t windowDurationSeconds) const {
    size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double lowestPrice = std::numeric_limits<double>::max();

    const Bucket& targetBucket = buckets[currentBucketId - bucketsToScan + 1 & MASK];

    if (!targetBucket.hasData) [[unlikely]] {
        for (size_t i = bucketsToScan; i < MAX_BUCKETS; ++i) {
            const size_t idx = (currentBucketId - i) & MASK;
            const Bucket& bucket = buckets[idx];

            if (bucket.hasData) [[likely]] {
                lowestPrice = bucket.closePrice;
                break;
            }
        }
    }

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            lowestPrice = std::min(lowestPrice, bucket.lowestPrice);
        }
    }

    return lowestPrice != std::numeric_limits<double>::max() ? lowestPrice : 0.0;
}

double RollingTradeStatistics::getHighestPrice(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double highestPrice = 0.0;

    const Bucket& targetBucket = buckets[currentBucketId - bucketsToScan + 1 & MASK];

    if (!targetBucket.hasData) [[unlikely]] {
        for (size_t i = bucketsToScan; i < MAX_BUCKETS; ++i) {
            const size_t idx = (currentBucketId - i) & MASK;
            const Bucket& bucket = buckets[idx];

            if (bucket.hasData) [[likely]] {
                highestPrice = bucket.highestPrice;
                break;
            }
        }
    }

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            highestPrice = std::max(highestPrice, bucket.highestPrice);
        }
    }

    return highestPrice;
}

double RollingTradeStatistics::getCumulatedBuyTradeVolume(const size_t windowDurationSeconds) const {
    const uint64_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double cumulatedBuyTradeVolume = 0;

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = currentBucketId - i & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            cumulatedBuyTradeVolume += bucket.cumulatedBuyVolume;
        }
    }

    return cumulatedBuyTradeVolume;
}

double RollingTradeStatistics::getCumulatedSellTradeVolume(const size_t windowDurationSeconds) const {
    const uint64_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double cumulatedSellTradeVolume = 0;

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = currentBucketId - i & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            cumulatedSellTradeVolume += bucket.cumulatedSellVolume;
        }
    }

    return cumulatedSellTradeVolume;
}

double RollingTradeStatistics::getOldestPrice(const size_t windowDurationSeconds) const {
    const size_t offset = std::min(windowDurationSeconds - 1, MASK);

    const Bucket& targetBucket = buckets[(currentBucketId - offset) & MASK];
    if (targetBucket.hasData) [[likely]] {
        return targetBucket.openPrice;
    }

    for (size_t i = offset; i < MAX_BUCKETS; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            return bucket.closePrice;
        }
    }

    for (size_t i = offset; i > 0; --i) {
        const size_t idx = (currentBucketId - i + 1) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            return bucket.openPrice;
        }
    }

    return lastTradePrice;
}

double RollingTradeStatistics::getPriceDifference(const size_t windowDurationSeconds) const {
    const size_t offset = std::min(windowDurationSeconds - 1, MASK);

    const Bucket& targetBucket = buckets[(currentBucketId - offset) & MASK];
    if (targetBucket.hasData) [[likely]] {
        return lastTradePrice - targetBucket.openPrice;
    }

    for (size_t i = offset; i < MAX_BUCKETS; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            return lastTradePrice - bucket.closePrice;
        }
    }

    for (size_t i = offset; i > 0; --i) {
        const size_t idx = (currentBucketId - i + 1) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            return lastTradePrice - bucket.openPrice;
        }
    }

    return 0.0;
}

double RollingTradeStatistics::getClosePrice(const size_t windowDurationSeconds) const {
    const size_t offset = std::min(windowDurationSeconds, MASK);

    const Bucket& targetBucket = buckets[currentBucketId - offset & MASK];
    if (targetBucket.hasData) [[likely]] {
        return targetBucket.closePrice;
    }

    for (size_t i = offset + 1; i < MAX_BUCKETS; ++i) {
        const size_t idx = currentBucketId - i & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            return bucket.closePrice;
        }
    }

    for (size_t i = offset; i > 0; --i) {
        const size_t idx = currentBucketId - i + 1 & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            return bucket.openPrice;
        }
    }

    return lastTradePrice;
}

double RollingTradeStatistics::getBiggestBuyTrade(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double biggestBuyVolume = 0.0;

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            biggestBuyVolume = std::max(biggestBuyVolume, bucket.biggestBuyVolume);
        }
    }

    return biggestBuyVolume;
}

double RollingTradeStatistics::getBiggestSellTrade(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double biggestSellVolume = 0.0;

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            biggestSellVolume = std::max(biggestSellVolume, bucket.biggestSellVolume);
        }
    }

    return biggestSellVolume;
}

double RollingTradeStatistics::getSMA(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double sumPrices = 0.0;
    size_t count = 0;
    double lastKnownBucketPrice = 0.0;

    const Bucket& targetBucket = buckets[currentBucketId - bucketsToScan + 1 & MASK];
    if (!targetBucket.hasData) [[unlikely]] {
        for (size_t i = bucketsToScan; i < MAX_BUCKETS; ++i) {
            const size_t idx = (currentBucketId - i) & MASK;
            const Bucket& bucket = buckets[idx];

            if (bucket.hasData) [[likely]] {
                lastKnownBucketPrice = bucket.closePrice;
                break;
            }
        }
    }

    for (size_t i = bucketsToScan; i > 0; --i) {
        const size_t idx = (currentBucketId - i + 1) & MASK;

        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            lastKnownBucketPrice = bucket.closePrice;
            sumPrices += bucket.closePrice;
            ++count;
        } else if (lastKnownBucketPrice != 0.0) [[unlikely]] {
            sumPrices += lastKnownBucketPrice;
            ++count;
        }
    }

    if (count == 0) return lastTradePrice;
    return sumPrices / static_cast<double>(count);
}

double RollingTradeStatistics::getHLC3(const size_t windowDurationSeconds) const {
    const size_t bucketsToScan = std::min(windowDurationSeconds, MAX_BUCKETS);
    double lowestPrice = std::numeric_limits<double>::infinity();
    double highestPrice = 0.0;

    const Bucket& targetBucket = buckets[currentBucketId - bucketsToScan + 1 & MASK];

    if (!targetBucket.hasData) [[unlikely]] {
        for (size_t i = bucketsToScan; i < MAX_BUCKETS; ++i) {
            const size_t idx = (currentBucketId - i) & MASK;
            const Bucket& bucket = buckets[idx];

            if (bucket.hasData) [[likely]] {
                lowestPrice = bucket.closePrice;
                highestPrice = bucket.closePrice;
                break;
            }
        }
    }

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const size_t idx = (currentBucketId - i) & MASK;
        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            lowestPrice = std::min(lowestPrice, bucket.lowestPrice);
            highestPrice = std::max(highestPrice, bucket.highestPrice);
        }
    }

    return lastTradePrice == 0.0
        ? 0.0
        : (highestPrice + lowestPrice + lastTradePrice) / 3.0;
}

double RollingTradeStatistics::getPriceDistance(const size_t seconds) const {
    // if seconds = max - earliest ohlc bucket     = one before the oldest
    // if seconds = max - earliest cPrevious       = oldest

    const size_t bucketsToScan = std::min(seconds, MASK);

    const double o = getOldestPrice(bucketsToScan);
    double h = 0.0;
    double l = std::numeric_limits<double>::infinity();
    const double c = lastTradePrice;
    const double cPrevious = getClosePrice(seconds);

    const Bucket& targetBucket = buckets[currentBucketId - bucketsToScan + 1 & MASK];

    if (!targetBucket.hasData) [[unlikely]] {
        for (size_t i = bucketsToScan; i < MAX_BUCKETS; ++i) {
            const Bucket& bucket = buckets[currentBucketId - i & MASK];

            if (bucket.hasData) [[likely]] {
                l = bucket.lowestPrice;
                h = bucket.highestPrice;
                break;
            }
        }
    }

    for (size_t i = 0; i < bucketsToScan; ++i) {
        const Bucket& bucket = buckets[currentBucketId - i & MASK];
        if (bucket.hasData) [[likely]] {
            l = std::min(l, bucket.lowestPrice);
            h = std::max(h, bucket.highestPrice);
        }
    }

    return l == std::numeric_limits<double>::infinity()
               ? 0.0
               : 2.0 * (h - l) - std::abs(c - o) + std::abs(o - cPrevious);
}

double RollingTradeStatistics::getBollingerBandsDistance(const size_t windowDurationSeconds) const {
    const size_t offset = std::min(windowDurationSeconds, MAX_BUCKETS);

    double sumPrices = 0.0;
    double sumSquares = 0.0;
    size_t count = 0;
    double lastKnownBucketPrice = 0.0;
    constexpr double eps = 1e-12;

    const Bucket& targetBucket = buckets[currentBucketId - offset + 1 & MASK];
    if (!targetBucket.hasData) [[unlikely]] {
        for (size_t i = offset; i < MAX_BUCKETS; ++i) {
            const size_t idx = (currentBucketId - i) & MASK;
            const Bucket& bucket = buckets[idx];

            if (bucket.hasData) [[likely]] {
                lastKnownBucketPrice = bucket.closePrice;
                break;
            }
        }
    }

    for (size_t i = offset; i > 0; --i) {
        const size_t idx = currentBucketId - i + 1 & MASK;

        const Bucket& bucket = buckets[idx];

        if (bucket.hasData) [[likely]] {
            const double px = bucket.closePrice;
            lastKnownBucketPrice = px;
            sumPrices += px;
            sumSquares += px*px;
            ++count;

        } else if (lastKnownBucketPrice != 0.0) [[unlikely]] {
            sumPrices += lastKnownBucketPrice;
            sumSquares += lastKnownBucketPrice*lastKnownBucketPrice;
            ++count;
        }
    }

    if (count == 0) { return 0.0; }
    const double mean = sumPrices / static_cast<double>(count);
    double variance = sumSquares / static_cast<double>(count) - mean * mean;
    if (variance < 0.0) variance = 0.0;
    const double std = std::sqrt(variance);

    return (lastTradePrice - mean) / (std + eps);
}
