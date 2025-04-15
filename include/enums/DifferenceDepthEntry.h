//
// Created by daniel on 14.04.2025.
//

#ifndef DIFFERENCEDEPTHENTRY_H
#define DIFFERENCEDEPTHENTRY_H

#include <cstdint>
#include <string>
#include <variant>

struct OrderBookEntryBase {
    virtual ~OrderBookEntryBase() = default;
    int64_t TimestampOfReceive;
    std::string Stream;
    std::string EventType;
    int64_t EventTime;
    std::string Symbol;
    int64_t FirstUpdateId;
    int64_t FinalUpdateId;
    bool IsAsk;
    double Price;
    double Quantity;
};

struct SpotDifferenceDepthEntry : public OrderBookEntryBase {};

struct USDMFuturesDifferenceDepthEntry : public OrderBookEntryBase {
    int64_t TransactionTime;
    int64_t FinalUpdateIdInLastStream;
};

struct CoinMFuturesDifferenceDepthEntry : public OrderBookEntryBase {
    int64_t TransactionTime;
    int64_t FinalUpdateIdInLastStream;
    std::string PSUnknownField;
};

using DifferenceDepthEntry = std::variant<
    SpotDifferenceDepthEntry,
    USDMFuturesDifferenceDepthEntry,
    CoinMFuturesDifferenceDepthEntry
>;

#endif //DIFFERENCEDEPTHENTRY_H
