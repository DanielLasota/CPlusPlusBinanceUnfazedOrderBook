#pragma once

#include <unordered_map>
#include <string>
#include <optional>

#include "MetricMask.h"
#include "OrderBookMetricsCalculator.h"
#include "MarketState.h"
#include "EntryDecoder.h"

struct AssetKey {
    Market      market;
    std::string symbol;

    bool operator==(const AssetKey& o) const noexcept {
        return market == o.market && symbol == o.symbol;
    }
};

struct AssetKeyHash {
    size_t operator()(const AssetKey& k) const noexcept {
        auto h1 = std::hash<int>{}(int(k.market));
        auto h2 = std::hash<std::string>{}(k.symbol);
        return h1 ^ (h2 << 1);
    }
};

class GlobalMarketState {
public:
    explicit GlobalMarketState(MetricMask mask);

    void update(DecodedEntry* entry);

    std::optional<OrderBookMetricsEntry> countMarketStateMetricsByEntry(DecodedEntry* entry);
    std::optional<OrderBookMetricsEntry> countMarketStateMetrics(const std::string& symbol, const Market& market);

    MarketState& getMarketState(const std::string& symbol, const std::string& market_str);

    size_t marketStateCount() const { return marketStates_.size(); }

    std::vector<std::pair<std::string, Market>> marketStateList() const {
        std::vector<std::pair<std::string, Market>> out;
        out.reserve(marketStates_.size());
        for (auto const& kv : marketStates_) {
            out.emplace_back(kv.first.symbol, kv.first.market);
        }
        return out;
    }

private:
    AssetKey extractKey(const DecodedEntry& d) const;
    MetricMask mask_;
    OrderBookMetricsCalculator calculator_;
    std::unordered_map<AssetKey, MarketState, AssetKeyHash> marketStates_;
};