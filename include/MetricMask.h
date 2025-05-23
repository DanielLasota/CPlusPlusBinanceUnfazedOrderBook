#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>
#include <string>

enum Metric : uint16_t {
    TimestampOfReceive   = 1 << 0,
    market               = 1 << 1,
    symbol               = 1 << 2,
    BestAskPrice         = 1 << 3,
    BestBidPrice         = 1 << 4,
    MidPrice             = 1 << 5,
    BestVolumeImbalance  = 1 << 6,
    QueueImbalance       = 1 << 7,
    VolumeImbalance      = 1 << 8,
    Gap                  = 1 << 9,
    IsAggressorAsk       = 1 << 10,
};
using MetricMask = uint16_t;

inline MetricMask parseMask(const std::vector<std::string>& vars) {
    MetricMask m = 0;
    for (auto const& s : vars) {
        if      (s == "timestampOfReceive")   m |= TimestampOfReceive;
        else if (s == "market")               m |= market;
        else if (s == "symbol")               m |= symbol;
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
