#pragma once

#include <cstdint>
#include <string>

struct DifferenceDepthEntry {
    int64_t TimestampOfReceive;
    std::string Stream;
    std::string EventType;
    int64_t EventTime;
    int64_t TransactionTime;
    std::string Symbol;
    int64_t FirstUpdateId;
    int64_t FinalUpdateId;
    int64_t FinalUpdateIdInLastStream;
    bool IsAsk;
    double Price;
    double Quantity;
    std::string PSUnknownField;
    bool IsLast;
};
