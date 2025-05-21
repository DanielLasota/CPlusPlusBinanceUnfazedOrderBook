#pragma once

#include <cstdint>
#include <string>
#include "enums/Market.h"

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
    Market Market_;
    DifferenceDepthEntry* prev_;
    DifferenceDepthEntry* next_;

    DifferenceDepthEntry(
        int64_t TimestampOfReceive,
        const std::string& Stream,
        const std::string& EventType,
        int64_t EventTime,
        int64_t TransactionTime,
        const std::string& Symbol,
        int64_t FirstUpdateId,
        int64_t FinalUpdateId,
        int64_t FinalUpdateIdInLastStream,
        bool IsAsk,
        double Price,
        double Quantity,
        const std::string& PSUnknownField,
        bool IsLast,
        Market Market_,
        DifferenceDepthEntry* prev_ = nullptr,
        DifferenceDepthEntry* next_ = nullptr
    )
    : TimestampOfReceive(TimestampOfReceive)
    , Stream(Stream)
    , EventType(EventType)
    , EventTime(EventTime)
    , TransactionTime(TransactionTime)
    , Symbol(Symbol)
    , FirstUpdateId(FirstUpdateId)
    , FinalUpdateId(FinalUpdateId)
    , FinalUpdateIdInLastStream(FinalUpdateIdInLastStream)
    , IsAsk(IsAsk)
    , Price(Price)
    , Quantity(Quantity)
    , PSUnknownField(PSUnknownField)
    , IsLast(IsLast)
    , Market_(Market_)
    , prev_(prev_)
    , next_(next_)
    {}
    DifferenceDepthEntry() = default;
};
