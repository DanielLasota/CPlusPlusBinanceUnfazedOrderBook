#include "GlobalMarketState.h"

GlobalMarketState::GlobalMarketState(const MetricMask& mask)
    : mask(mask), metricsCalculator(mask) {}

GlobalMarketState::GlobalMarketState(const std::vector<std::string>& variables)
    : GlobalMarketState(parseMask(variables)) {}

void GlobalMarketState::update(XEvent *e) {
    AssetKey key{*e};
    auto [it, inserted] = marketStates.try_emplace(key, key.market, key.symbol);
    it->second.update(e);
}

std::optional<MetricsEntry> GlobalMarketState::calcMetricsEntry(const XEvent* e) {
    const AssetKey key{*e};
    return metricsCalculator.calc(marketStates[key]);
}

std::optional<MetricsEntry> GlobalMarketState::calcMetricsEntryFromAp(const Symbol symbol, const Market& market) {
    const AssetKey key{market, symbol};
    const auto it = marketStates.find(key);
    if (it == marketStates.end()) { throw std::runtime_error("no specified market"); }
    return metricsCalculator.calc(it->second);
}

MarketState& GlobalMarketState::getMarketState(const Symbol symbol, const Market& market) {
    const AssetKey key{market, symbol};
    const auto it = marketStates.find(key);
    if (it == marketStates.end()) { throw std::runtime_error("no specified market"); }
    return it->second;
}

size_t GlobalMarketState::getMarketStateCount() const { return marketStates.size(); }

std::vector<std::pair<Symbol, Market>> GlobalMarketState::getMarketStateList() const {
    std::vector<std::pair<Symbol, Market>> out;
    out.reserve(marketStates.size());
    for (auto const &kv : marketStates) {
        out.emplace_back(kv.first.symbol, kv.first.market);
    }
    return out;
}
