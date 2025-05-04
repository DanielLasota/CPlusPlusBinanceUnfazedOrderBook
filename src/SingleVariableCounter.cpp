// ===== src/SingleVariableCounter.cpp =====
#include "SingleVariableCounter.h"
#include "OrderBook.h"
#include <stdexcept>
#include <vector>

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderbook) {
        auto asks = orderbook.getAsks();
        if (asks.empty()) {
            throw std::runtime_error("Orderbook has no asks");
        }
        return asks.front()->Price;
    }

    double calculateBestBidPrice(const OrderBook& orderbook) {
        auto bids = orderbook.getBids();
        if (bids.empty()) {
            throw std::runtime_error("Orderbook has no bids");
        }
        return bids.front()->Price;
    }

    double calculateMidPrice(const OrderBook& orderbook) {
        double bestAsk = calculateBestAskPrice(orderbook);
        double bestBid = calculateBestBidPrice(orderbook);
        return (bestAsk + bestBid) / 2.0;
    }

    double calculateBestVolumeImbalance(const OrderBook& orderbook) {
        auto asks = orderbook.getAsks();
        auto bids = orderbook.getBids();
        if (asks.empty() || bids.empty()) {
            throw std::runtime_error("Orderbook has insufficient data for imbalance calculation");
        }
        double bestAskQty = asks.front()->Quantity;
        double bestBidQty = bids.front()->Quantity;
        return (bestBidQty - bestAskQty) / (bestBidQty + bestAskQty);
    }

    double calculateQueueImbalance(const OrderBook& orderbook) {
        auto asks = orderbook.getAsks();
        auto bids = orderbook.getBids();
        size_t askCount = asks.size();
        size_t bidCount = bids.size();
        if (askCount + bidCount == 0) {
            throw std::runtime_error("Orderbook has no orders");
        }
        return double(bidCount - askCount) / double(bidCount + askCount);
    }

    double calculateVolumeImbalance(const OrderBook& orderbook) {
        auto asks = orderbook.getAsks();
        auto bids = orderbook.getBids();
        double sumAsk = 0, sumBid = 0;
        for (auto *e : asks) sumAsk += e->Quantity;
        for (auto *e : bids) sumBid += e->Quantity;
        double total = sumAsk + sumBid;
        if (total == 0.0) {
            throw std::runtime_error("Total volume is zero");
        }
        return (sumBid - sumAsk) / total;
    }

    double calculateGap(const OrderBook& orderbook) {
        auto asks = orderbook.getAsks();
        auto bids = orderbook.getBids();
        if (asks.size() < 2 || bids.size() < 2) {
            throw std::runtime_error("Insufficient data to calculate gap");
        }
        double bestAsk       = asks.front()->Price;
        double secondAsk     = asks[1]->Price;
        double bestBid       = bids.front()->Price;
        double secondBid     = bids[1]->Price;
        return (secondBid + secondAsk) - (bestBid + bestAsk);
    }

    bool calculateIsAggressorAsk(const TradeEntry *trade_entry) {
        return trade_entry->IsBuyerMarketMaker;
    }

} // namespace SingleVariableCounter
