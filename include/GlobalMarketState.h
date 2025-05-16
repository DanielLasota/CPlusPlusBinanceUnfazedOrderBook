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

    std::optional<OrderBookMetricsEntry> countMarketStateMetrics(DecodedEntry* entry);

private:
    AssetKey extractKey(const DecodedEntry& d) const;
    MetricMask                                                      mask_;
    OrderBookMetricsCalculator                                      calculator_;
    std::unordered_map<AssetKey, MarketState, AssetKeyHash>         states_;
};