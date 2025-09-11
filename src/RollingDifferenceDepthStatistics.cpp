#include "RollingDifferenceDepthStatistics.h"
#include <algorithm>
#include <limits>

void RollingDifferenceDepthStatistics::Bucket::resetDepthBucket() {
    bidDifferenceDepthEntryCount = 0;
    askDifferenceDepthEntryCount = 0;
    hasDifferenceDepthData       = false;
}

size_t RollingDifferenceDepthStatistics::getBucketIndex(const int64_t timestamp) {
    return (timestamp / BUCKET_SIZE_US) % MAX_BUCKETS;
}

void RollingDifferenceDepthStatistics::advanceDepthToTimestamp(const int64_t timestamp) {
    if (timestamp <= lastDepthTimestamp_) return;
    const int64_t old_bucket_time = lastDepthTimestamp_ / BUCKET_SIZE_US;
    const int64_t new_bucket_time = timestamp / BUCKET_SIZE_US;
    const int64_t buckets_to_clear = std::min(new_bucket_time - old_bucket_time, static_cast<int64_t>(MAX_BUCKETS));
    for (int64_t i = 1; i <= buckets_to_clear; ++i) {
        const size_t idx = getBucketIndex((old_bucket_time + i) * BUCKET_SIZE_US);
        buckets_[idx].start_time = (old_bucket_time + i) * BUCKET_SIZE_US;
        buckets_[idx].resetDepthBucket();
    }
    currentBucketIdx_ = getBucketIndex(timestamp);
    lastDepthTimestamp_ = timestamp;
}

void RollingDifferenceDepthStatistics::update(const DifferenceDepthEntry& entry) {
    const int64_t ts = entry.timestampOfReceive;

    advanceDepthToTimestamp(ts);

    const size_t idx = getBucketIndex(ts);
    Bucket& bucket = buckets_[idx];
    bucket.start_time = (ts / BUCKET_SIZE_US) * BUCKET_SIZE_US;

    bucket.hasDifferenceDepthData = true;
    if (entry.isAsk) ++bucket.askDifferenceDepthEntryCount;
    else         ++bucket.bidDifferenceDepthEntryCount;
}

size_t RollingDifferenceDepthStatistics::bidDifferenceDepthEntryCount(const int windowDurationSeconds) const {
    if (lastDepthTimestamp_ == 0) return 0;
    const int64_t cutoff = lastDepthTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    size_t total = 0;
    for (auto const& bucket : buckets_) {
        if (bucket.hasDifferenceDepthData && bucket.start_time >= cutoff) {
            total += bucket.bidDifferenceDepthEntryCount;
        }
    }
    return total;
}

size_t RollingDifferenceDepthStatistics::askDifferenceDepthEntryCount(const int windowDurationSeconds) const {
    if (lastDepthTimestamp_ == 0) return 0;
    const int64_t cutoff = lastDepthTimestamp_ - static_cast<int64_t>(windowDurationSeconds) * BUCKET_SIZE_US;
    size_t total = 0;
    for (auto const& bucket : buckets_) {
        if (bucket.hasDifferenceDepthData && bucket.start_time >= cutoff) {
            total += bucket.askDifferenceDepthEntryCount;
        }
    }
    return total;
}
