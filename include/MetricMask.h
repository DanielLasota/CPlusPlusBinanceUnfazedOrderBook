#pragma once

#include <cstdint>
#include <vector>
#include <string>

enum Metric : uint8_t {
    BestAsk              = 1 << 0,
    BestBid              = 1 << 1,
    MidPrice             = 1 << 2,
    BestVolumeImbalance  = 1 << 3,
    QueueImbalance       = 1 << 4,
    VolumeImbalance      = 1 << 5,
    Gap                  = 1 << 6,
    IsAggressorAsk       = 1 << 7,
};
using MetricMask = uint8_t;

inline MetricMask parseMask(const std::vector<std::string>& vars) {
    MetricMask m = 0;
    for (auto const& s : vars) {
        if      (s == "bestAsk")              m |= BestAsk;
        else if (s == "bestBid")              m |= BestBid;
        else if (s == "midPrice")             m |= MidPrice;
        else if (s == "bestVolumeImbalance")  m |= BestVolumeImbalance;
        else if (s == "queueImbalance")       m |= QueueImbalance;
        else if (s == "volumeImbalance")      m |= VolumeImbalance;
        else if (s == "gap")                  m |= Gap;
        else if (s == "isAggressorAsk")       m |= IsAggressorAsk;
    }
    return m;
}
