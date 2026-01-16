#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "AssetKey.h"
#include "MarketState.h"
#include "MetricMask.h"
#include "MetricsCalculator.h"
#include "Resampler.h"

class GlobalMarketState {
public:
    explicit GlobalMarketState(const MetricMask& mask, const ResampleConfig& resampleCfg);

    explicit GlobalMarketState(const std::vector<std::string>& variables, const ResampleConfig& resampleCfg);

    void update(XEvent* e);

    std::optional<MetricsEntry> calcMetricsEntry(const XEvent* e);

    std::optional<MetricsEntry> calcMetricsEntryFromAp(Symbol symbol, const Market& market);

    MarketState& getMarketState(Symbol symbol, const Market& market);

    [[nodiscard]] size_t getMarketStateCount() const;

    [[nodiscard]] std::vector<std::pair<Symbol, Market>> getMarketStateList() const;

private:
    MetricMask mask;
    MetricsCalculator metricsCalculator;
    ResampleConfig resampleCfg;

    std::unordered_map<AssetKey, MarketState, AssetKeyHash> marketStates;
    std::unordered_map<AssetKey, Resampler, AssetKeyHash> resamplers;
};
