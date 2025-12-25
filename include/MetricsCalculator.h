#pragma once

#include "MarketState.h"
#include "MetricMask.h"
#include "MetricsEntry.h"

#include <optional>

class MetricsCalculator {
public:
    explicit MetricsCalculator(const MetricMask& mask)
        : mask(mask) {}

    explicit MetricsCalculator(const std::vector<std::string>& variables)
        : MetricsCalculator(parseMask(variables)) {}

    [[nodiscard]] std::optional<MetricsEntry> calc(const MarketState& marketState) const;

private:
    MetricMask mask;
};
