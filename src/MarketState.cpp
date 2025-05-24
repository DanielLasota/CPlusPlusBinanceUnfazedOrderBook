#include <variant>

#include "MarketState.h"
#include "enums/TradeEntry.h"
#include "SingleVariableCounter.h"

void MarketState::update(DecodedEntry* entry) {

    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entry);

    if (auto* differenceDepthEntry = std::get_if<DifferenceDepthEntry>(entry)) {
        orderBook.update(differenceDepthEntry);
    }

    if (auto* tradeEntry = std::get_if<TradeEntry>(entry)) {
        lastTrade = *tradeEntry;
        lastTradePtr = &lastTrade;
        hasLastTrade = true;
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

void MarketState::updateTradeRegistry(int64_t timestampOfReceive, double price, double quantity, bool isBuyerMM) {
    lastTimestampOfReceive = timestampOfReceive;
    lastTrade.TimestampOfReceive = timestampOfReceive;
    lastTrade.Price              = price;
    lastTrade.Quantity           = quantity;
    lastTrade.IsBuyerMarketMaker = isBuyerMM;
    lastTradePtr = &lastTrade;
    hasLastTrade = true;
}

void MarketState::doNothing() {}
