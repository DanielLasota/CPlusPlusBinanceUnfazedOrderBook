#include <cmath>

#include "SingleVariableCounter.h"
#include "OrderBook.h"

inline double round2(double x) {
    double y = x * 100.0;
    y += (y >= 0.0 ?  0.5 : -0.5);
    int t = static_cast<int>(y);
    return t * 0.01;
}

inline double round8(double x) {
    double p = 100000000.0; // 8
    return std::round(x * p) / p;
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

    double calculateBestVolumeRatio(const OrderBook& orderBook) {
        const double a = orderBook.bestAskQuantity();
        const double b = orderBook.bestBidQuantity();
        return round2(b/a);
    }

    double calculateBestTwoVolumeImbalance(const OrderBook& ob) {
        double sumB = ob.sumTopBidQuantity(2);
        double sumA = ob.sumTopAskQuantity(2);
        if (sumB + sumA == 0.0) return 0.0;
        return round2((sumB - sumA) / (sumB + sumA));
    }

    double calculateBestThreeVolumeImbalance(const OrderBook& ob) {
        double sumB = ob.sumTopBidQuantity(3);
        double sumA = ob.sumTopAskQuantity(3);
        if (sumB + sumA == 0.0) return 0.0;
        return round2((sumB - sumA) / (sumB + sumA));
    }

    double calculateBestFiveVolumeImbalance(const OrderBook& ob) {
        double sumB = ob.sumTopBidQuantity(5);
        double sumA = ob.sumTopAskQuantity(5);
        if (sumB + sumA == 0.0) return 0.0;
        return round2((sumB - sumA) / (sumB + sumA));
    }

    double calculateVolumeImbalance(const OrderBook& orderBook) {
        return round2(
            (orderBook.sumBidQuantity() - orderBook.sumAskQuantity())
            / (orderBook.sumBidQuantity() + orderBook.sumAskQuantity())
            );
    }

    double calculateQueueImbalance(const OrderBook& orderBook) {
        return round2(
            static_cast<double>(orderBook.bidCount() - orderBook.askCount())
            / static_cast<double>(orderBook.bidCount() + orderBook.askCount())
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
        return trade_entry->isBuyerMarketMaker;
    }

    double calculateVwapDeviation(const OrderBook& ob) {
        const double sumQ  = ob.sumAskQuantity() + ob.sumBidQuantity();
        if (sumQ == 0.0) return 0.0;
        const double vwap  = ob.sumPriceQty() / sumQ;
        const double mid   = calculateMidPrice(ob);
        return (vwap - mid) / mid;
    }

}
