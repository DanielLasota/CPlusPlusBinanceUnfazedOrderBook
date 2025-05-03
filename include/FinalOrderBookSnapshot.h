#pragma once

#include <vector>
#include "enums/OrderBookEntry.h"

struct FinalDifferenceDepthSnapshot {
    std::vector<DifferenceDepthEntry> bids;
    std::vector<DifferenceDepthEntry> asks;

    void printFinalDifferenceDepthSnapshot() const;
};
