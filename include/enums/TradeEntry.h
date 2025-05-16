#pragma once

#include <cstdint>
#include <string>

#include "enums/Market.h"

struct TradeEntry {
    int64_t TimestampOfReceive;
    std::string Stream;
    std::string EventType;
    int64_t EventTime;
    int64_t TransactionTime;
    std::string Symbol;
    int64_t TradeId;
    double Price;
    double Quantity;
    bool IsBuyerMarketMaker;
    std::string MUnknownParameter;
    std::string XUnknownParameter;
    bool IsLast;
    Market market_;
};
