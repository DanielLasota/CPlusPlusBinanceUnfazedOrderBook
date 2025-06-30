#include <iostream>
#include <iomanip>
#include <pybind11/embed.h>

#include "../include/OrderBook.h"
#include "../include/OrderBookSessionSimulator.h"

int main() {
    py::scoped_interpreter guard{};

    std::cout << std::fixed << std::setprecision(5);
    OrderBookSessionSimulator orderBookSessionSimulator;
    std::string csvPath = "C:/Users/daniel/Documents/merged_csvs/binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv";

    std::vector<std::string> variables {
        "timestampOfReceive",
        "market",
        "symbol",
        "bestAskPrice",
        "bestBidPrice",
        "midPrice",

        "bestVolumeImbalance",
        "bestTwoVolumeImbalance",
        "bestThreeVolumeImbalance",
        "bestFiveVolumeImbalance",
        "queueImbalance",
        "volumeImbalance",
        // "gap",
        "isAggressorAsk",
        "vwapDeviation",
        "simplifiedSlopeImbalance",

        "tradeCountImbalance1Seconds",
        "tradeCountImbalance3Seconds",
        "tradeCountImbalance5Seconds",
        "tradeCountImbalance10Seconds",
        "tradeCountImbalance15Seconds",
        "tradeCountImbalance30Seconds",
        "tradeCountImbalance60Seconds",

        "cumulativeDelta1Seconds",
        "cumulativeDelta3Seconds",
        "cumulativeDelta5Seconds",
        "cumulativeDelta10Seconds",
        "cumulativeDelta15Seconds",
        "cumulativeDelta30Seconds",
        "cumulativeDelta60Seconds",

        "priceDifference1Seconds",
        "priceDifference3Seconds",
        "priceDifference5Seconds",
        "priceDifference10Seconds",
        "priceDifference15Seconds",
        "priceDifference30Seconds",
        "priceDifference60Seconds",

        "rateOfReturn1Seconds",
        "rateOfReturn3Seconds",
        "rateOfReturn5Seconds",
        "rateOfReturn10Seconds",
        "rateOfReturn15Seconds",
        "rateOfReturn30Seconds",
        "rateOfReturn60Seconds",

        "differenceDepthVolatilityImbalance1Seconds",
        "differenceDepthVolatilityImbalance3Seconds",
        "differenceDepthVolatilityImbalance5Seconds",
        "differenceDepthVolatilityImbalance10Seconds",
        "differenceDepthVolatilityImbalance15Seconds",
        "differenceDepthVolatilityImbalance30Seconds",
        "differenceDepthVolatilityImbalance60Seconds",

        "rsi5Seconds",
        "stochRsi5Seconds",
        "macd2Seconds",
    };
    orderBookSessionSimulator.computeVariablesNumPy(csvPath, variables);

    return 0;
}
