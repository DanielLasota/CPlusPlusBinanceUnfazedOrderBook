#pragma once

#include <vector>

#include "enums/DifferenceDepthEntry.h"


class OrderBook {
public:
    std::vector<DifferenceDepthEntry*> asks;
    std::vector<DifferenceDepthEntry*> bids;

    void addOrder(DifferenceDepthEntry* order);
    void printOrderBook() const;
};
