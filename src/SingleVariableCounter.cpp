#include "SingleVariableCounter.h"
#include "OrderBook.h"
#include <stdexcept>
#include <variant>

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderbook) {
        if (orderbook.asks.empty()) {
            throw std::runtime_error("Orderbook has no asks");
        }
        // Używamy std::visit, aby pobrać wartość Price
        return std::visit([](auto const& o) { return o.Price; }, *(orderbook.asks.front()));
    }

    double calculateBestBidPrice(const OrderBook& orderbook) {
        if (orderbook.bids.empty()) {
            throw std::runtime_error("Orderbook has no bids");
        }
        return std::visit([](auto const& o) { return o.Price; }, *(orderbook.bids.front()));
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
        double bestAskQuantity = std::visit([](auto const& o) { return o.Quantity; }, *(orderbook.asks.front()));
        double bestBidQuantity = std::visit([](auto const& o) { return o.Quantity; }, *(orderbook.bids.front()));

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
            sumBid += std::visit([](auto const& o) { return o.Quantity; }, *order);
        }
        for (const auto* order : orderbook.asks) {
            sumAsk += std::visit([](auto const& o) { return o.Quantity; }, *order);
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
        double bestBid = std::visit([](auto const& o){ return o.Price; }, *(orderbook.bids.front()));
        double bestAsk = std::visit([](auto const& o){ return o.Price; }, *(orderbook.asks.front()));
        double secondBestBid = std::visit([](auto const& o){ return o.Price; }, *(orderbook.bids[1]));
        double secondBestAsk = std::visit([](auto const& o){ return o.Price; }, *(orderbook.asks[1]));
        return (secondBestBid + secondBestAsk) - (bestBid + bestAsk);
    }

} // namespace SingleVariableCounter