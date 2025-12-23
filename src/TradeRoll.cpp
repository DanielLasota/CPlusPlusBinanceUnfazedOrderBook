#include <algorithm>
#include <iostream>

#include "TradeRoll.h"

void TradeRoll::Bucket::reset() {
    buyCount                = 0;
    sellCount               = 0;
    cumBuyQuantity          = 0.0;
    cumSellQuantity         = 0.0;
    openPrice               = 0.0;
    closePrice              = 0.0;
    biggestBuyQuantity      = 0.0;
    biggestSellQuantity     = 0.0;
    highestPrice            = 0.0;
    lowestPrice             = std::numeric_limits<double>::infinity();
    hasData                 = false;
}

void TradeRoll::update(const TradeEntry &trade) {
    const uint64_t timestampOfReceiveUS = trade.timestampOfReceive;
    const uint64_t timestampOfReceiveS = timestampOfReceiveUS / 1'000'000;
    if (timestampOfReceiveUS < lastTradeTimestampUS) [[unlikely]] return;

    rewind(timestampOfReceiveS);

    lastTradeTimestampUS = timestampOfReceiveUS;
    newestPrice = trade.price;
    currentBucketId = getBucketId(timestampOfReceiveS);

    Bucket &bucket = buckets[currentBucketId];
    bucket.startTimeSeconds = timestampOfReceiveS;

    if (!bucket.hasData){
        bucket.hasData = true;
        bucket.openPrice = trade.price;
        bucket.highestPrice = trade.price;
        bucket.lowestPrice = trade.price;
    }

    bucket.closePrice = trade.price;
    bucket.highestPrice = std::max(bucket.highestPrice, trade.price);
    bucket.lowestPrice = std::min(bucket.lowestPrice, trade.price);

    if (!trade.isBuyerMarketMaker) {
        ++bucket.buyCount;
        bucket.cumBuyQuantity += trade.quantity;
        bucket.biggestBuyQuantity = std::max(bucket.biggestBuyQuantity, trade.quantity);
    } else {
        ++bucket.sellCount;
        bucket.cumSellQuantity += trade.quantity;
        bucket.biggestSellQuantity = std::max(bucket.biggestSellQuantity, trade.quantity);
    }
}

void TradeRoll::rewind(const uint64_t newTimestampS) {
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

        buckets[id].openPrice = newestPrice;
        buckets[id].highestPrice = newestPrice;
        buckets[id].lowestPrice = newestPrice;
        buckets[id].closePrice = newestPrice;
    }
    lastBucketTimestampS = newTimestampS;
}

size_t TradeRoll::getBucketId(const uint64_t timestampS) {
    return timestampS & MASK;
}


uint64_t TradeRoll::getLastTradeTimestamp() const {
    return lastTradeTimestampUS;
}

double TradeRoll::getNewestPrice() const {
    return newestPrice;
}

TradeRoll::BuySellCount TradeRoll::getBuySellTradeCount(const size_t seconds) const {
    int buy = 0;
    int sell = 0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &b = buckets[currentBucketId - i & MASK];
        buy += b.buyCount;
        sell += b.sellCount;
    }

    return {buy, sell};
}

TradeRoll::HiLo TradeRoll::getHiLoPrice(const size_t seconds) const{
    double hi = 0.0;
    double lo = std::numeric_limits<double>::infinity();

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &b = buckets[currentBucketId - i & MASK];
        hi = std::max(hi, b.highestPrice);
        lo = std::min(lo, b.lowestPrice);
    }

    return {hi, lo};
}

TradeRoll::BuySell TradeRoll::getCumBuySellQuantity(const size_t seconds) const {
    double buy = 0;
    double sell = 0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &b = buckets[currentBucketId - i & MASK];
        buy += b.cumBuyQuantity;
        sell += b.cumSellQuantity;
    }

    return {buy, sell};
}

TradeRoll::BuySell TradeRoll::getBiggestBuySell(const size_t seconds) const {
    double biggestBuyQuantity = 0.0;
    double biggestSellQuantity = 0.0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &b = buckets[currentBucketId - i & MASK];

        biggestBuyQuantity = std::max(biggestBuyQuantity, b.biggestBuyQuantity);
        biggestSellQuantity = std::max(biggestSellQuantity, b.biggestSellQuantity);
    }

    return {biggestBuyQuantity, biggestSellQuantity};
}

double TradeRoll::getOldestPrice(const size_t seconds) const {
    const Bucket &b = buckets[currentBucketId - seconds + 1 & MASK];
    return b.openPrice;
}

double TradeRoll::getPriceDifference(const size_t seconds) const {
    const Bucket &b = buckets[currentBucketId - seconds + 1 & MASK];
    return newestPrice - b.openPrice;
}

double TradeRoll::getClosePrice(const size_t seconds) const {
    const size_t offset = std::min(seconds, MASK);
    const Bucket &b = buckets[currentBucketId - offset & MASK];
    return b.closePrice;
}

double TradeRoll::getPriceDistance(const size_t seconds) const {
    const size_t bucketsToScan = std::min(seconds, MASK);
    const auto [h, l] = getHiLoPrice(bucketsToScan);
    const double o = getOldestPrice(bucketsToScan);
    const double cPrevious = getClosePrice(seconds);

    return 2.0 * (h - l) - std::abs(newestPrice - o) + std::abs(o - cPrevious);
}

double TradeRoll::getSMA(const size_t seconds) const {
    double sumPrices = 0.0;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &bucket = buckets[currentBucketId - i & MASK];
        sumPrices += bucket.closePrice;
    }

    return sumPrices / static_cast<double>(seconds);
}

double TradeRoll::getHLC3(const size_t seconds) const {
    const auto [h, l] = getHiLoPrice(seconds);
    return (h + l + newestPrice) / 3.0;
}

double TradeRoll::getBollingerBandsDistance(const size_t seconds) const {
    double sumPrices = 0.0;
    double sumSquares = 0.0;
    constexpr double eps = 1e-12;

    for (size_t i = 0; i < seconds; ++i) {
        const Bucket &b = buckets[currentBucketId - i & MASK];

        const double p = b.closePrice;

        sumPrices += p;
        sumSquares += p*p;
    }

    const double invN = 1.0 / static_cast<double>(seconds);
    const double mean = sumPrices * invN;
    double variance = sumSquares * invN - mean * mean;
    if (variance < 0.0) [[unlikely]] variance = 0.0;
    const double sd = std::sqrt(variance);

    return (newestPrice - mean) / (sd + eps);
}

double TradeRoll::getRFWMA(const size_t seconds, const size_t interval) const {
    /*
     *   getSampleInterval(5)    → 1   (sample co 1s)
     *   getSampleInterval(30)   → 1   (sample co 1s)
     *   getSampleInterval(60)   → 1   (sample co 1s)
     *   getSampleInterval(61)   → 1   (61/60 = 1)
     *   getSampleInterval(120)  → 2   (sample co 2s)
     *   getSampleInterval(180)  → 3   (sample co 3s)
     *   getSampleInterval(300)  → 5   (sample co 5s)
     *   getSampleInterval(600)  → 10  (sample co 10s)
     *   getSampleInterval(1800) → 30  (sample co 30s)
     *
     *   warning!:
     *      seconds = 120, MAX_FWMA_SAMPLES = 60, interval = 1
     */

    constexpr size_t MAX_SAMPLE_COUNT = 60;

    const size_t sampleCount = (seconds + interval - 1) / interval;
    const size_t finalSampleCount = std::min(sampleCount, MAX_SAMPLE_COUNT);

    static constexpr double fib[60] = {
        1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610,
        987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
        121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578,
        5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155,
        165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903,
        2971215073, 4807526976, 7778742049, 12586269025, 20365011074,
        32951280099, 53316291173, 86267571272, 139583862445, 225851433717,
        365435296162, 591286729879, 956722026041, 1548008755920
    };

    static constexpr double fibSum[60] = {
        1, 2, 4, 7, 12, 20, 33, 54, 88, 143, 232, 376, 609, 986, 1596,
        2583, 4180, 6764, 10945, 17710, 28656, 46367, 75024, 121392, 196417,
        317810, 514228, 832039, 1346268, 2178308, 3524577, 5702886, 9227464,
        14930351, 24157816, 39088168, 63245985, 102334154, 165580140, 267914295,
        433494436, 701408732, 1134903169, 1836311902, 2971215072, 4807526975,
        7778742048, 12586269024, 20365011073, 32951280098, 53316291172,
        86267571271, 139583862444, 225851433716, 365435296161, 591286729878,
        956722026040, 1548008755919, 2504730781960, 4052739537880
    };

    double weightedSum = 0.0;

    for (size_t i = 0; i < finalSampleCount; ++i) {
        const Bucket &b = buckets[currentBucketId - i * interval & MASK];
        weightedSum += b.closePrice * fib[i];
    }

    return weightedSum / fibSum[finalSampleCount-1];
}

double TradeRoll::getFWMA(const size_t seconds, const size_t interval) const {
    constexpr size_t MAX_SAMPLE_COUNT = 60;

    const size_t sampleCount = (seconds + interval - 1) / interval;
    const size_t finalSampleCount = std::min(sampleCount, MAX_SAMPLE_COUNT);

    static constexpr double fib[60] = {
        1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610,
        987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
        121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578,
        5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155,
        165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903,
        2971215073, 4807526976, 7778742049, 12586269025, 20365011074,
        32951280099, 53316291173, 86267571272, 139583862445, 225851433717,
        365435296162, 591286729879, 956722026041, 1548008755920
    };

    static constexpr double fibSum[60] = {
        1, 2, 4, 7, 12, 20, 33, 54, 88, 143, 232, 376, 609, 986, 1596,
        2583, 4180, 6764, 10945, 17710, 28656, 46367, 75024, 121392, 196417,
        317810, 514228, 832039, 1346268, 2178308, 3524577, 5702886, 9227464,
        14930351, 24157816, 39088168, 63245985, 102334154, 165580140, 267914295,
        433494436, 701408732, 1134903169, 1836311902, 2971215072, 4807526975,
        7778742048, 12586269024, 20365011073, 32951280098, 53316291172,
        86267571271, 139583862444, 225851433716, 365435296161, 591286729878,
        956722026040, 1548008755919, 2504730781960, 4052739537880
    };

    double weightedSum = 0.0;

    for (size_t i = 0; i < finalSampleCount; ++i) {
        const Bucket &b = buckets[currentBucketId - i * interval & MASK];
        weightedSum += b.closePrice * fib[finalSampleCount - 1 - i];
    }

    return weightedSum / fibSum[finalSampleCount-1];
}
