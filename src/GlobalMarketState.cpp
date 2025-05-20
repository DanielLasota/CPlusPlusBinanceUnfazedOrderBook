#include <variant>

#include "GlobalMarketState.h"

GlobalMarketState::GlobalMarketState(MetricMask mask)
    : mask_(mask), calculator_(mask)
{}

void GlobalMarketState::update(DecodedEntry* entry) {
    AssetKey key = extractKey(*entry);
    marketStates_[key].update(entry);
}

std::optional<OrderBookMetricsEntry> GlobalMarketState::countMarketStateMetricsByEntry(DecodedEntry* entry) {
    AssetKey key = extractKey(*entry);
    return calculator_.countMarketStateMetrics(marketStates_[key]);
}

std::optional<OrderBookMetricsEntry> GlobalMarketState::countMarketStateMetrics(const std::string& symbol, const Market& market) {
    AssetKey key{ market, symbol };
    return calculator_.countMarketStateMetrics(marketStates_[key]);
}

MarketState& GlobalMarketState::getMarketState(const std::string& symbol, const std::string& market_str) {
    Market market;
    if (market_str == "SPOT") {
        market = Market::SPOT;
    }
    else if (market_str == "USD_M_FUTURES") {
        market = Market::USD_M_FUTURES;
    }
    else if (market_str == "COIN_M_FUTURES") {
        market = Market::COIN_M_FUTURES;
    }
    else {
        throw std::invalid_argument("Unknown market: " + market_str);
    }

    AssetKey key{ market, symbol };
    return marketStates_[key];
}

AssetKey GlobalMarketState::extractKey(const DecodedEntry& d) const {
    if (auto e = std::get_if<DifferenceDepthEntry>(&d)){
        return AssetKey{ e->Market_, e->Symbol };
    }

    else if (auto e = std::get_if<TradeEntry>(&d)){
        return AssetKey{ e->Market_, e->Symbol };
    }
    else
        throw std::runtime_error("Unknown DecodedEntry variant");
}
