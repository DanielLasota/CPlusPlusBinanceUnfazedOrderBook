#include "GlobalMarketState.h"

GlobalMarketState::GlobalMarketState(const MetricMask& mask, const ResampleConfig& resampleCfg)
    : mask(mask)
    , metricsCalculator(mask)
    , resampleCfg(resampleCfg) {}

GlobalMarketState::GlobalMarketState(const std::vector<std::string>& variables, const ResampleConfig& resampleCfg)
    : GlobalMarketState(parseMask(variables), resampleCfg) {}

void GlobalMarketState::update(XEvent* e) {
    const AssetKey key{*e};
    auto [it, inserted] = marketStates.try_emplace(key, key.market, key.symbol);

    if (resampleCfg.method != ResampleMethod::NONE) resamplers.try_emplace(key, resampleCfg);

    it->second.update(e);
}

std::optional<MetricsEntry> GlobalMarketState::calcMetricsEntry(const XEvent* e) {
    const AssetKey key{*e};

    const auto ms = marketStates.find(key);
    if (ms == marketStates.end()) return std::nullopt;

    const std::optional<MetricsEntry> me = metricsCalculator.calc(ms->second);
    if (!me) return std::nullopt;

    if (resampleCfg.method == ResampleMethod::NONE) return me;

    const auto resampler = resamplers.find(key);
    if (resampler == resamplers.end()) return std::nullopt;

    return resampler->second.update(*me);
}

std::optional<MetricsEntry> GlobalMarketState::calcMetricsEntryFromAp(const Symbol symbol, const Market& market) {
    const AssetKey key{market, symbol};

    const auto ms = marketStates.find(key);
    if (ms == marketStates.end()) throw std::runtime_error("no market specified");

    const std::optional<MetricsEntry> me = metricsCalculator.calc(ms->second);
    if (!me) return std::nullopt;

    if (resampleCfg.method == ResampleMethod::NONE) return me;

    const auto resampler = resamplers.find(key);
    if (resampler == resamplers.end()) return std::nullopt;

    return resampler->second.update(*me);
}

MarketState& GlobalMarketState::getMarketState(const Symbol symbol, const Market& market) {
    const AssetKey key{market, symbol};
    const auto it = marketStates.find(key);
    if (it == marketStates.end()) { throw std::runtime_error("no market specified"); }
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
