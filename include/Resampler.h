#pragma once

#include <cstdint>
#include <optional>

#include "MetricsEntry.h"
#include "ResampleConfig.h"

class Resampler {
public:

    explicit Resampler(const ResampleConfig &cfg): cfg(cfg) {}

    std::optional<MetricsEntry> update(const MetricsEntry &me);

private:
    ResampleConfig cfg;

    bool hasPrev{false};
    uint64_t prevTsId{};
    MetricsEntry prevMe{};

    MetricsEntry meanMe{};
    uint64_t count{0};

    MetricsEntry hiMe{};
    MetricsEntry loMe{};
    MetricsEntry closeMe{};

    void resetAvg(const MetricsEntry& first);
    void updateAvg(const MetricsEntry& e);
    [[nodiscard]] MetricsEntry calcAvg() const;

    void resetHLC3(const MetricsEntry& firstMe);
    void updateHLC3(const MetricsEntry& me);
    [[nodiscard]] MetricsEntry calcHLC3() const;
};
