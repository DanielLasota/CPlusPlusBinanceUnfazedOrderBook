#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>
#include <string>

enum Metric : uint64_t  {
    TimestampOfReceive                              = 1ULL << 0,
    market                                          = 1ULL << 1,
    symbol                                          = 1ULL << 2,
    BestAskPrice                                    = 1ULL << 3,
    BestBidPrice                                    = 1ULL << 4,
    MidPrice                                        = 1ULL << 5,
    BestVolumeImbalance                             = 1ULL << 6,
    BestVolumeRatio                                 = 1ULL << 7,
    BestTwoVolumeImbalance                          = 1ULL << 8,
    BestThreeVolumeImbalance                        = 1ULL << 9,
    BestFiveVolumeImbalance                         = 1ULL << 10,
    VolumeImbalance                                 = 1ULL << 11,
    QueueImbalance                                  = 1ULL << 12,
    Gap                                             = 1ULL << 13,
    IsAggressorAsk                                  = 1ULL << 14,
    VwapDeviation                                   = 1ULL << 15,
    simplifiedSlopeImbalance                        = 1ULL << 16,

    tradeCountImbalance1Seconds                     = 1ULL << 17,
    tradeCountImbalance3Seconds                     = 1ULL << 18,
    tradeCountImbalance5Seconds                     = 1ULL << 19,
    tradeCountImbalance10Seconds                    = 1ULL << 20,
    tradeCountImbalance15Seconds                    = 1ULL << 21,
    tradeCountImbalance30Seconds                    = 1ULL << 22,
    tradeCountImbalance60Seconds                    = 1ULL << 23,

    cumulativeDelta1Seconds                         = 1ULL << 24,
    cumulativeDelta3Seconds                         = 1ULL << 25,
    cumulativeDelta5Seconds                         = 1ULL << 26,
    cumulativeDelta10Seconds                        = 1ULL << 27,
    cumulativeDelta15Seconds                        = 1ULL << 28,
    cumulativeDelta30Seconds                        = 1ULL << 29,
    cumulativeDelta60Seconds                        = 1ULL << 30,

    priceDifference1Seconds                         = 1ULL << 31,
    priceDifference3Seconds                         = 1ULL << 32,
    priceDifference5Seconds                         = 1ULL << 33,
    priceDifference10Seconds                        = 1ULL << 34,
    priceDifference15Seconds                        = 1ULL << 35,
    priceDifference30Seconds                        = 1ULL << 36,
    priceDifference60Seconds                        = 1ULL << 37,

    rateOfReturn1Seconds                            = 1ULL << 38,
    rateOfReturn3Seconds                            = 1ULL << 39,
    rateOfReturn5Seconds                            = 1ULL << 40,
    rateOfReturn10Seconds                           = 1ULL << 41,
    rateOfReturn15Seconds                           = 1ULL << 42,
    rateOfReturn30Seconds                           = 1ULL << 43,
    rateOfReturn60Seconds                           = 1ULL << 44,

    differenceDepthVolatilityImbalance1Seconds      = 1ULL << 45,
    differenceDepthVolatilityImbalance3Seconds      = 1ULL << 46,
    differenceDepthVolatilityImbalance5Seconds      = 1ULL << 47,
    differenceDepthVolatilityImbalance10Seconds     = 1ULL << 48,
    differenceDepthVolatilityImbalance15Seconds     = 1ULL << 49,
    differenceDepthVolatilityImbalance30Seconds     = 1ULL << 50,
    differenceDepthVolatilityImbalance60Seconds     = 1ULL << 51,

};
using MetricMask = uint64_t;

inline MetricMask parseMask(const std::vector<std::string>& vars) {
    MetricMask m = 0;
    for (auto const& s : vars) {
        if      (s == "timestampOfReceive")                             m |= TimestampOfReceive;
        else if (s == "market")                                         m |= market;
        else if (s == "symbol")                                         m |= symbol;
        else if (s == "bestAskPrice")                                   m |= BestAskPrice;
        else if (s == "bestBidPrice")                                   m |= BestBidPrice;
        else if (s == "midPrice")                                       m |= MidPrice;
        else if (s == "bestVolumeImbalance")                            m |= BestVolumeImbalance;
        else if (s == "bestVolumeRatio")                                m |= BestVolumeRatio;
        else if (s == "bestTwoVolumeImbalance")                         m |= BestTwoVolumeImbalance;
        else if (s == "bestThreeVolumeImbalance")                       m |= BestThreeVolumeImbalance;
        else if (s == "bestFiveVolumeImbalance")                        m |= BestFiveVolumeImbalance;
        else if (s == "volumeImbalance")                                m |= VolumeImbalance;
        else if (s == "queueImbalance")                                 m |= QueueImbalance;
        else if (s == "gap")                                            m |= Gap;
        else if (s == "isAggressorAsk")                                 m |= IsAggressorAsk;
        else if (s == "vwapDeviation")                                  m |= VwapDeviation;
        else if (s == "simplifiedSlopeImbalance")                       m |= simplifiedSlopeImbalance;

        else if (s == "tradeCountImbalance1Seconds")                    m |= tradeCountImbalance1Seconds;
        else if (s == "tradeCountImbalance3Seconds")                    m |= tradeCountImbalance3Seconds;
        else if (s == "tradeCountImbalance5Seconds")                    m |= tradeCountImbalance5Seconds;
        else if (s == "tradeCountImbalance10Seconds")                   m |= tradeCountImbalance10Seconds;
        else if (s == "tradeCountImbalance15Seconds")                   m |= tradeCountImbalance15Seconds;
        else if (s == "tradeCountImbalance30Seconds")                   m |= tradeCountImbalance30Seconds;
        else if (s == "tradeCountImbalance60Seconds")                   m |= tradeCountImbalance60Seconds;

        else if (s == "cumulativeDelta1Seconds")                        m |= cumulativeDelta1Seconds;
        else if (s == "cumulativeDelta3Seconds")                        m |= cumulativeDelta3Seconds;
        else if (s == "cumulativeDelta5Seconds")                        m |= cumulativeDelta5Seconds;
        else if (s == "cumulativeDelta10Seconds")                       m |= cumulativeDelta10Seconds;
        else if (s == "cumulativeDelta15Seconds")                       m |= cumulativeDelta15Seconds;
        else if (s == "cumulativeDelta30Seconds")                       m |= cumulativeDelta30Seconds;
        else if (s == "cumulativeDelta60Seconds")                       m |= cumulativeDelta60Seconds;

        else if (s == "priceDifference1Seconds")                        m |= priceDifference1Seconds;
        else if (s == "priceDifference3Seconds")                        m |= priceDifference3Seconds;
        else if (s == "priceDifference5Seconds")                        m |= priceDifference5Seconds;
        else if (s == "priceDifference10Seconds")                       m |= priceDifference10Seconds;
        else if (s == "priceDifference15Seconds")                       m |= priceDifference15Seconds;
        else if (s == "priceDifference30Seconds")                       m |= priceDifference30Seconds;
        else if (s == "priceDifference60Seconds")                       m |= priceDifference60Seconds;

        else if (s == "rateOfReturn1Seconds")                           m |= rateOfReturn1Seconds;
        else if (s == "rateOfReturn3Seconds")                           m |= rateOfReturn3Seconds;
        else if (s == "rateOfReturn5Seconds")                           m |= rateOfReturn5Seconds;
        else if (s == "rateOfReturn10Seconds")                          m |= rateOfReturn10Seconds;
        else if (s == "rateOfReturn15Seconds")                          m |= rateOfReturn15Seconds;
        else if (s == "rateOfReturn30Seconds")                          m |= rateOfReturn30Seconds;
        else if (s == "rateOfReturn60Seconds")                          m |= rateOfReturn60Seconds;

        else if (s == "differenceDepthVolatilityImbalance1Seconds")     m |= differenceDepthVolatilityImbalance1Seconds;
        else if (s == "differenceDepthVolatilityImbalance3Seconds")     m |= differenceDepthVolatilityImbalance3Seconds;
        else if (s == "differenceDepthVolatilityImbalance5Seconds")     m |= differenceDepthVolatilityImbalance5Seconds;
        else if (s == "differenceDepthVolatilityImbalance10Seconds")    m |= differenceDepthVolatilityImbalance10Seconds;
        else if (s == "differenceDepthVolatilityImbalance15Seconds")    m |= differenceDepthVolatilityImbalance15Seconds;
        else if (s == "differenceDepthVolatilityImbalance30Seconds")    m |= differenceDepthVolatilityImbalance30Seconds;
        else if (s == "differenceDepthVolatilityImbalance60Seconds")    m |= differenceDepthVolatilityImbalance60Seconds;

        else {
            throw std::invalid_argument("Unknown variable name: " + s);
        }
    }
    return m;
}
