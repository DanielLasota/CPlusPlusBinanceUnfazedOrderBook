// ===== ./include/FinalOrderBookSnapshot.h =====
#ifndef FINALORDERBOOKSNAPSHOT_H
#define FINALORDERBOOKSNAPSHOT_H

#include <vector>
#include "enums/OrderBookEntry.h"

struct FinalOrderBookSnapshot {
    std::vector<DifferenceDepthEntry> bids;
    std::vector<DifferenceDepthEntry> asks;

    void printFinalOrderBookSnapshot() const;
};

#endif // FINALORDERBOOKSNAPSHOT_H
