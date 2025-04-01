#include "SingleVariableCounter.h"
#include "OrderBook.h"
#include <stdexcept>

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderbook) {
        if (orderbook.asks.empty()) {
            throw std::runtime_error("Orderbook has no asks");
        }
        return orderbook.asks.front()->Price;
    }

    double calculateBestBidPrice(const OrderBook& orderbook) {
        if (orderbook.bids.empty()) {
            throw std::runtime_error("Orderbook has no bids");
        }
        return orderbook.bids.front()->Price;
    }

    double calculateMidPrice(const OrderBook& orderbook) {
        double bestAskPrice = calculateBestAskPrice(orderbook);
        double bestBidPrice = calculateBestBidPrice(orderbook);
        return (bestAskPrice + bestBidPrice) / 2.0;
    }

    double calculateBestVolumeImbalance(const OrderBook& orderbook) {
        if (orderbook.asks.empty() || orderbook.bids.empty()) {
            throw std::runtime_error("Orderbook has insufficient data for imbalance calculation");
        }
        double bestAskQuantity = orderbook.asks.front()->Quantity;
        double bestBidQuantity = orderbook.bids.front()->Quantity;

        return (bestBidQuantity - bestAskQuantity) / (bestBidQuantity + bestAskQuantity);
    }

    double calculateQueueImbalance(const OrderBook& orderbook) {
        size_t bidCount = orderbook.bids.size();
        size_t askCount = orderbook.asks.size();
        if (bidCount + askCount == 0) {
            throw std::runtime_error("Orderbook has no orders");
        }
        return static_cast<double>(bidCount - askCount) / (bidCount + askCount);
    }

    double calculateVolumeImbalance(const OrderBook& orderbook) {
        double sumBid = 0.0;
        double sumAsk = 0.0;
        for (const auto* order : orderbook.bids) {
            sumBid += order->Quantity;
        }
        for (const auto* order : orderbook.asks) {
            sumAsk += order->Quantity;
        }
        double total = sumBid + sumAsk;
        if (total == 0.0) {
            throw std::runtime_error("Total volume is zero, cannot calculate volume imbalance");
        }
        return (sumBid - sumAsk) / total;
    }

    double calculateGap(const OrderBook& orderbook) {
        if (orderbook.bids.size() < 2 || orderbook.asks.size() < 2) {
            throw std::runtime_error("Insufficient data to calculate gap");
        }
        double bestBid = orderbook.bids.front()->Price;
        double bestAsk = orderbook.asks.front()->Price;
        double secondBestBid = orderbook.bids[1]->Price;
        double secondBestAsk = orderbook.asks[1]->Price;
        return (secondBestBid + secondBestAsk) - (bestBid + bestAsk);
    }

} // namespace SingleVariableCounter
