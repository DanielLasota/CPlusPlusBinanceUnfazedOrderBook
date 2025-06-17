#include "RollingStatisticsData.h"
#include <algorithm>

void RollingStatisticsData::Bucket::reset() {
    buy_count = sell_count = 0;
    buy_volume = sell_volume = 0.0;
    first_price = last_price = 0.0;
    has_data = false;
}

size_t RollingStatisticsData::getBucketIndex(int64_t timestamp) const {
    return (timestamp / BUCKET_SIZE_US) % MAX_BUCKETS;
}

void RollingStatisticsData::advanceToTimestamp(int64_t timestamp) {
    if (timestamp <= last_timestamp_) return;

    int64_t old_bucket_time = last_timestamp_ / BUCKET_SIZE_US;
    int64_t new_bucket_time = timestamp / BUCKET_SIZE_US;

    if (new_bucket_time > old_bucket_time) {
        int64_t buckets_to_clear = std::min(
            new_bucket_time - old_bucket_time,
            static_cast<int64_t>(MAX_BUCKETS)
        );

        for (int64_t i = 1; i <= buckets_to_clear; ++i) {
            const size_t idx = getBucketIndex((old_bucket_time + i) * BUCKET_SIZE_US);
            buckets_[idx].reset();
            buckets_[idx].start_time = (old_bucket_time + i) * BUCKET_SIZE_US;
        }

        current_bucket_idx_ = getBucketIndex(timestamp);
    }

    last_timestamp_ = timestamp;
}

void RollingStatisticsData::update(const DecodedEntry* entry) {
    std::visit([this](auto const& e) {
        if constexpr (std::is_same_v<std::decay_t<decltype(e)>, TradeEntry>) {
            int64_t ts = e.timestampOfReceive;
            advanceToTimestamp(ts);

            const size_t bucket_idx = getBucketIndex(ts);
            Bucket& bucket = buckets_[bucket_idx];

            if (!bucket.has_data) {
                bucket.first_price = e.price;
                bucket.has_data = true;
            }
            bucket.last_price = e.price;

            if (!e.isBuyerMarketMaker) {
                ++bucket.buy_count;
                bucket.buy_volume += e.quantity;
            } else {
                ++bucket.sell_count;
                bucket.sell_volume += e.quantity;
            }
        }
    }, *entry);
}

size_t RollingStatisticsData::buyTradeCount(int window_sec) const {
    if (last_timestamp_ == 0) return 0;

    int64_t cutoff = last_timestamp_ - static_cast<int64_t>(window_sec) * 1'000'000;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.has_data && bucket.start_time >= cutoff) {
            total += bucket.buy_count;
        }
    }
    return total;
}

size_t RollingStatisticsData::sellTradeCount(int window_sec) const {
    if (last_timestamp_ == 0) return 0;

    int64_t cutoff = last_timestamp_ - static_cast<int64_t>(window_sec) * 1'000'000;
    size_t total = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.has_data && bucket.start_time >= cutoff) {
            total += bucket.sell_count;
        }
    }
    return total;
}

double RollingStatisticsData::buyTradeVolume(int window_sec) const {
    if (last_timestamp_ == 0) return 0.0;

    int64_t cutoff = last_timestamp_ - static_cast<int64_t>(window_sec) * 1'000'000;
    double total = 0.0;

    for (const auto& bucket : buckets_) {
        if (bucket.has_data && bucket.start_time >= cutoff) {
            total += bucket.buy_volume;
        }
    }
    return total;
}

double RollingStatisticsData::sellTradeVolume(int window_sec) const {
    if (last_timestamp_ == 0) return 0.0;

    int64_t cutoff = last_timestamp_ - static_cast<int64_t>(window_sec) * 1'000'000;
    double total = 0.0;

    for (const auto& bucket : buckets_) {
        if (bucket.has_data && bucket.start_time >= cutoff) {
            total += bucket.sell_volume;
        }
    }
    return total;
}

double RollingStatisticsData::priceDiff(int window_sec) const {
    if (last_timestamp_ == 0) return 0.0;

    int64_t cutoff = last_timestamp_ - static_cast<int64_t>(window_sec) * 1'000'000;
    double oldest_price = 0.0;
    double newest_price = 0.0;
    int64_t oldest_time = INT64_MAX;
    int64_t newest_time = 0;

    for (const auto& bucket : buckets_) {
        if (bucket.has_data && bucket.start_time >= cutoff) {
            if (bucket.start_time >= newest_time) {
                newest_time = bucket.start_time;
                newest_price = bucket.last_price;
            }
            if (bucket.start_time <= oldest_time) {
                oldest_time = bucket.start_time;
                oldest_price = bucket.first_price;
            }
        }
    }

    return newest_time > 0 ? newest_price - oldest_price : 0.0;
}
