#include <optional>
#include <variant>
#include "MarketState.h"
#include "enums/TradeEntry.h"
#include "OrderBookMetrics.h"
#include "SingleVariableCounter.h"
#include "MetricMask.h"

MarketState::MarketState() = default;

void MarketState::update(DecodedEntry* entry) {

    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entry);

    if (auto* differenceDepthEntry = std::get_if<DifferenceDepthEntry>(entry)) {
        orderBook.update(differenceDepthEntry);
        // std::cout << "received order: "<< differenceDepthEntry->Price << std::endl;
    }

    if (auto* tradeEntry = std::get_if<TradeEntry>(entry)) {
        lastTradePtr = tradeEntry;
        hasLastTrade = true;
        // std::cout << "received trade: "<< tradeEntry->Price << std::endl;
    }
}

void MarketState::updateOrderBook(int64_t timestampOfReceive, double price, double quantity, bool isAsk){
    lastTimestampOfReceive = timestampOfReceive;
    DifferenceDepthEntry e;
    e.TimestampOfReceive = timestampOfReceive;
    e.Price              = price;
    e.Quantity           = quantity;
    e.IsAsk              = isAsk;
    orderBook.update(&e);
}

void MarketState::updateTradeRegister(int64_t timestampOfReceive, double price, double quantity, bool isBuyerMM) {
    lastTimestampOfReceive = timestampOfReceive;
    lastTradeStorage.TimestampOfReceive = timestampOfReceive;
    lastTradeStorage.Price              = price;
    lastTradeStorage.Quantity           = quantity;
    lastTradeStorage.IsBuyerMarketMaker = isBuyerMM;
    lastTradePtr = &lastTradeStorage;
    hasLastTrade = true;
}

std::optional<OrderBookMetricsEntry>MarketState::countOrderBookMetrics(MetricMask mask) const {
    if (!hasLastTrade ||
        orderBook.askCount() < 2 ||
        orderBook.bidCount() < 2)
        return std::nullopt;

    OrderBookMetricsEntry o{};
    if (mask & TimestampOfReceive) {
        o.timestampOfReceive = lastTimestampOfReceive;
    }
    if (mask & BestAsk) {
        o.bestAskPrice = SingleVariableCounter::calculateBestAskPrice(orderBook);
    }
    if (mask & BestBid) {
        o.bestBidPrice = SingleVariableCounter::calculateBestBidPrice(orderBook);
    }
    if (mask & MidPrice) {
        o.midPrice = SingleVariableCounter::calculateMidPrice(orderBook);
    }
    if (mask & BestVolumeImbalance) {
        o.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderBook);
    }
    if (mask & QueueImbalance) {
        o.queueImbalance = SingleVariableCounter::calculateQueueImbalance(orderBook);
    }
    if (mask & VolumeImbalance) {
        o.volumeImbalance = SingleVariableCounter::calculateVolumeImbalance(orderBook);
    }
    if (mask & Gap) {
        o.gap = SingleVariableCounter::calculateGap(orderBook);
    }
    if (mask & IsAggressorAsk) {
        o.isAggressorAsk = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);
    }
    return o;
}

void MarketState::doNothing() {}
