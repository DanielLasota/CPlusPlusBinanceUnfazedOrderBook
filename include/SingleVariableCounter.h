#pragma once

#include "enums/TradeEntry.h"
#include "OrderBook.h"

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderBook);
    double calculateBestBidPrice(const OrderBook& orderBook);
    double calculateMidPrice(const OrderBook& orderBook);
    double calculateBestVolumeImbalance(const OrderBook& orderBook);
    double calculateQueueImbalance(const OrderBook& orderBook);
    double calculateVolumeImbalance(const OrderBook& orderBook);
    double calculateGap(const OrderBook& orderBook);
    bool calculateIsAggressorAsk(const TradeEntry *trade_entry);

}
