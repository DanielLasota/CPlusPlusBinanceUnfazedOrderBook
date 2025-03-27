#include "OrderBook.h"
#include <algorithm>
#include <iostream>


void OrderBook::addOrder(const OrderBookEntry &order) {
    if (order.IsAsk) {
        auto it = std::lower_bound(asks.begin(), asks.end(), order,
            [](const OrderBookEntry &lhs, const OrderBookEntry &rhs) {
                return lhs.Price > rhs.Price;
            }
        );
        if (it != asks.end() && it->Price == order.Price) {
            *it = order;
        } else {
            asks.insert(it, order);
        }
    } else {
        auto it = std::lower_bound(bids.begin(), bids.end(), order,
            [](const OrderBookEntry &lhs, const OrderBookEntry &rhs) {
                return lhs.Price > rhs.Price;
            }
        );
        if (it != bids.end() && it->Price == order.Price) {
            *it = order;
        } else {
            bids.insert(it, order);
        }
    }
}

void OrderBook::printOrderBook() const {
    std::cout << "ORDERBOOK:" << std::endl;

    std::cout << "\033[31m" << "Asks:" << "\033[0m" << std::endl;
    for (const auto &ask : asks) {
        std::cout << "\033[31m"
                  << "Price: " << ask.Price << " Quantity: " << ask.Quantity << "IsAsk:" << ask.IsAsk
                  << "\033[0m" << std::endl;
    }

    std::cout << "\033[32m" << "Bids:" << "\033[0m" << std::endl;
    for (const auto &bid : bids) {
        std::cout << "\033[32m"
                  << "Price: " << bid.Price << " Quantity: " << bid.Quantity << " IsAsk:" << bid.IsAsk
                  << "\033[0m" << std::endl;
    }

    std::cout << "\n" << std::endl;
}
