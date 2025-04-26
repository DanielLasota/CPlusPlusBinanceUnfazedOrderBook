#ifndef SINGLEVARIABLECOUNTER_H
#define SINGLEVARIABLECOUNTER_H

#include <enums/TradeEntry.h>
#include <OrderBook.h>

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderbook);
    double calculateBestBidPrice(const OrderBook& orderbook);
    double calculateMidPrice(const OrderBook& orderbook);
    double calculateBestVolumeImbalance(const OrderBook& orderbook);
    double calculateQueueImbalance(const OrderBook& orderbook);
    double calculateVolumeImbalance(const OrderBook& orderbook);
    double calculateGap(const OrderBook& orderbook);
    bool calculateIsAggressorAsk(const TradeEntry *trade_entry);

}

#endif // SINGLEVARIABLECOUNTER_H
