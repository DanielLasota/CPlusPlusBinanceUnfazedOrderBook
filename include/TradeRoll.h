#pragma once
#include <array>
#include <cstdint>

#include "EntryDecoder.h"

class TradeRoll {
public:

    void update(const TradeEntry &trade);

    struct HiLo {
        double h;
        double l;
    };

    struct BuySellCount {
        int buy;
        int sell;
    };

    struct BuySell {
        double buy;
        double sell;
    };

    struct TValCoef {
        double tVal;
        double coef;
    };

    struct CVal {
        double bestTVal;
        double bestCoef;
        size_t bestTrendLength;
    };

    [[nodiscard]] uint64_t getLastTradeTimestamp() const;
    [[nodiscard]] double getNewestPrice() const;
    [[nodiscard]] BuySellCount getBuySellTradeCount(size_t seconds) const;
    [[nodiscard]] HiLo getHiLoPrice(size_t seconds) const;
    [[nodiscard]] BuySell getCumBuySellQuantity(size_t seconds) const;
    [[nodiscard]] BuySell getBiggestBuySell(size_t seconds) const;
    [[nodiscard]] double getOldestPrice(size_t seconds) const;
    [[nodiscard]] double getPriceDifference(size_t seconds) const;
    [[nodiscard]] double getClosePrice(size_t seconds) const;
    [[nodiscard]] double getPriceDistance(size_t seconds) const;
    [[nodiscard]] double getSMA(size_t seconds) const;
    [[nodiscard]] double getHLC3(size_t seconds) const;
    [[nodiscard]] double getBollingerBandsDistance(size_t seconds) const;
    [[nodiscard]] double getRFWMA(size_t seconds, size_t interval) const;
    [[nodiscard]] double getFWMA(size_t seconds, size_t interval) const;
    [[nodiscard]] CVal getCVal() const;
    [[nodiscard]] TValCoef getTrendCoefAndTValue(size_t secondsId) const;

private:
    static constexpr uint64_t BUCKET_SIZE_SECONDS = 1;
    static constexpr size_t MAX_BUCKETS = 16384;
    static constexpr size_t MASK = MAX_BUCKETS - 1;

    static_assert((MAX_BUCKETS &MAX_BUCKETS - 1) == 0);
    static_assert((MAX_BUCKETS &MASK) == 0);

    struct Bucket {
        uint64_t startTimeSeconds = 0;
        double openPrice = 0.0;
        double closePrice = 0.0;
        int buyCount = 0;
        int sellCount = 0;
        double cumBuyQuantity = 0.0;
        double cumSellQuantity = 0.0;
        double biggestBuyQuantity = 0.0;
        double biggestSellQuantity = 0.0;
        double highestPrice = 0.0;
        double lowestPrice = std::numeric_limits<double>::infinity();
        bool hasData = false;

        void reset();
    };

    std::array<Bucket, MAX_BUCKETS> buckets;
    size_t currentBucketId = 0;
    uint64_t lastTradeTimestampUS = 0;
    uint64_t lastBucketTimestampS = 0;
    double newestPrice = 0.0;

    static size_t getBucketId(uint64_t timestampS);

    void rewind(uint64_t newTimestampS);
};
