#include "OrderBook.h"
#include <algorithm>
#include <iostream>
#include <ranges>

void OrderBook::addOrder(DifferenceDepthEntry* order) {
    bool isAsk = std::visit([](auto const& o) { return o.IsAsk; }, *order);
    double price = std::visit([](auto const& o) { return o.Price; }, *order);
    double quantity = std::visit([](auto const& o) { return o.Quantity; }, *order);

    if (isAsk) {
        auto cmp = [](const DifferenceDepthEntry* lhs, const DifferenceDepthEntry* rhs) {
            double lPrice = std::visit([](auto const& o) { return o.Price; }, *lhs);
            double rPrice = std::visit([](auto const& o) { return o.Price; }, *rhs);
            return lPrice < rPrice;
        };
        auto it = std::lower_bound(asks.begin(), asks.end(), order, cmp);

        if (quantity == 0.0) {
            if (it != asks.end() && std::visit([](auto const& o) { return o.Price; }, **it) == price) {
                asks.erase(it);
            }
        } else {
            if (it != asks.end() && std::visit([](auto const& o) { return o.Price; }, **it) == price) {
                *it = order;
            } else {
                asks.insert(it, order);
            }
        }
    } else {  // Dla bidów
        auto cmp = [](const DifferenceDepthEntry* lhs, const DifferenceDepthEntry* rhs) {
            double lPrice = std::visit([](auto const& o) { return o.Price; }, *lhs);
            double rPrice = std::visit([](auto const& o) { return o.Price; }, *rhs);
            return lPrice > rPrice;
        };
        auto it = std::lower_bound(bids.begin(), bids.end(), order, cmp);

        if (quantity == 0.0) {
            if (it != bids.end() && std::visit([](auto const& o) { return o.Price; }, **it) == price) {
                bids.erase(it);
            }
        } else {
            if (it != bids.end() && std::visit([](auto const& o) { return o.Price; }, **it) == price) {
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
        std::string symbol = std::visit([](auto const& o) { return o.Symbol; }, *ask);
        double price = std::visit([](auto const& o) { return o.Price; }, *ask);
        double quantity = std::visit([](auto const& o) { return o.Quantity; }, *ask);
        bool isAsk = std::visit([](auto const& o) { return o.IsAsk; }, *ask);
        std::cout << "\033[31m"
                  << "SYMBOL: " << symbol
                  << " Price: " << price
                  << " Quantity: " << quantity
                  << " IsAsk: " << (isAsk ? "true" : "false")
                  << "\033[0m" << std::endl;
    }

    std::cout << "\033[32m" << "Bids:" << "\033[0m" << std::endl;
    for (const auto* bid : bids) {
        std::string symbol = std::visit([](auto const& o) { return o.Symbol; }, *bid);
        double price = std::visit([](auto const& o) { return o.Price; }, *bid);
        double quantity = std::visit([](auto const& o) { return o.Quantity; }, *bid);
        bool isAsk = std::visit([](auto const& o) { return o.IsAsk; }, *bid);
        std::cout << "\033[32m"
                  << "SYMBOL: " << symbol
                  << " Price: " << price
                  << " Quantity: " << quantity
                  << " IsAsk: " << (isAsk ? "true" : "false")
                  << "\033[0m" << std::endl;
    }
}
