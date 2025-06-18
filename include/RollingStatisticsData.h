#pragma once
#include <array>
#include <cstdint>
#include "EntryDecoder.h"

class RollingStatisticsData {
public:
    void update(const DecodedEntry* entry);

    size_t buyTradeCount(int windowDurationSeconds) const;
    size_t sellTradeCount(int windowDurationSeconds) const;
    double buyTradeVolume(int windowDurationSeconds) const;
    double sellTradeVolume(int windowDurationSeconds) const;
    double priceDifference(int windowDurationSeconds) const;
    double oldestPrice(int windowTimeSeconds) const;

private:
    static constexpr int64_t BUCKET_SIZE_US = 100'000; // 100ms buckets
    static constexpr size_t MAX_BUCKETS = 10; // 10s history

    struct Bucket {
        int64_t start_time = 0;
        size_t buy_count = 0;
        size_t sell_count = 0;
        double buy_volume = 0.0;
        double sell_volume = 0.0;
        double first_price = 0.0;
        double last_price = 0.0;
        bool has_data = false;

        void reset();
    };

    std::array<Bucket, MAX_BUCKETS> buckets_;
    size_t current_bucket_idx_ = 0;
    int64_t last_timestamp_ = 0;

    size_t getBucketIndex(int64_t timestamp) const;
    void advanceToTimestamp(int64_t timestamp);
};
