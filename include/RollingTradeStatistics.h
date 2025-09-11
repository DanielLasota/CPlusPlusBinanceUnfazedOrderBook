#pragma once
#include <array>
#include <cstdint>
#include "EntryDecoder.h"

class RollingTradeStatistics {
public:
    void update(const TradeEntry& e);

    size_t buyTradeCount(int windowDurationSeconds) const;
    size_t sellTradeCount(int windowDurationSeconds) const;
    double buyTradeVolume(int windowDurationSeconds) const;
    double sellTradeVolume(int windowDurationSeconds) const;
    double priceDifference(int windowDurationSeconds) const;
    double oldestPrice(int windowTimeSeconds) const;
    double lastTradePrice() const { return lastTradePrice_; }
    double biggestBuyTradeNSeconds(int windowSeconds) const;
    double biggestSellTradeNSeconds(int windowSeconds) const;
    double simpleMovingAverage(int windowTimeSeconds) const;

private:
    static constexpr int64_t    BUCKET_SIZE_US = 1'000'000; // 1 s
    static constexpr size_t     MAX_BUCKETS    = 136;       // 136 s of history

    struct Bucket {

        int64_t start_time = 0;
        size_t buyTradesCount = 0;
        size_t sellTradesCount = 0;
        double cumulatedBuyTradesQuantity = 0.0;
        double cumulatedSellTradesQuantity = 0.0;
        double firstTradePrice = 0.0;
        double lastTradePrice = 0.0;

        double biggestBuyTrade = 0.0;
        double biggestSellTrade = 0.0;

        bool hasTradeData = false;

        void resetTradeBucket();
    };

    std::array<Bucket, MAX_BUCKETS> buckets_;
    size_t currentBucketIdx_ = 0;
    int64_t lastTradeTimestamp_ = 0;
    double lastTradePrice_ = 0.0;

    static size_t getBucketIndex(int64_t timestamp);

    void advanceTradeToTimestamp(int64_t timestamp);
};
