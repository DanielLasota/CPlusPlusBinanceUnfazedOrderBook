#pragma once
#include <array>
#include <cstdint>
#include "EntryDecoder.h"

class RollingStatisticsData {
public:
    void update(const DecodedEntry* entry);

    size_t bidDifferenceDepthEntryCount(int windowDurationSeconds) const;
    size_t askDifferenceDepthEntryCount(int windowDurationSeconds) const;

    size_t buyTradeCount(int windowDurationSeconds) const;
    size_t sellTradeCount(int windowDurationSeconds) const;
    double buyTradeVolume(int windowDurationSeconds) const;
    double sellTradeVolume(int windowDurationSeconds) const;
    double priceDifference(int windowDurationSeconds) const;
    double oldestPrice(int windowTimeSeconds) const;

    double simpleMovingAverage(int windowTimeSeconds) const;

private:
    static constexpr int64_t BUCKET_SIZE_US = 100'000; // 100ms buckets
    static constexpr size_t MAX_BUCKETS = 70; // 70s history

    struct Bucket {

        size_t bidDifferenceDepthEntryCount = 0;
        size_t askDifferenceDepthEntryCount = 0;

        int64_t start_time = 0;
        size_t buyTradesCount = 0;
        size_t sellTradesCount = 0;
        double cumulatedBuyTradesQuantity = 0.0;
        double cumulatedSellTradesQuantity = 0.0;
        double firstTradePrice = 0.0;
        double lastTradePrice = 0.0;
        bool hasDifferenceDepthData = false;
        bool hasTradeData = false;

        void reset();
    };

    std::array<Bucket, MAX_BUCKETS> buckets_;
    size_t currentBucketIdx_ = 0;
    int64_t lastTimestamp_ = 0;

    static size_t getBucketIndex(int64_t timestamp);
    void advanceToTimestamp(int64_t timestamp);
};
