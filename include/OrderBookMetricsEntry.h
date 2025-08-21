#pragma once

#include <cstdint>

#include <enums/Market.h>
#include <enums/symbol.h>


struct OrderBookMetricsEntry {
    int64_t timestampOfReceive;
    uint8_t market;
    uint8_t symbol;
    double bestAskPrice;
    double bestBidPrice;
    double midPrice;
    double bestVolumeImbalance;
    double bestVolumeRatio;
    double bestTwoVolumeImbalance;
    double bestThreeVolumeImbalance;
    double bestFiveVolumeImbalance;
    double bestTenVolumeImbalance;
    double bestFifteenVolumeImbalance;
    double bestTwentyVolumeImbalance;
    double bestThirtyVolumeImbalance;
    double bestFiftyVolumeImbalance;
    double volumeImbalance;
    double queueImbalance;
    double gap;
    bool isAggressorAsk;
    double vwapDeviation;
    double simplifiedSlopeImbalance;

    double tradeCountImbalance1Seconds;
    double tradeCountImbalance3Seconds;
    double tradeCountImbalance5Seconds;
    double tradeCountImbalance10Seconds;
    double tradeCountImbalance15Seconds;
    double tradeCountImbalance30Seconds;
    double tradeCountImbalance60Seconds;

    double cumulativeDelta1Seconds;
    double cumulativeDelta3Seconds;
    double cumulativeDelta5Seconds;
    double cumulativeDelta10Seconds;
    double cumulativeDelta15Seconds;
    double cumulativeDelta30Seconds;
    double cumulativeDelta60Seconds;

    double priceDifference1Seconds;
    double priceDifference3Seconds;
    double priceDifference5Seconds;
    double priceDifference10Seconds;
    double priceDifference15Seconds;
    double priceDifference30Seconds;
    double priceDifference60Seconds;

    double rateOfReturn1Seconds;
    double rateOfReturn3Seconds;
    double rateOfReturn5Seconds;
    double rateOfReturn10Seconds;
    double rateOfReturn15Seconds;
    double rateOfReturn30Seconds;
    double rateOfReturn60Seconds;

    double differenceDepthVolatilityImbalance1Seconds;
    double differenceDepthVolatilityImbalance3Seconds;
    double differenceDepthVolatilityImbalance5Seconds;
    double differenceDepthVolatilityImbalance10Seconds;
    double differenceDepthVolatilityImbalance15Seconds;
    double differenceDepthVolatilityImbalance30Seconds;
    double differenceDepthVolatilityImbalance60Seconds;

    double rsi5Seconds;
    double stochRsi5Seconds;
    double macd2Seconds;
};
