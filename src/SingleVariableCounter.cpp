#include "SingleVariableCounter.h"
#include "OrderBook.h"

namespace SingleVariableCounter {

    double calculateBestAskPrice(const OrderBook& orderBook) {
        return orderBook.bestAskPrice();
    }

    double calculateBestBidPrice(const OrderBook& orderBook) {
        return orderBook.bestBidPrice();
    }

    double calculateMidPrice(const OrderBook& orderBook) {
        return (orderBook.bestBidPrice() + orderBook.bestAskPrice()) * 0.5;
    }

    double calculateBestVolumeImbalance(const OrderBook& orderBook) {
        const double a = orderBook.bestAskQuantity();
        const double b = orderBook.bestBidQuantity();
        return (b - a) / (b + a);
    }

    double calculateQueueImbalance(const OrderBook& orderBook) {
        return static_cast<double>(orderBook.bidCount() - orderBook.askCount()) / static_cast<double>(orderBook.bidCount() + orderBook.askCount());
    }

    double calculateVolumeImbalance(const OrderBook& orderBook) {
        return (orderBook.sumBidQuantity() - orderBook.sumAskQuantity()) / (orderBook.sumBidQuantity() + orderBook.sumAskQuantity());
    }

    double calculateGap(const OrderBook& orderBook) {
        const double bestBidPrice = orderBook.bestBidPrice();
        const double bestAskPrice = orderBook.bestAskPrice();
        const double secondBidPrice = orderBook.secondBidPrice();
        const double secondAskPrice = orderBook.secondAskPrice();
        return (secondBidPrice + secondAskPrice) - (bestBidPrice + bestAskPrice);
    }

    bool calculateIsAggressorAsk(const TradeEntry *trade_entry) {
        return trade_entry->IsBuyerMarketMaker;
    }

} // namespace SingleVariableCounter
