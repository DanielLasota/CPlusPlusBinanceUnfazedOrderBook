#include <algorithm>
#include <limits>

#include "DepthRoll.h"

void DepthRoll::Bucket::reset() {
    bidCount = 0;
    askCount = 0;
    hasData = false;
}

void DepthRoll::update(const DifferenceDepthEntry &depthEntry) {
    const uint64_t timestampOfReceiveUS = depthEntry.timestampOfReceive;
    const uint64_t timestampOfReceiveS = timestampOfReceiveUS / 1'000'000;
    if (timestampOfReceiveUS < lastDepthEntryTimestampUS) [[unlikely]] return;

    rewind(timestampOfReceiveS);

    lastDepthEntryTimestampUS = timestampOfReceiveUS;
    newestPrice = depthEntry.price;
    currentBucketId = getBucketId(timestampOfReceiveS);

    Bucket &bucket = buckets[currentBucketId];
    bucket.startTimeSeconds = timestampOfReceiveS;

    if (!bucket.hasData){
        bucket.hasData = true;
    }

    if (!depthEntry.isAsk) {
        ++bucket.bidCount;
    } else {
        ++bucket.askCount;
    }
}

void DepthRoll::rewind(const uint64_t newTimestampS) {
    if (newTimestampS <= lastBucketTimestampS) [[unlikely]] return;
    if (lastBucketTimestampS == 0) [[unlikely]] {
        lastBucketTimestampS = newTimestampS;
        return;
    }

    const uint64_t bucketsToOverride = std::min(newTimestampS - lastBucketTimestampS, MAX_BUCKETS);
    for (uint64_t i = 1; i <= bucketsToOverride; ++i) {
        const size_t id = getBucketId(lastBucketTimestampS + i);
        buckets[id].reset();
        buckets[id].startTimeSeconds = lastBucketTimestampS + i;
    }
    lastBucketTimestampS = newTimestampS;
}

size_t DepthRoll::getBucketId(const uint64_t timestampS) {
    return timestampS & MASK;
}

uint64_t DepthRoll::getNewestDepthEntryTimestamp() const {
    return lastDepthEntryTimestampUS;
}

double DepthRoll::getNewestPrice() const {
    return newestPrice;
}

DepthRoll::BidAskCount DepthRoll::getBidAskCount(const int seconds) const {
    int buy = 0;
    int sell = 0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &b = buckets[currentBucketId - i & MASK];
        buy += b.bidCount;
        sell += b.askCount;
    }

    return {buy, sell};
}
