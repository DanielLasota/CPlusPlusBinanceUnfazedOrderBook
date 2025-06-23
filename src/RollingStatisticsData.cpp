#include "RollingStatisticsData.h"
#include <algorithm>
#include <limits>

void RollingStatisticsData::Bucket::reset() {
    bidDifferenceDepthEntryCount = askDifferenceDepthEntryCount = 0;
    buyTradesCount = sellTradesCount = 0;
    cumulatedBuyTradesQuantity = cumulatedSellTradesQuantity = 0.0;
    firstTradePrice = lastTradePrice = 0.0;
    hasDifferenceDepthData = false;
    hasTradeData = false;
}

size_t RollingStatisticsData::getBucketIndex(int64_t timestamp) {
    return (timestamp / BUCKET_SIZE_US) % MAX_BUCKETS;
}

void RollingStatisticsData::advanceToTimestamp(int64_t timestamp) {
    if (timestamp <= lastTimestamp_) return;

    const int64_t old_bucket_time = lastTimestamp_ / BUCKET_SIZE_US;
    int64_t new_bucket_time = timestamp / BUCKET_SIZE_US;

    if (new_bucket_time > old_bucket_time) {
        int64_t buckets_to_clear = std::min(
            new_bucket_time - old_bucket_time,
            static_cast<int64_t>(MAX_BUCKETS)
        );

        for (int64_t i = 1; i <= buckets_to_clear; ++i) {
            const size_t idx = getBucketIndex((old_bucket_time + i) * BUCKET_SIZE_US);

            int64_t bucket_ts = (old_bucket_time + i) * BUCKET_SIZE_US;
            buckets_[idx].start_time = bucket_ts;

            buckets_[idx].reset();
        }

        currentBucketIdx_ = getBucketIndex(timestamp);
    }

    lastTimestamp_ = timestamp;
}

void RollingStatisticsData::update(const DecodedEntry* entry) {
    std::visit([this](auto const& e) {
        int64_t ts = e.timestampOfReceive;
        advanceToTimestamp(ts);

        size_t idx = getBucketIndex(ts);
        Bucket& bucket = buckets_[idx];
        bucket.start_time = (ts / BUCKET_SIZE_US) * BUCKET_SIZE_US;

        if constexpr (std::is_same_v<std::decay_t<decltype(e)>, TradeEntry>) {
            if (!bucket.hasTradeData) {
                bucket.firstTradePrice = e.price;
                bucket.hasTradeData = true;
            }
            bucket.lastTradePrice = e.price;

            if (!e.isBuyerMarketMaker) {
                ++bucket.buyTradesCount;
                bucket.cumulatedBuyTradesQuantity += e.quantity;
            } else {
                ++bucket.sellTradesCount;
                bucket.cumulatedSellTradesQuantity += e.quantity;
            }
        }
        else {
            if (!bucket.hasDifferenceDepthData) {
                bucket.hasDifferenceDepthData = true;
            }
            if (e.isAsk) {
                ++bucket.askDifferenceDepthEntryCount;
            } else {
                ++bucket.bidDifferenceDepthEntryCount;
            }
        }
    }, *entry);
}

size_t RollingStatisticsData::bidDifferenceDepthEntryCount(int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0;
    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    size_t total = 0;
    for (auto const& bucket : buckets_) {
        if (bucket.hasDifferenceDepthData && bucket.start_time >= cutoff) {
            total += bucket.bidDifferenceDepthEntryCount;
        }
    }
    return total;
}

size_t RollingStatisticsData::askDifferenceDepthEntryCount(int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0;
    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    size_t total = 0;
    for (auto const& bucket : buckets_) {
        if (bucket.hasDifferenceDepthData && bucket.start_time >= cutoff) {
            total += bucket.askDifferenceDepthEntryCount;
        }
    }
    return total;
}

size_t RollingStatisticsData::buyTradeCount(int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0;

    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.buyTradesCount;
        }
    }
    return total;
}

size_t RollingStatisticsData::sellTradeCount(int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0;

    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.sellTradesCount;
        }
    }
    return total;
}

double RollingStatisticsData::buyTradeVolume(int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0.0;

    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    double total = 0.0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.cumulatedBuyTradesQuantity;
        }
    }
    return total;
}

double RollingStatisticsData::sellTradeVolume(int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0.0;

    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    double total = 0.0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            total += bucket.cumulatedSellTradesQuantity;
        }
    }
    return total;
}

double RollingStatisticsData::priceDifference(const int windowDurationSeconds) const {
    if (lastTimestamp_ == 0) return 0.0;

    const int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * 1'000'000;
    double oldest_price = 0.0;
    double newest_price = 0.0;
    int64_t oldest_time = INT64_MAX;
    int64_t newest_time = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            if (bucket.start_time >= newest_time) {
                newest_time = bucket.start_time;
                newest_price = bucket.lastTradePrice;
            }
            if (bucket.start_time <= oldest_time) {
                oldest_time = bucket.start_time;
                oldest_price = bucket.firstTradePrice;
            }
        }
    }

    return newest_time > 0 ? newest_price - oldest_price : 0.0;
}

double RollingStatisticsData::oldestPrice(int windowTimeSeconds) const {
    if (lastTimestamp_ == 0) return 0.0;

    int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowTimeSeconds) * 1'000'000;

    double oldest_price = 0.0;
    int64_t oldest_time = std::numeric_limits<int64_t>::max();

    for (const auto& bucket : buckets_) {
        if (bucket.hasTradeData && bucket.start_time >= cutoff) {
            if (bucket.start_time < oldest_time) {
                oldest_time = bucket.start_time;
                oldest_price = bucket.firstTradePrice;
            }
        }
    }

    return oldest_time != std::numeric_limits<int64_t>::max()
        ? oldest_price
        : 0.0;
}

double RollingStatisticsData::simpleMovingAverage(int windowTimeSeconds) const {
    if (lastTimestamp_ == 0)
        return 0.0;

    const int64_t cutoff = lastTimestamp_ - static_cast<int64_t>(windowTimeSeconds) * 1'000'000;

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
