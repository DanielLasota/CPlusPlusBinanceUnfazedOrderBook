#include <iostream>
#include <optional>

#include "MarketState.h"
#include "enums/TradeEntry.h"
#include "OrderBookMetrics.h"
#include "SingleVariableCounter.h"

MarketState::MarketState()
    : lastTradePtr(nullptr)
    , hasLastTrade(false)
    , orderBook()
{}

void MarketState::update(DecodedEntry* entry) {
    if (auto* differenceDepthEntry = std::get_if<DifferenceDepthEntry>(entry)) {
        orderBook.addOrder(differenceDepthEntry);
        // std::cout << "received order: "<< differenceDepthEntry->Price << std::endl;
    }

    if (auto* tradeEntry = std::get_if<TradeEntry>(entry)) {
        lastTradePtr = tradeEntry;
        hasLastTrade = true;
        // std::cout << "received trade: "<< tradeEntry->Price << std::endl;
    }
}

std::optional<OrderBookMetrics> MarketState::countOrderBookMetrics() const {
    if (orderBook.asks.size() < 2
     || orderBook.bids.size() < 2
     || !hasLastTrade)
    {
        return std::nullopt;
    }

    OrderBookMetrics obm{};
    obm.bestAsk             = SingleVariableCounter::calculateBestAskPrice(orderBook);
    obm.bestBid             = SingleVariableCounter::calculateBestBidPrice(orderBook);
    obm.midPrice            = SingleVariableCounter::calculateMidPrice(orderBook);
    obm.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderBook);
    obm.queueImbalance      = SingleVariableCounter::calculateQueueImbalance(orderBook);
    obm.gap                 = SingleVariableCounter::calculateGap(orderBook);
    obm.isAggressorAsk      = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);

    return obm;
}
