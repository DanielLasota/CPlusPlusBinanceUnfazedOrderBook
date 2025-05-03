#include "FinalOrderBookSnapshot.h"
#include <iostream>
#include <ranges>

void FinalDifferenceDepthSnapshot::printFinalDifferenceDepthSnapshot() const {
    std::cout << "Final Order Book Snapshot:" << std::endl;

    std::cout << "Asks:" << std::endl;
    for (const auto& ask : std::ranges::reverse_view(asks)) {
        std::cout << "Price: " << ask.Price << ", Quantity: " << ask.Quantity;
        std::cout << std::endl;
    }

    std::cout << "Bids:" << std::endl;
    for (const auto& bid : bids) {
        std::cout << "Price: " << bid.Price << ", Quantity: " << bid.Quantity;
        std::cout << std::endl;
    }

}
