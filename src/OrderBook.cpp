#include "OrderBook.h"
#include <algorithm>
#include <iostream>
#include <ranges>

void OrderBook::addOrder(DifferenceDepthEntry* order) {
    if (order->IsAsk) {
        auto it = std::lower_bound(asks.begin(), asks.end(), order,
            [](const DifferenceDepthEntry* lhs, const DifferenceDepthEntry* rhs) {
                return lhs->Price < rhs->Price;
            }
        );
        if (order->Quantity == 0) {
            if (it != asks.end() && (*it)->Price == order->Price) {
                asks.erase(it);
            }
        } else {
            if (it != asks.end() && (*it)->Price == order->Price) {
                *it = order;
            } else {
                asks.insert(it, order);
            }
        }
    } else {
        auto it = std::lower_bound(bids.begin(), bids.end(), order,
            [](const DifferenceDepthEntry* lhs, const DifferenceDepthEntry* rhs) {
                return lhs->Price > rhs->Price;
            }
        );
        if (order->Quantity == 0) {
            if (it != bids.end() && (*it)->Price == order->Price) {
                bids.erase(it);
            }
        } else {
            if (it != bids.end() && (*it)->Price == order->Price) {
                *it = order;
            } else {
                bids.insert(it, order);
            }
        }
    }
}

void OrderBook::printOrderBook() const {
    std::cout << "ORDERBOOK:" << std::endl;

    std::cout << "\033[31m" << "Asks (odwrotnie):" << "\033[0m" << std::endl;
    for (const auto* ask : std::ranges::reverse_view(asks)) {
        std::cout << "\033[31m"
                  << "SYMBOL: " << ask->Symbol << " Price: " << ask->Price
                  << " Quantity: " << ask->Quantity << " IsAsk: " << ask->IsAsk
                  << "\033[0m" << std::endl;
    }

    std::cout << "\033[32m" << "Bids:" << "\033[0m" << std::endl;
    for (const auto* bid : bids) {
        std::cout << "\033[32m"
                  << "SYMBOL: " << bid->Symbol << " Price: " << bid->Price << " Quantity: " << bid->Quantity << " IsAsk:" << bid->IsAsk
                  << "\033[0m" << std::endl;
    }

    std::cout << "\n" << std::endl;
}
