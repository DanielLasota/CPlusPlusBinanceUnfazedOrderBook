#include <fstream>
#include <iostream>

#include "OrderBookMetrics.h"


void OrderBookMetrics::toCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << path << std::endl;
        return;
    }

    file << "bestAsk,bestBid,midPrice,bestVolumeImbalance,queueImbalance,volumeImbalance,gap,isAggressorAsk\n";

    for (size_t i = 0; i < entries_.size(); ++i) {
        const auto &e = entries_[i];
        file
            << e.bestAsk                << ","
            << e.bestBid                << ","
            << e.midPrice               << ","
            << e.bestVolumeImbalance    << ","
            << e.queueImbalance         << ","
            << e.volumeImbalance        << ","
            << e.gap                    << ","
            << (e.isAggressorAsk ? "1" : "0")
            << "\n";
    }

    file.close();
}