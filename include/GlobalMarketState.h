#pragma once

#include <unordered_map>
#include <string>
#include <optional>

#include "MetricMask.h"
#include "OrderBookMetricsCalculator.h"
#include "MarketState.h"
#include "AssetKey.h"

class GlobalMarketState {
public:
    explicit GlobalMarketState(const MetricMask& mask);

    explicit GlobalMarketState(const std::vector<std::string>& variables);

    void update(DecodedEntry* entry);

    std::optional<OrderBookMetricsEntry> countMarketStateMetricsByEntry(DecodedEntry* entry);

    std::optional<OrderBookMetricsEntry> countMarketStateMetrics(Symbol symbol, const Market& market);

    MarketState& getMarketState(Symbol symbol, const Market& market);

    size_t getMarketStateCount() const;

    std::vector<std::pair<Symbol, Market>> getMarketStateList() const;

private:
    MetricMask mask_;
    OrderBookMetricsCalculator calculator_;
    std::unordered_map<AssetKey, MarketState, AssetKeyHash> marketStates_;
};