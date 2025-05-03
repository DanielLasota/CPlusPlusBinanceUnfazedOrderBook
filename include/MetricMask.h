#pragma once

#include <cstdint>
#include <vector>
#include <string>

enum Metric : uint16_t {
    TimestampOfReceive   = 1 << 0,
    BestAsk              = 1 << 1,
    BestBid              = 1 << 2,
    MidPrice             = 1 << 3,
    BestVolumeImbalance  = 1 << 4,
    QueueImbalance       = 1 << 5,
    VolumeImbalance      = 1 << 6,
    Gap                  = 1 << 7,
    IsAggressorAsk       = 1 << 8,
};
using MetricMask = uint16_t;

inline MetricMask parseMask(const std::vector<std::string>& vars) {
    MetricMask m = 0;
    for (auto const& s : vars) {
        if      (s == "timestampOfReceive")   m |= TimestampOfReceive;
        else if (s == "bestAsk")              m |= BestAsk;
        else if (s == "bestBid")              m |= BestBid;
        else if (s == "midPrice")             m |= MidPrice;
        else if (s == "bestVolumeImbalance")  m |= BestVolumeImbalance;
        else if (s == "queueImbalance")       m |= QueueImbalance;
        else if (s == "volumeImbalance")      m |= VolumeImbalance;
        else if (s == "gap")                  m |= Gap;
        else if (s == "isAggressorAsk")       m |= IsAggressorAsk;
        else {
            throw std::invalid_argument("Unknown variable name: " + s);
        }
    }
    return m;
}
