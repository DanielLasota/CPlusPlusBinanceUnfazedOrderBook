#ifndef DIFFERENCEDEPTHSNAPSHOTENTRY_H
#define DIFFERENCEDEPTHSNAPSHOTENTRY_H

#include <cstdint>
#include <string>
#include <variant>

struct DifferenceDepthSnapshotBase {
    virtual ~DifferenceDepthSnapshotBase() = default;
    int64_t TimestampOfReceive;
    int64_t TimestampOfRequest;
    int64_t LastUpdateId;
    bool IsAsk;
    double Price;
    double Quantity;
};

struct SpotDifferenceDepthSnapshot : public DifferenceDepthSnapshotBase {
};

struct USDMFuturesDifferenceDepthSnapshot : public DifferenceDepthSnapshotBase {
    int64_t MessageOutputTime;
    int64_t TransactionTime;
};

struct CoinMFuturesDifferenceDepthSnapshot : public DifferenceDepthSnapshotBase {
    int64_t MessageOutputTime;
    int64_t TransactionTime;
    std::string Symbol;
    std::string Pair;
};

using DifferenceDepthSnapshot = std::variant<
    SpotDifferenceDepthSnapshot,
    USDMFuturesDifferenceDepthSnapshot,
    CoinMFuturesDifferenceDepthSnapshot
>;


#endif //DIFFERENCEDEPTHSNAPSHOTENTRY_H
