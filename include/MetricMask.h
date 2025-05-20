#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>
#include <string>

enum Metric : uint16_t {
    TimestampOfReceive   = 1 << 0,
    BestAskPrice         = 1 << 1,
    BestBidPrice         = 1 << 2,
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
        else if (s == "bestAskPrice")         m |= BestAskPrice;
        else if (s == "bestBidPrice")         m |= BestBidPrice;
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
