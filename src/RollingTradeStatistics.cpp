#include "RollingTradeStatistics.h"
#include <algorithm>
#include <limits>

void RollingTradeStatistics::Bucket::resetTradeBucket() {
    buyTradesCount              = 0;
    sellTradesCount             = 0;
    cumulatedBuyTradesQuantity  = 0.0;
    cumulatedSellTradesQuantity = 0.0;
    firstTradePrice             = 0.0;
    lastTradePrice              = 0.0;
    biggestBuyTrade             = 0.0;
    biggestSellTrade            = 0.0;
    hasTradeData                = false;
}

size_t RollingTradeStatistics::getBucketIndex(const int64_t timestamp) {
    return (timestamp / BUCKET_SIZE_US) % MAX_BUCKETS;
}

void RollingTradeStatistics::advanceTradeToTimestamp(const int64_t timestamp) {
    if (timestamp <= lastTradeTimestamp_) return;
    const int64_t old_bucket_time = lastTradeTimestamp_ / BUCKET_SIZE_US;
    const int64_t new_bucket_time = timestamp / BUCKET_SIZE_US;
    const int64_t buckets_to_clear = std::min(new_bucket_time - old_bucket_time, static_cast<int64_t>(MAX_BUCKETS));
    for (int64_t i = 1; i <= buckets_to_clear; ++i) {
        const size_t idx = getBucketIndex((old_bucket_time + i) * BUCKET_SIZE_US);
        buckets_[idx].start_time = (old_bucket_time + i) * BUCKET_SIZE_US;
        buckets_[idx].resetTradeBucket();
    }
    currentBucketIdx_ = getBucketIndex(timestamp);
    lastTradeTimestamp_ = timestamp;
}

void RollingTradeStatistics::update(const TradeEntry& e) {
    lastTradePrice_ = e.price;

    const int64_t ts = e.timestampOfReceive;
    advanceTradeToTimestamp(ts);

    const size_t idx = getBucketIndex(ts);
    Bucket& bucket = buckets_[idx];
    bucket.start_time = (ts / BUCKET_SIZE_US) * BUCKET_SIZE_US;

    if (!bucket.hasTradeData) {
        bucket.firstTradePrice = e.price;
        bucket.hasTradeData = true;
    }
    bucket.lastTradePrice = e.price;
    if (!e.isBuyerMarketMaker) {
        ++bucket.buyTradesCount;
        bucket.cumulatedBuyTradesQuantity += e.quantity;
        bucket.biggestBuyTrade = std::max(bucket.biggestBuyTrade, e.quantity);
    } else {
        ++bucket.sellTradesCount;
        bucket.cumulatedSellTradesQuantity += e.quantity;
        bucket.biggestSellTrade = std::max(bucket.biggestSellTrade, e.quantity);
    }
}

size_t RollingTradeStatistics::buyTradeCount(const int windowDurationSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0;

    int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.buyTradesCount;
        }
    }
    return total;
}

size_t RollingTradeStatistics::sellTradeCount(const int windowDurationSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0;

    int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.sellTradesCount;
        }
    }
    return total;
}

double RollingTradeStatistics::buyTradeVolume(const int windowDurationSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    double total = 0.0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.cumulatedBuyTradesQuantity;
        }
    }
    return total;
}

double RollingTradeStatistics::sellTradeVolume(const int windowDurationSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    double total = 0.0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.cumulatedSellTradesQuantity;
        }
    }
    return total;
}

double RollingTradeStatistics::priceDifference(const int windowDurationSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;

    double price_at_cutoff = 0.0;
    int64_t best_cutoff_time = std::numeric_limits<int64_t>::min();

    double current_price = 0.0;
    int64_t best_current_time = std::numeric_limits<int64_t>::min();

    for (const auto& bucket : buckets_) {
        if (!bucket.hasTradeData) continue;

        if (bucket.start_time > best_current_time) {
            best_current_time = bucket.start_time;
            current_price = bucket.lastTradePrice;
        }

        if (bucket.start_time <= cutoff && bucket.start_time > best_cutoff_time) {
            best_cutoff_time = bucket.start_time;
            price_at_cutoff = bucket.lastTradePrice;
        }
    }

    if (best_current_time == std::numeric_limits<int64_t>::min() ||
        best_cutoff_time == std::numeric_limits<int64_t>::min()) {
        return 0.0;
        }

    return current_price - price_at_cutoff;
}

double RollingTradeStatistics::oldestPrice(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    double priceAtCutoff     = 0.0;
    int64_t bestCutoffBucket = std::numeric_limits<int64_t>::min();

    for (auto const& bucket : buckets_) {
        if (!bucket.hasTradeData) continue;

        if (bucket.start_time <= cutoff &&
            bucket.start_time > bestCutoffBucket) {
            bestCutoffBucket = bucket.start_time;
            priceAtCutoff    = bucket.lastTradePrice;
            }
    }

    return priceAtCutoff;
}

double RollingTradeStatistics::biggestBuyTradeNSeconds(const int windowSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowSeconds) * BUCKET_SIZE_US;
    double biggest = 0.0;

    for (auto const& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            biggest = std::max(biggest, bucket.biggestBuyTrade);
        }
    }
    return biggest;
}

double RollingTradeStatistics::biggestSellTradeNSeconds(const int windowSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowSeconds) * BUCKET_SIZE_US;
    double biggest = 0.0;

    for (auto const& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            biggest = std::max(biggest, bucket.biggestSellTrade);
        }
    }
    return biggest;
}

double RollingTradeStatistics::simpleMovingAverage(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0)
        return 0.0;

    const int64_t cutoff = lastTradeTimestamp_ - static_cast<int64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    double sum = 0.0;
    size_t count = 0;

    for (auto const& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            sum += bucket.lastTradePrice;
            ++count;
        }
    }

    return (count > 0) ? (sum / static_cast<double>(count)) : 0.0;
}
