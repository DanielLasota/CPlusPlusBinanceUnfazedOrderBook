// ===== ./include/FinalOrderBookSnapshot.h =====
#ifndef FINALORDERBOOKSNAPSHOT_H
#define FINALORDERBOOKSNAPSHOT_H

#include <vector>
#include "enums/OrderBookEntry.h"

struct FinalOrderBookSnapshot {
    std::vector<OrderBookEntry> bids;
    std::vector<OrderBookEntry> asks;

    void printFinalOrderBookSnapshot() const;
};

#endif // FINALORDERBOOKSNAPSHOT_H
