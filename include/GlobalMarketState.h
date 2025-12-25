#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "AssetKey.h"
#include "MarketState.h"
#include "MetricMask.h"
#include "MetricsCalculator.h"

class GlobalMarketState {
public:
    explicit GlobalMarketState(const MetricMask& mask);

    explicit GlobalMarketState(const std::vector<std::string>& variables);

    void update(XEvent* entry);

    std::optional<MetricsEntry> calcMetricsEntry(const XEvent* e);

    std::optional<MetricsEntry> calcMetricsEntryFromAp(Symbol symbol, const Market& market);

    MarketState& getMarketState(Symbol symbol, const Market& market);

    [[nodiscard]] size_t getMarketStateCount() const;

    [[nodiscard]] std::vector<std::pair<Symbol, Market>> getMarketStateList() const;

private:
    MetricMask mask;
    MetricsCalculator calculator;
    std::unordered_map<AssetKey, MarketState, AssetKeyHash> marketStates;
};