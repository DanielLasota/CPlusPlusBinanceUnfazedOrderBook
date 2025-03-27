// include/OrderBookEntry.h
#ifndef ORDERBOOKENTRY_H
#define ORDERBOOKENTRY_H

#include <cstdint>
#include <string>

struct OrderBookEntry {
    int64_t TimestampOfReceive;
    int64_t EventTime;
    std::string Symbol;
    int64_t FirstUpdateId;
    int64_t FinalUpdateId;
    bool IsAsk;
    double Price;
    double Quantity;
};

#endif // ORDERBOOKENTRY_H
