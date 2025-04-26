#include "MarketState.h"
#include <enums/TradeEntry.h>
#include <SingleVariableCounter.h>

#include <optional>
#include <OrderBookMetrics.h>

MarketState::MarketState() {}

std::optional<OrderBookMetrics> MarketState::update(const OrderBook& orderbook, const TradeEntry* trade) {

    if (trade) {
        lastTrade = *trade;
        hasLastTrade = true;
        // std::cout << "received trade: " << trade->Price << std::endl;
    }

    if ((orderbook.asks.size() < 2 || orderbook.bids.size() < 2) || (!trade && !hasLastTrade)) {
        return std::nullopt;
    }

    if (!trade) {
        trade = &lastTrade;
    }

    OrderBookMetrics obm{};

    obm.bestAsk = SingleVariableCounter::calculateBestAskPrice(orderbook);
    obm.bestBid = SingleVariableCounter::calculateBestBidPrice(orderbook);
    obm.midPrice = SingleVariableCounter::calculateMidPrice(orderbook);
    obm.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderbook);
    obm.queueImbalance = SingleVariableCounter::calculateQueueImbalance(orderbook);
    // obm.volumeImbalance = SingleVariableCounter::calculateVolumeImbalance(orderbook);
    obm.gap = SingleVariableCounter::calculateGap(orderbook);
    obm.isAggressorAsk = SingleVariableCounter::calculateIsAggressorAsk(trade);
    return obm;
}
