#pragma once
#include <array>
#include <cstdint>

#include "EntryDecoder.h"

class DepthRoll {
public:
    void update(const DifferenceDepthEntry& depthEntry);

    struct BidAskCount {
        int bid;
        int ask;
    };

    [[nodiscard]] uint64_t getNewestDepthEntryTimestamp() const;
    [[nodiscard]] double getNewestPrice() const;
    [[nodiscard]] BidAskCount getBidAskCount(size_t seconds) const;

private:
    static constexpr uint64_t BUCKET_SIZE_SECONDS = 1;
    static constexpr size_t MAX_BUCKETS = 64;
    static constexpr size_t MASK = MAX_BUCKETS - 1;

    static_assert(MAX_BUCKETS != 0);
    static_assert((MAX_BUCKETS &MAX_BUCKETS - 1) == 0);
    static_assert((MAX_BUCKETS &MASK) == 0);
    static_assert(MAX_BUCKETS == 64);

    struct Bucket {
        uint64_t startTimeSeconds = 0;

        int bidCount = 0;
        int askCount = 0;

        bool hasData = false;

        void reset();
    };

    std::array<Bucket, MAX_BUCKETS> buckets;
    size_t currentBucketId = 0;
    uint64_t lastDepthEntryTimestampUS = 0;
    uint64_t lastBucketTimestampS = 0;
    double newestPrice = 0.0;

    static size_t getBucketId(uint64_t timestampS);

    void rewind(uint64_t newTimestampS);
};
