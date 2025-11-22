#pragma once
#include <array>
#include <cstdint>
#include "EntryDecoder.h"

class RollingTradeStatistics {
public:

    void update(const TradeEntry& tradeEntry);

    [[nodiscard]] uint64_t getLastTradeTimestamp() const;
    [[nodiscard]] double getLastTradePrice() const;
    [[nodiscard]] size_t getBuyTradeCount(size_t windowDurationSeconds) const;
    [[nodiscard]] size_t getSellTradeCount(size_t windowDurationSeconds) const;
    [[nodiscard]] double getLowestPrice(size_t windowDurationSeconds) const;
    [[nodiscard]] double getHighestPrice(size_t windowDurationSeconds) const;
    [[nodiscard]] double getCumulatedBuyTradeVolume(size_t windowDurationSeconds) const;
    [[nodiscard]] double getCumulatedSellTradeVolume(size_t windowDurationSeconds) const;
    [[nodiscard]] double getOldestPrice(size_t windowDurationSeconds) const;
    [[nodiscard]] double getPriceDifference(size_t windowDurationSeconds) const;
    [[nodiscard]] double getClosePrice(size_t windowDurationSeconds) const;
    [[nodiscard]] double getBiggestBuyTrade(size_t windowDurationSeconds) const;
    [[nodiscard]] double getBiggestSellTrade(size_t windowDurationSeconds) const;
    [[nodiscard]] double getSMA(size_t windowDurationSeconds) const;
    [[nodiscard]] double getHLC3(size_t windowDurationSeconds) const;
    [[nodiscard]] double getPriceDistance(size_t seconds) const;
    [[nodiscard]] double getBollingerBandsDistance(size_t windowDurationSeconds) const;

private:
    static constexpr uint64_t BUCKET_SIZE_SECONDS = 1;
    static constexpr size_t MAX_BUCKETS = 1024;
    static constexpr size_t MASK = MAX_BUCKETS - 1;

    static_assert(MAX_BUCKETS != 0);
    static_assert((MAX_BUCKETS & MAX_BUCKETS - 1) == 0);
    static_assert((MAX_BUCKETS & MASK) == 0);
    static_assert(MAX_BUCKETS == 1024);

    struct Bucket {
        uint64_t startTimeSeconds = 0;
        double openPrice = 0.0;
        double closePrice = 0.0;
        size_t buyCount = 0;
        size_t sellCount = 0;
        double cumulatedBuyVolume = 0.0;
        double cumulatedSellVolume = 0.0;
        double biggestBuyVolume = 0.0;
        double biggestSellVolume = 0.0;
        double highestPrice = 0.0;
        double lowestPrice = 0.0;
        bool hasData = false;

        void resetTradeBucket();
    };

    std::array<Bucket, MAX_BUCKETS> buckets;
    size_t currentBucketId = 0;
    uint64_t lastTradeTimestampUS = 0;
    uint64_t lastBucketTimestampS = 0;
    double lastTradePrice = 0.0;

    static size_t getBucketIndex(uint64_t timestampS);

    void rewindRingBufferTape(uint64_t newTimestampS);
};
