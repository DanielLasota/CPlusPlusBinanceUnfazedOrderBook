#include <algorithm>

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

    Bucket &b = buckets[currentBucketId];
    b.startTimeSeconds = timestampOfReceiveS;

    if (!b.hasData){
        b.hasData = true;
        b.openPrice = trade.price;
        b.highestPrice = trade.price;
        b.lowestPrice = trade.price;
    }

    b.closePrice = trade.price;
    b.highestPrice = std::max(b.highestPrice, trade.price);
    b.lowestPrice = std::min(b.lowestPrice, trade.price);

    if (!trade.isBuyerMarketMaker) {
        ++b.buyCount;
        b.cumBuyQuantity += trade.quantity;
        b.biggestBuyQuantity = std::max(b.biggestBuyQuantity, trade.quantity);
    } else {
        ++b.sellCount;
        b.cumSellQuantity += trade.quantity;
        b.biggestSellQuantity = std::max(b.biggestSellQuantity, trade.quantity);
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
        const Bucket &b = buckets[currentBucketId - i & MASK];
        sumPrices += b.closePrice;
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

TradeRoll::TValCoef TradeRoll::getTrendCoefAndTValue(const size_t secondsId) const {
    static constexpr std::array<size_t, 20> _len_x = {
        5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368
    };

    static constexpr std::array<double, 20> _inv_len_x = {
        1./5, 1./8, 1./13, 1./21, 1./34, 1./55, 1./89, 1./144, 1./233, 1./377, 1./610, 1./987, 1./1597, 1./2584,
        1./4181, 1./6765, 1./10946, 1./17711, 1./28657, 1./46368
    };

    static constexpr std::array<double, 20> _ssx = {
        10, 42, 182, 770, 3272, 13860, 58740, 248820, 1054092, 4465188, 18915032, 80125318, 339416798, 1437792510,
        6090588130, 25800145030, 109291171632, 462964831560, 1961150506728, 8307566858472
    };

    static constexpr std::array<double, 20> _inv_ssx = {
        1./10, 1./42, 1./182, 1./770, 1./3272, 1./13860, 1./58740, 1./248820, 1./1054092, 1./4465188, 1./18915032,
        1./80125318, 1./339416798, 1./1437792510, 1./6090588130, 1./25800145030, 1./109291171632, 1./462964831560,
        1./1961150506728, 1./8307566858472,
    };

    static constexpr std::array<double, 20> _mean_x = {
        2.0, 3.5, 6.0, 10.0, 16.5, 27.0, 44.0, 71.5, 116.0, 188.0, 304.5, 493.0, 798.0, 1291.5, 2090.0, 3382.0,
        5472.5, 8855.0, 14328.0, 23183.5,
    };

    static constexpr std::array<double, 20> _inv_n_minus_2 = {
        1./3, 1./6, 1./11, 1./19, 1./32, 1./53, 1./87, 1./142, 1./231, 1./375, 1./608, 1./985, 1./1595, 1./2582,
        1./4179, 1./6763, 1./10944, 1./17709, 1./28655, 1./46366
    };

    const size_t len_x = _len_x[secondsId];

    if (buckets[currentBucketId - len_x + 1 & MASK].closePrice == 0) {
        return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
    }

    const double inv_len_x = _inv_len_x[secondsId];
    const double mean_x = _mean_x[secondsId];
    const double ssx = _ssx[secondsId];
    const double inv_ssx = _inv_ssx[secondsId];
    const double inv_n_minus_2 = _inv_n_minus_2[secondsId];

    double sum_xy = 0.0;
    double sum_y = 0.0;
    double sum_y2 = 0.0;

    double x = 0.0;
    for (size_t i = 0; i < len_x; i++) {
        const size_t idx = currentBucketId - len_x + i + 1 & MASK;
        const Bucket &b = buckets[idx];

        sum_y += b.closePrice;
        sum_xy += x * b.closePrice;
        sum_y2 += b.closePrice * b.closePrice;
        x += 1.0;
    }

    const double sscp = sum_xy - mean_x * sum_y;
    const double ssy = sum_y2 - sum_y * sum_y * inv_len_x;
    const double s2yx = (ssy - sscp * sscp * inv_ssx) * inv_n_minus_2;

    if (!(s2yx > 0.0))
        return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};

    const double b1 = sscp * inv_ssx;
    const double tVal = b1 * std::sqrt(ssx / s2yx);

    return {tVal, b1};
}

TradeRoll::CVal TradeRoll::getCVal() const {
    double bestTVal = 0.0;
    double bestCoef = 0.0;
    size_t bestTrendLength = 0;
    double maxAbsTVal = 0.0;

    auto consider = [&](const size_t secondsId, const size_t trendLen) {
        auto [tVal, coef] = getTrendCoefAndTValue(secondsId);
        if (!std::isnan(tVal)) {
            const double absTVal = std::abs(tVal);
            if (absTVal > maxAbsTVal) {
                maxAbsTVal = absTVal;
                bestTVal = tVal;
                bestCoef = coef;
                bestTrendLength = trendLen;
            }
        }
    };

    consider(0, 5);
    consider(1, 8);
    consider(2, 13);
    consider(3, 21);
    consider(4, 34);
    consider(5, 55);
    consider(6, 89);

    consider(7, 144);
    consider(8, 233);
    consider(9, 377);
    consider(10, 610);
    consider(11, 987);

    consider(12, 1597);
    consider(13, 2584);
    consider(14, 4181);
    consider(15, 6765);
    consider(16, 10946);

    // consider(17, 17711);
    // consider(18, 28657);
    // consider(19, 46368);

    if (maxAbsTVal == 0.0) {
        return {
            std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::quiet_NaN(),
            0
        };
    }

    return {bestTVal, bestCoef, bestTrendLength};
}
