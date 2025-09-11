#pragma once

#include "MarketState.h"
#include "OrderBookMetricsEntry.h"
#include "MetricMask.h"

#include <optional>

class OrderBookMetricsCalculator {
public:
    explicit OrderBookMetricsCalculator(const MetricMask& mask)
      : mask_(mask) {}

    explicit OrderBookMetricsCalculator(const std::vector<std::string>& variables)
    : OrderBookMetricsCalculator(parseMask(variables))
    {}

    std::optional<OrderBookMetricsEntry> countMarketStateMetrics(const MarketState& marketState) const;

private:
    MetricMask mask_;
};
