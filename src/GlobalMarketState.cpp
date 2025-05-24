#include "GlobalMarketState.h"

GlobalMarketState::GlobalMarketState(MetricMask mask)
    : mask_(mask), calculator_(mask) {}

void GlobalMarketState::update(DecodedEntry* entry) {
    AssetKey key = AssetKey(*entry);
    auto [it, inserted] = marketStates_.try_emplace(key, key.market, key.symbol);
    it->second.update(entry);
}

std::optional<OrderBookMetricsEntry> GlobalMarketState::countMarketStateMetricsByEntry(DecodedEntry* entry) {
    AssetKey key = AssetKey(*entry);
    return calculator_.countMarketStateMetrics(marketStates_[key]);
}

std::optional<OrderBookMetricsEntry> GlobalMarketState::countMarketStateMetrics(const std::string& symbol, const Market& market) {
    AssetKey key{ market, symbol };
    auto it = marketStates_.find(key);
    if (it == marketStates_.end()) {
        throw std::runtime_error("no specified market");
    }
    return calculator_.countMarketStateMetrics(it->second);
}

MarketState& GlobalMarketState::getMarketState(const std::string& symbol, const Market& market) {
    AssetKey key{ market, symbol };
    auto it = marketStates_.find(key);
    if (it == marketStates_.end()) {
        throw std::runtime_error("no specified market");
    }
    return it->second;
}

size_t GlobalMarketState::getMarketStateCount() const {
    return marketStates_.size();
}

std::vector<std::pair<std::string, Market>> GlobalMarketState::getMarketStateList() const {
    std::vector<std::pair<std::string, Market>> out;
    out.reserve(marketStates_.size());
    for (auto const& kv : marketStates_) {
        out.emplace_back(kv.first.symbol, kv.first.market);
    }
    return out;
}
