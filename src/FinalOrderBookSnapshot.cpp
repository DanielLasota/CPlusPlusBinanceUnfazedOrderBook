#include "FinalOrderBookSnapshot.h"
#include <iostream>
#include <ranges>
#include <variant>

void FinalOrderBookSnapshot::printFinalOrderBookSnapshot() const {
    std::cout << "Final Order Book Snapshot:" << std::endl;

    std::cout << "Asks:" << std::endl;
    for (const auto& ask : std::ranges::reverse_view(asks)) {
        std::visit([](auto const& o) {
            std::cout << "Price: " << o.Price << ", Quantity: " << o.Quantity << std::endl;
        }, ask);
    }

    std::cout << "Bids:" << std::endl;
    for (const auto& bid : bids) {
        std::visit([](auto const& o) {
            std::cout << "Price: " << o.Price << ", Quantity: " << o.Quantity << std::endl;
        }, bid);
    }
}
