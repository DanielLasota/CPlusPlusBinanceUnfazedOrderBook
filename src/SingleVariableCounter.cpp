#include "SingleVariableCounter.h"
#include "OrderBook.h"

inline double round2(double x) {
    double y = x * 100.0;
    y += (y >= 0.0 ?  0.5 : -0.5);
    int t = static_cast<int>(y);
    return t * 0.01;
}

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
        return round2(
            (b - a) / (b + a)
            );
    }

    double calculateQueueImbalance(const OrderBook& orderBook) {
        return round2(
            static_cast<double>(orderBook.bidCount() - orderBook.askCount())
            / static_cast<double>(orderBook.bidCount() + orderBook.askCount())
            );
    }

    double calculateVolumeImbalance(const OrderBook& orderBook) {
        return round2(
            (orderBook.sumBidQuantity() - orderBook.sumAskQuantity())
            / (orderBook.sumBidQuantity() + orderBook.sumAskQuantity())
            );
    }

    double calculateGap(const OrderBook& orderBook) {
        const double bestBidPrice = orderBook.bestBidPrice();
        const double bestAskPrice = orderBook.bestAskPrice();
        const double secondBidPrice = orderBook.secondBidPrice();
        const double secondAskPrice = orderBook.secondAskPrice();
        return round2(
            secondBidPrice + secondAskPrice - bestBidPrice - bestAskPrice
            );
    }

    bool calculateIsAggressorAsk(const TradeEntry *trade_entry) {
        return trade_entry->IsBuyerMarketMaker;
    }

}
