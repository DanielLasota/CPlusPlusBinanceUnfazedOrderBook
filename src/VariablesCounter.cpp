// ===== ./src/VariablesCounter.cpp =====
#include "VariablesCounter.h"
#include <enums/TradeEntry.h>
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
    isAgressorAsk.reserve(size);
}

void VariablesCounter::update(const OrderBook& orderbook, const TradeEntry* trade) {

    if (trade) {
        lastTrade = *trade;
        hasLastTrade = true;
        // std::cout << "received trade: " << trade->Price << std::endl;
    }

    if ((orderbook.asks.size() < 2 || orderbook.bids.size() < 2) || (!trade && !hasLastTrade)) {
        return;
    }

    if (!trade) {
        trade = &lastTrade;
    }

    double bestAskPrice = SingleVariableCounter::calculateBestAskPrice(orderbook);
    double bestBidPrice = SingleVariableCounter::calculateBestBidPrice(orderbook);
    double midPriceValue = SingleVariableCounter::calculateMidPrice(orderbook);
    double bestVolumeImbalanceValue = SingleVariableCounter::calculateBestVolumeImbalance(orderbook);
    double queueImbalanceValue = SingleVariableCounter::calculateQueueImbalance(orderbook);
    // double volumeImbalanceValue = SingleVariableCounter::calculateVolumeImbalance(orderbook);
    double gapValue = SingleVariableCounter::calculateGap(orderbook);
    bool isAggressorAskValue = SingleVariableCounter::calculateIsAggressorAsk(trade);

    bestAsk.push_back(bestAskPrice);
    bestBid.push_back(bestBidPrice);
    midPrice.push_back(midPriceValue);
    bestVolumeImbalance.push_back(bestVolumeImbalanceValue);
    queueImbalance.push_back(queueImbalanceValue);
    // volumeImbalance.push_back(volumeImbalanceValue);
    gap.push_back(gapValue);
    isAgressorAsk.push_back(isAggressorAskValue);

}

void VariablesCounter::saveVariablesListToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    file << "BestAsk,BestBid,MidPrice,BestVolumeImbalance,QueueImbalance,VolumeImbalance,Gap,IsAggressorAsk\n";
    for (size_t i = 0; i < bestAsk.size(); ++i) {
        file
          << bestAsk[i] << ',' << bestBid[i] << ',' << midPrice[i] << ','
          << bestVolumeImbalance[i] << ',' << queueImbalance[i] << ','
          << volumeImbalance[i] << ',' << gap[i] << ','
          << isAgressorAsk[i] << '\n';
    }
}
