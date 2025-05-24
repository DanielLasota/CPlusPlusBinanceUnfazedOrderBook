#include <fstream>
#include <iostream>

#include "OrderBookMetrics.h"


void OrderBookMetrics::toCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << path << std::endl;
        return;
    }

    for (size_t i = 0; i < variables_.size(); ++i) {
        file << variables_[i];
        if (i + 1 < variables_.size()) file << ",";
    }
    file << "\n";

    for (const auto &e : entries_) {
        for (size_t j = 0; j < variables_.size(); ++j) {
            const auto &var = variables_[j];
            if      (var == "timestampOfReceive")  file << e.timestampOfReceive;
            else if (var == "market")              file << e.market;
            else if (var == "symbol")              file << e.symbol;
            else if (var == "bestAskPrice")        file << e.bestAskPrice;
            else if (var == "bestBidPrice")        file << e.bestBidPrice;
            else if (var == "midPrice")            file << e.midPrice;
            else if (var == "bestVolumeImbalance") file << e.bestVolumeImbalance;
            else if (var == "queueImbalance")      file << e.queueImbalance;
            else if (var == "volumeImbalance")     file << e.volumeImbalance;
            else if (var == "gap")                 file << e.gap;
            else if (var == "isAggressorAsk")      file << (e.isAggressorAsk ? "1" : "0");

            if (j + 1 < variables_.size()) file << ",";
        }
        file << "\n";
    }

    file.close();
}