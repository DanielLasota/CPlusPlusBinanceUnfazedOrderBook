#include "SingleVariableCounter.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"
#include <cassert>
#include <cmath>
#include <iostream>

void test_calculateBestAskPrice() {
    OrderBook orderbook;
    OrderBookEntry ask1, ask2, ask3;

    ask1.Price = 100.0; ask1.Quantity = 10.0; ask1.IsAsk = true;
    ask2.Price = 90.0; ask2.Quantity = 10.0; ask2.IsAsk = true;
    ask3.Price = 110.0; ask3.Quantity = 10.0; ask3.IsAsk = true;

    orderbook.addOrder(&ask1);
    orderbook.addOrder(&ask2);
    orderbook.addOrder(&ask3);

    double bestAsk = SingleVariableCounter::calculateBestAskPrice(orderbook);
    assert(bestAsk == 90.0);
    std::cout << "test_calculateBestAskPrice passed." << std::endl;
}

void test_calculateBestBidPrice() {
    OrderBook orderbook;
    OrderBookEntry bid1, bid2, bid3;

    bid1.Price = 100.0; bid1.Quantity = 10.0; bid1.IsAsk = false;
    bid2.Price = 90.0; bid2.Quantity = 10.0; bid2.IsAsk = false;
    bid3.Price = 110.0; bid3.Quantity = 10.0; bid3.IsAsk = false;

    orderbook.addOrder(&bid1);
    orderbook.addOrder(&bid2);
    orderbook.addOrder(&bid3);

    double bestBid = SingleVariableCounter::calculateBestBidPrice(orderbook);
    assert(bestBid == 110.0);
    std::cout << "test_calculateBestBidPrice passed." << std::endl;
}

void test_calculateMidPrice() {
    OrderBook orderbook;
    OrderBookEntry ask, bid;

    ask.Price = 100.0; ask.Quantity = 10.0; ask.IsAsk = true;
    bid.Price = 90.0; bid.Quantity = 5.0; bid.IsAsk = false;

    orderbook.addOrder(&ask);
    orderbook.addOrder(&bid);

    double midPrice = SingleVariableCounter::calculateMidPrice(orderbook);
    assert(midPrice == 95.0);
    std::cout << "test_calculateMidPrice passed." << std::endl;
}

void test_calculateQueueImbalance() {
    OrderBook orderbook;
    OrderBookEntry bid1, bid2, ask;

    bid1.Price = 100.0; bid1.Quantity = 5.0; bid1.IsAsk = false;
    bid2.Price = 98.0; bid2.Quantity = 3.0; bid2.IsAsk = false;
    ask.Price = 105.0; ask.Quantity = 10.0; ask.IsAsk = true;

    orderbook.addOrder(&bid1);
    orderbook.addOrder(&bid2);
    orderbook.addOrder(&ask);

    double qi = SingleVariableCounter::calculateQueueImbalance(orderbook);
    assert(std::fabs(qi - 0.3333333) < 1e-6);
    std::cout << "test_calculateQueueImbalance passed." << std::endl;
}

void test_calculateVolumeImbalance() {
    OrderBook orderbook;

    OrderBookEntry bid1, bid2, ask;

    bid1.Price = 100.0; bid1.Quantity = 5.0; bid1.IsAsk = false;
    bid2.Price = 98.0; bid2.Quantity = 3.0; bid2.IsAsk = false;
    ask.Price = 105.0; ask.Quantity = 10.0; ask.IsAsk = true;

    orderbook.addOrder(&bid1);
    orderbook.addOrder(&bid2);
    orderbook.addOrder(&ask);

    double vi = SingleVariableCounter::calculateVolumeImbalance(orderbook);
    assert(std::fabs(vi - (-0.1111111)) < 1e-6);
    std::cout << "test_calculateVolumeImbalance passed." << std::endl;
}

void test_calculateGap() {
    OrderBook orderbook;

    OrderBookEntry bid1, bid2, ask1, ask2;
    bid1.Price = 100.0; bid1.Quantity = 5.0; bid1.IsAsk = false;
    bid2.Price = 98.0;  bid2.Quantity = 3.0; bid2.IsAsk = false;
    ask1.Price = 105.0; ask1.Quantity = 10.0; ask1.IsAsk = true;
    ask2.Price = 108.0; ask2.Quantity = 6.0;  ask2.IsAsk = true;

    orderbook.addOrder(&bid1);
    orderbook.addOrder(&bid2);
    orderbook.addOrder(&ask1);
    orderbook.addOrder(&ask2);

    double gap = SingleVariableCounter::calculateGap(orderbook);
    assert(std::fabs(gap - 1.0) < 1e-6);
    std::cout << "test_calculateGap passed." << std::endl;
}

void test_calculateBestVolumeImbalance() {
    OrderBook orderbook;
    OrderBookEntry bid, ask;
    bid.Price = 100.0; bid.Quantity = 5.0; bid.IsAsk = false;
    ask.Price = 105.0; ask.Quantity = 10.0; ask.IsAsk = true;
    orderbook.addOrder(&bid);
    orderbook.addOrder(&ask);

    double bvi = SingleVariableCounter::calculateBestVolumeImbalance(orderbook);
    assert(std::fabs(bvi - (-0.3333333)) < 1e-6);
    std::cout << "test_calculateBestVolumeImbalance passed." << std::endl;
}

int main() {
    test_calculateBestAskPrice();
    test_calculateBestBidPrice();
    test_calculateMidPrice();
    test_calculateBestVolumeImbalance();
    test_calculateQueueImbalance();
    test_calculateVolumeImbalance();
    test_calculateGap();

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}
