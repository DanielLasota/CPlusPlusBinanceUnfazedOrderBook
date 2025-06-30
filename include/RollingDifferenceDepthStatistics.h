#pragma once
#include <array>
#include <cstdint>
#include "EntryDecoder.h"

class RollingDifferenceDepthStatistics {
public:
    void update(const DifferenceDepthEntry& entry);

    size_t bidDifferenceDepthEntryCount(int windowDurationSeconds) const;
    size_t askDifferenceDepthEntryCount(int windowDurationSeconds) const;

private:
    static constexpr int64_t    BUCKET_SIZE_US = 1'000'000; // 1 s
    static constexpr size_t     MAX_BUCKETS    = 61;        // 60 s of history

    struct Bucket {

        size_t bidDifferenceDepthEntryCount = 0;
        size_t askDifferenceDepthEntryCount = 0;

        int64_t start_time = 0;
        bool hasDifferenceDepthData = false;

        void resetDepthBucket();
    };

    std::array<Bucket, MAX_BUCKETS> buckets_;
    size_t currentBucketIdx_ = 0;
    int64_t lastDepthTimestamp_ = 0;

    static size_t getBucketIndex(int64_t timestamp);

    void advanceDepthToTimestamp(int64_t timestamp);
};
