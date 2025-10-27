#include "RollingTradeStatistics.h"
#include <algorithm>
#include <cmath>
#include <limits>

void RollingTradeStatistics::Bucket::resetTradeBucket() {
    buyTradesCount              = 0;
    sellTradesCount             = 0;
    cumulatedBuyTradesQuantity  = 0.0;
    cumulatedSellTradesQuantity = 0.0;
    lastTradePrice              = 0.0;
    biggestBuyTrade             = 0.0;
    biggestSellTrade            = 0.0;
    highestTradePrice           = 0.0;
    lowestTradePrice            = 0.0;
    sumTradePrice               = 0.0;
    sumSqTradePrice             = 0.0;
    hasTradeData                = false;
}

size_t RollingTradeStatistics::getBucketIndex(const uint64_t timestamp) {
    return (timestamp / BUCKET_SIZE_US) % MAX_BUCKETS;
}

void RollingTradeStatistics::advanceTradeToTimestamp(const uint64_t timestamp) {
    if (timestamp <= lastTradeTimestamp_) return;
    const uint64_t old_bucket_time = lastTradeTimestamp_ / BUCKET_SIZE_US;
    const uint64_t new_bucket_time = timestamp / BUCKET_SIZE_US;
    const uint64_t buckets_to_clear = std::min(new_bucket_time - old_bucket_time, MAX_BUCKETS);
    for (uint64_t i = 1; i <= buckets_to_clear; ++i) {
        const size_t idx = getBucketIndex((old_bucket_time + i) * BUCKET_SIZE_US);
        buckets_[idx].start_time = (old_bucket_time + i) * BUCKET_SIZE_US;
        buckets_[idx].resetTradeBucket();
    }
    currentBucketIdx_ = getBucketIndex(timestamp);
    lastTradeTimestamp_ = timestamp;
}

void RollingTradeStatistics::update(const TradeEntry& e) {
    lastTradePrice_ = e.price;

    const uint64_t ts = e.timestampOfReceive;
    advanceTradeToTimestamp(ts);

    const size_t idx = getBucketIndex(ts);
    Bucket& bucket = buckets_[idx];
    bucket.start_time = (ts / BUCKET_SIZE_US) * BUCKET_SIZE_US;

    if (!bucket.hasTradeData){
        bucket.hasTradeData = true;
        bucket.highestTradePrice = e.price;
        bucket.lowestTradePrice  = e.price;
    }

    bucket.lastTradePrice = e.price;
    bucket.highestTradePrice = std::max(bucket.highestTradePrice, e.price);
    bucket.lowestTradePrice = std::min(bucket.lowestTradePrice, e.price);

    bucket.sumTradePrice   += e.price;
    bucket.sumSqTradePrice += e.price * e.price;

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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.sellTradesCount;
        }
    }
    return total;
}

double RollingTradeStatistics::getLowestPriceNSeconds(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    double lowest = std::numeric_limits<double>::max();
    bool have = false;

    for (const auto& b : buckets_) {
        if (b.hasTradeData && b.start_time >= cutoff) {
            lowest = std::min(lowest, b.lowestTradePrice);
            have = true;
        }
    }
    return have ? lowest : 0.0;
}

double RollingTradeStatistics::getHighestPriceNSeconds(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    double highest = std::numeric_limits<double>::lowest();
    bool have = false;

    for (const auto& b : buckets_) {
        if (b.hasTradeData && b.start_time >= cutoff) {
            highest = std::max(highest, b.highestTradePrice);
            have = true;
        }
    }
    return have ? highest : 0.0;
}

double RollingTradeStatistics::buyTradeVolume(const int windowDurationSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowDurationSeconds) * BUCKET_SIZE_US;

    double price_at_cutoff = 0.0;
    uint64_t best_cutoff_time = std::numeric_limits<uint64_t>::min();

    double current_price = 0.0;
    uint64_t best_current_time = std::numeric_limits<uint64_t>::min();

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

    if (best_current_time == std::numeric_limits<uint64_t>::min() ||
        best_cutoff_time == std::numeric_limits<uint64_t>::min()) {
        return 0.0;
        }

    return current_price - price_at_cutoff;
}

double RollingTradeStatistics::oldestPrice(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    double priceAtCutoff     = 0.0;
    uint64_t bestCutoffBucket = std::numeric_limits<uint64_t>::min();

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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowSeconds) * BUCKET_SIZE_US;
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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowSeconds) * BUCKET_SIZE_US;
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

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

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

double RollingTradeStatistics::hlc3t(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;

    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    bool have = false;
    double H = std::numeric_limits<double>::lowest();
    double L = std::numeric_limits<double>::max();

    for (const auto& b : buckets_) {
        if (b.hasTradeData && b.start_time >= cutoff) {
            H = std::max(H, b.highestTradePrice);
            L = std::min(L, b.lowestTradePrice);
            have = true;
        }
    }
    if (!have) return 0.0;

    const double C = lastTradePrice_;
    return (H + L + C) / 3.0;
}

double RollingTradeStatistics::priceDistance(const int windowTimeSeconds) const {
    const double H = getHighestPriceNSeconds(windowTimeSeconds);
    const double L = getLowestPriceNSeconds(windowTimeSeconds);
    const double O = oldestPrice(windowTimeSeconds);
    const double C = lastTradePrice();

    const double C_prev = oldestPrice(2 * windowTimeSeconds);

    return 2.0 * (H - L) - std::abs(C - O) + std::abs(O - C_prev);
}

double RollingTradeStatistics::rollingMeanTradePrice(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;
    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    long double S = 0.0L;
    long double N = 0.0L;

    for (auto const& b : buckets_) {
        if (b.hasTradeData && b.start_time >= cutoff) {
            S += b.sumTradePrice;
            N += static_cast<long double>(b.buyTradesCount + b.sellTradesCount);
        }
    }
    return (N > 0.0L) ? static_cast<double>(S / N) : 0.0;
}

double RollingTradeStatistics::rollingStdTradePrice(const int windowTimeSeconds) const {
    if (lastTradeTimestamp_ == 0) return 0.0;
    const uint64_t cutoff = lastTradeTimestamp_ - static_cast<uint64_t>(windowTimeSeconds) * BUCKET_SIZE_US;

    long double S = 0.0L, SS = 0.0L, N = 0.0L;
    for (auto const& b : buckets_) {
        if (b.hasTradeData && b.start_time >= cutoff) {
            S  += b.sumTradePrice;
            SS += b.sumSqTradePrice;
            N  += static_cast<long double>(b.buyTradesCount + b.sellTradesCount);
        }
    }
    if (N <= 0.0L) return 0.0;
    const long double mu  = S / N;
    const long double var = std::max<long double>(0.0L, SS / N - mu * mu); // ddof=0
    return static_cast<double>(std::sqrt(var));
}
