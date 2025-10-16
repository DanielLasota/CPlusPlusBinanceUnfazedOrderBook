#include <variant>

#include "MarketState.h"
#include "enums/TradeEntry.h"
#include "SingleVariableCounter.h"

void MarketState::update(DecodedEntry* entry) {

    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.timestampOfReceive;
    }, *entry);

    if (auto* differenceDepthEntry = std::get_if<DifferenceDepthEntry>(entry)) {
        orderBook.update(differenceDepthEntry);
        rollingDifferenceDepthStatistics.update(*differenceDepthEntry);
        lastStreamType = StreamType::DIFFERENCE_DEPTH_STREAM;
    }

    if (const auto* tradeEntry = std::get_if<TradeEntry>(entry)) {
        lastTrade = *tradeEntry;
        lastTradePtr = &lastTrade;
        hasLastTrade = true;
        rollingTradeStatistics.update(*tradeEntry);
        lastStreamType = StreamType::TRADE_STREAM;
    }
}

void MarketState::updateOrderBook(const uint64_t timestampOfReceive, const double price, const double quantity, const bool isAsk){
    lastTimestampOfReceive = timestampOfReceive;
    DifferenceDepthEntry e;
    e.timestampOfReceive = timestampOfReceive;
    e.price              = price;
    e.quantity           = quantity;
    e.isAsk              = isAsk;
    orderBook.update(&e);
}

void MarketState::updateTradeRegistry(const uint64_t timestampOfReceive, const double price, const double quantity, const bool isBuyerMM) {
    lastTimestampOfReceive = timestampOfReceive;
    lastTrade.timestampOfReceive = timestampOfReceive;
    lastTrade.price              = price;
    lastTrade.quantity           = quantity;
    lastTrade.isBuyerMarketMaker = isBuyerMM;
    lastTradePtr = &lastTrade;
    hasLastTrade = true;
}
