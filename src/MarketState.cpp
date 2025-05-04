// ===== src/MarketState.cpp =====
#include "MarketState.h"
#include "enums/TradeEntry.h"
#include "SingleVariableCounter.h"
#include "MetricMask.h"

#include <variant>
#include <optional>

MarketState::MarketState()
  : lastTradePtr(nullptr)
  , hasLastTrade(false)
  , orderBook(std::make_unique<OrderBook>())
  , lastTimestampOfReceive(0)
{}

void MarketState::update(DecodedEntry* entry) {
    // Zaktualizuj timestamp
    std::visit([this](auto const& e){
        lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entry);

    // Jeśli to event booku – dorzuć do silnika
    if (auto* diff = std::get_if<DifferenceDepthEntry>(entry)) {
        orderBook->addOrder(*diff);
    }

    // Jeśli to trade – zapamiętaj do metryki IsAggressorAsk
    if (auto* trade = std::get_if<TradeEntry>(entry)) {
        lastTradePtr = trade;
        hasLastTrade = true;
    }
}

std::optional<OrderBookMetricsEntry> MarketState::countOrderBookMetrics(MetricMask mask) const {
    // Pobierz snapshot obydwu stron
    auto bids = orderBook->getBids();
    auto asks = orderBook->getAsks();

    // Musimy mieć co najmniej po 2 poziomy i µostatni trade
    if (bids.size() < 2 || asks.size() < 2 || !hasLastTrade)
        return std::nullopt;

    OrderBookMetricsEntry o{};

    if (mask & TimestampOfReceive)
        o.timestampOfReceive    = lastTimestampOfReceive;
    if (mask & BestAsk)
        o.bestAsk               = SingleVariableCounter::calculateBestAskPrice(*orderBook);
    if (mask & BestBid)
        o.bestBid               = SingleVariableCounter::calculateBestBidPrice(*orderBook);
    if (mask & MidPrice)
        o.midPrice              = SingleVariableCounter::calculateMidPrice(*orderBook);
    if (mask & BestVolumeImbalance)
        o.bestVolumeImbalance   = SingleVariableCounter::calculateBestVolumeImbalance(*orderBook);
    if (mask & QueueImbalance)
        o.queueImbalance        = SingleVariableCounter::calculateQueueImbalance(*orderBook);
    if (mask & VolumeImbalance)
        o.volumeImbalance       = SingleVariableCounter::calculateVolumeImbalance(*orderBook);
    if (mask & Gap)
        o.gap                   = SingleVariableCounter::calculateGap(*orderBook);
    if (mask & IsAggressorAsk)
        o.isAggressorAsk        = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);

    return o;
}
