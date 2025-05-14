#pragma once

#include "MarketState.h"
#include "OrderBookMetricsEntry.h"
#include "MetricMask.h"

#include <optional>

class OrderBookMetricsCalculator {
public:
    explicit OrderBookMetricsCalculator(MetricMask mask)
      : mask_(mask)
    {}

    std::optional<OrderBookMetricsEntry> compute(const MarketState& ms) const;

private:
    MetricMask mask_;
};
