// ===== ./src/VariablesCounter.cpp =====
#include "VariablesCounter.h"
#include "OrderBook.h"
#include <SingleVariableCounter.h>

#include <fstream>
#include <iostream>

VariablesCounter::VariablesCounter(size_t expectedSize) {
    reserveMemory(expectedSize);
}

void VariablesCounter::reserveMemory(size_t size) {
    bestAsk.reserve(size);
    bestBid.reserve(size);
    midPrice.reserve(size);
    bestVolumeImbalance.reserve(size);
    queueImbalance.reserve(size);
    volumeImbalance.reserve(size);
    gap.reserve(size);
}

void VariablesCounter::update(const OrderBook& orderbook) {
    if (orderbook.asks.size() < 2 || orderbook.bids.size() < 2) {
        return;
    }

    double bestAskPrice = SingleVariableCounter::calculateBestAskPrice(orderbook);
    double bestBidPrice = SingleVariableCounter::calculateBestBidPrice(orderbook);
    double midPriceValue = SingleVariableCounter::calculateMidPrice(orderbook);

    double bestVolumeImbalanceValue = SingleVariableCounter::calculateBestVolumeImbalance(orderbook);
    double queueImbalanceValue = SingleVariableCounter::calculateQueueImbalance(orderbook);
    double volumeImbalanceValue = SingleVariableCounter::calculateVolumeImbalance(orderbook);

    double gapValue = SingleVariableCounter::calculateGap(orderbook);

    bestAsk.push_back(bestAskPrice);
    bestBid.push_back(bestBidPrice);
    midPrice.push_back(midPriceValue);
    bestVolumeImbalance.push_back(bestVolumeImbalanceValue);
    queueImbalance.push_back(queueImbalanceValue);
    volumeImbalance.push_back(volumeImbalanceValue);
    gap.push_back(gapValue);

    // std::cout
    // << "bestAskPrice " << bestAskPrice
    // << "\nbestBidPrice " << bestBidPrice
    // << "\nmidPriceValue " << midPriceValue
    // << "\nbestVolumeImbalanceValue " << bestVolumeImbalanceValue
    // << "\nqueueImbalanceValue " << queueImbalanceValue
    // << "\nvolumeImbalanceValue " << volumeImbalanceValue
    // << "\ngapValue " << gapValue
    //
    // << std::endl;
}

void VariablesCounter::saveVariablesListToCSV(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << filename << std::endl;
        return;
    }

    file << "BestAsk,BestBid,MidPrice,BestVolumeImbalance,QueueImbalance,VolumeImbalance,Gap\n";

    for (size_t i = 0; i < bestAsk.size(); ++i) {
        file << bestAsk[i] << "," << bestBid[i] << "," << midPrice[i] << ","
             << bestVolumeImbalance[i] << "," << queueImbalance[i] << ","
             << volumeImbalance[i] << "," << gap[i] << "\n";
    }

    file.close();
}
