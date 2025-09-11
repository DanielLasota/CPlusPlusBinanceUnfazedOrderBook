#include <iomanip>
#include <pybind11/embed.h>

#include "../include/OrderBookSessionSimulator.h"

int main() {

    // #ifdef NDEBUG
    //     std::cerr << "Build: Release-like (NDEBUG)\n";
    // #else
    //     std::cerr << "Build: Debug-like (no NDEBUG)\n";
    // #endif

    py::scoped_interpreter guard{};

    OrderBookSessionSimulator orderBookSessionSimulator;
    // std::string csvPath = "C:/Users/daniel/Documents/merged_csvs/binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv";
    std::string csvPath = "D:/merged_csvs/binance_merged_btcusdt_usd_m_futures_2025-09-03.csv";

    std::vector<std::string> variables {
        "timestampOfReceive",
        "market",
        "symbol",
        "bestAskPrice",
        "bestBidPrice",
        "midPrice",

        "microPriceDiff",
        "microPriceImbalance",
        "microPriceFisherImbalance",
        "microPriceDeviation",
        "microPriceLogRatio",

        "bestBidQuantity",
        "bestAskQuantity",

        "bestOrderFlowDiff",
        "bestOrderFlowImbalance",
        "bestOrderFlowFisherImbalance",
        "bestOrderFlowCKSDiff",
        "bestOrderFlowCKSImbalance",
        "bestOrderFlowCKSFisherImbalance",
        "orderFlowDiff",
        "orderFlowImbalance",
        "orderFlowFisherImbalance",
        "queueCountFlowDiff",
        "queueCountFlowImbalance",
        "queueCountFlowFisherImbalance",

        "bestVolumeDiff",
        "bestVolumeImbalance",
        "bestVolumeFisherImbalance",
        "bestVolumeLogRatio",
        "bestVolumeSignedLogRatioXVolume",

        "bestTwoVolumeDiff",
        "bestThreeVolumeDiff",
        "bestFiveVolumeDiff",
        "bestTenVolumeDiff",
        "bestFifteenVolumeDiff",
        "bestTwentyVolumeDiff",
        "bestThirtyVolumeDiff",
        "bestFiftyVolumeDiff",

        "bestTwoVolumeImbalance",
        "bestThreeVolumeImbalance",
        "bestFiveVolumeImbalance",
        "bestTenVolumeImbalance",
        "bestFifteenVolumeImbalance",
        "bestTwentyVolumeImbalance",
        "bestThirtyVolumeImbalance",
        "bestFiftyVolumeImbalance",

        "bestTwoVolumeLogRatio",
        "bestThreeVolumeLogRatio",
        "bestFiveVolumeLogRatio",
        "bestTenVolumeLogRatio",
        "bestFifteenVolumeLogRatio",
        "bestTwentyVolumeLogRatio",
        "bestThirtyVolumeLogRatio",
        "bestFiftyVolumeLogRatio",

        "bestFiveVolumeLogRatioXVolume",
        "bestFiftyVolumeLogRatioXVolume",

        "volumeDiff",
        "volumeImbalance",
        "volumeLogRatio",
        "volumeLogRatioXVolume",

        "queueDiff",
        "queueImbalance",
        "queueLogRatio",
        "queueLogRatioXVolume",

        "gap",
        "isAggressorAsk",

        "vwapDeviation",
        "vwapLogRatio",

        "simplifiedSlopeDiff",
        "simplifiedSlopeImbalance",
        "simplifiedSlopeLogRatio",
        "bgcSlopeDiff",
        "bgcSlopeImbalance",
        "bgcSlopeLogRatio",

        "differenceDepthCount1Seconds",
        "differenceDepthCount3Seconds",
        "differenceDepthCount5Seconds",
        "differenceDepthCount10Seconds",
        "differenceDepthCount15Seconds",
        "differenceDepthCount30Seconds",
        "differenceDepthCount60Seconds",

        "tradeCount1Seconds",
        "tradeCount3Seconds",
        "tradeCount5Seconds",
        "tradeCount10Seconds",
        "tradeCount15Seconds",
        "tradeCount30Seconds",
        "tradeCount60Seconds",

        "differenceDepthCountDiff1Seconds",
        "differenceDepthCountDiff3Seconds",
        "differenceDepthCountDiff5Seconds",
        "differenceDepthCountDiff10Seconds",
        "differenceDepthCountDiff15Seconds",
        "differenceDepthCountDiff30Seconds",
        "differenceDepthCountDiff60Seconds",

        "differenceDepthCountImbalance1Seconds",
        "differenceDepthCountImbalance3Seconds",
        "differenceDepthCountImbalance5Seconds",
        "differenceDepthCountImbalance10Seconds",
        "differenceDepthCountImbalance15Seconds",
        "differenceDepthCountImbalance30Seconds",
        "differenceDepthCountImbalance60Seconds",

        "differenceDepthCountFisherImbalance1Seconds",
        "differenceDepthCountFisherImbalance3Seconds",
        "differenceDepthCountFisherImbalance5Seconds",
        "differenceDepthCountFisherImbalance10Seconds",
        "differenceDepthCountFisherImbalance15Seconds",
        "differenceDepthCountFisherImbalance30Seconds",
        "differenceDepthCountFisherImbalance60Seconds",

        "differenceDepthCountLogRatio1Seconds",
        "differenceDepthCountLogRatio3Seconds",
        "differenceDepthCountLogRatio5Seconds",
        "differenceDepthCountLogRatio10Seconds",
        "differenceDepthCountLogRatio15Seconds",
        "differenceDepthCountLogRatio30Seconds",
        "differenceDepthCountLogRatio60Seconds",

        "differenceDepthCountLogRatioXEventCount1Seconds",
        "differenceDepthCountLogRatioXEventCount3Seconds",
        "differenceDepthCountLogRatioXEventCount5Seconds",
        "differenceDepthCountLogRatioXEventCount10Seconds",
        "differenceDepthCountLogRatioXEventCount15Seconds",
        "differenceDepthCountLogRatioXEventCount30Seconds",
        "differenceDepthCountLogRatioXEventCount60Seconds",

        "tradeCountDiff1Seconds",
        "tradeCountDiff3Seconds",
        "tradeCountDiff5Seconds",
        "tradeCountDiff10Seconds",
        "tradeCountDiff15Seconds",
        "tradeCountDiff30Seconds",
        "tradeCountDiff60Seconds",

        "tradeCountImbalance1Seconds",
        "tradeCountImbalance3Seconds",
        "tradeCountImbalance5Seconds",
        "tradeCountImbalance10Seconds",
        "tradeCountImbalance15Seconds",
        "tradeCountImbalance30Seconds",
        "tradeCountImbalance60Seconds",

        "tradeCountFisherImbalance1Seconds",
        "tradeCountFisherImbalance3Seconds",
        "tradeCountFisherImbalance5Seconds",
        "tradeCountFisherImbalance10Seconds",
        "tradeCountFisherImbalance15Seconds",
        "tradeCountFisherImbalance30Seconds",
        "tradeCountFisherImbalance60Seconds",

        "tradeCountLogRatio1Seconds",
        "tradeCountLogRatio3Seconds",
        "tradeCountLogRatio5Seconds",
        "tradeCountLogRatio10Seconds",
        "tradeCountLogRatio15Seconds",
        "tradeCountLogRatio30Seconds",
        "tradeCountLogRatio60Seconds",

        "tradeVolumeDiff1Seconds",
        "tradeVolumeDiff3Seconds",
        "tradeVolumeDiff5Seconds",
        "tradeVolumeDiff10Seconds",
        "tradeVolumeDiff15Seconds",
        "tradeVolumeDiff30Seconds",
        "tradeVolumeDiff60Seconds",

        "tradeVolumeImbalance1Seconds",
        "tradeVolumeImbalance3Seconds",
        "tradeVolumeImbalance5Seconds",
        "tradeVolumeImbalance10Seconds",
        "tradeVolumeImbalance15Seconds",
        "tradeVolumeImbalance30Seconds",
        "tradeVolumeImbalance60Seconds",

        "tradeVolumeLogRatio1Seconds",
        "tradeVolumeLogRatio3Seconds",
        "tradeVolumeLogRatio5Seconds",
        "tradeVolumeLogRatio10Seconds",
        "tradeVolumeLogRatio15Seconds",
        "tradeVolumeLogRatio30Seconds",
        "tradeVolumeLogRatio60Seconds",

        "avgTradeSizeDiff1Seconds",
        "avgTradeSizeDiff3Seconds",
        "avgTradeSizeDiff5Seconds",
        "avgTradeSizeDiff10Seconds",
        "avgTradeSizeDiff15Seconds",
        "avgTradeSizeDiff30Seconds",
        "avgTradeSizeDiff60Seconds",

        "avgTradeSizeImbalance1Seconds",
        "avgTradeSizeImbalance3Seconds",
        "avgTradeSizeImbalance5Seconds",
        "avgTradeSizeImbalance10Seconds",
        "avgTradeSizeImbalance15Seconds",
        "avgTradeSizeImbalance30Seconds",
        "avgTradeSizeImbalance60Seconds",

        "avgTradeSizeLogRatio1Seconds",
        "avgTradeSizeLogRatio3Seconds",
        "avgTradeSizeLogRatio5Seconds",
        "avgTradeSizeLogRatio10Seconds",
        "avgTradeSizeLogRatio15Seconds",
        "avgTradeSizeLogRatio30Seconds",
        "avgTradeSizeLogRatio60Seconds",

        "biggestSingleBuyTradeVolume1Seconds",
        "biggestSingleBuyTradeVolume3Seconds",
        "biggestSingleBuyTradeVolume5Seconds",
        "biggestSingleBuyTradeVolume10Seconds",
        "biggestSingleBuyTradeVolume15Seconds",
        "biggestSingleBuyTradeVolume30Seconds",
        "biggestSingleBuyTradeVolume60Seconds",

        "biggestSingleSellTradeVolume1Seconds",
        "biggestSingleSellTradeVolume3Seconds",
        "biggestSingleSellTradeVolume5Seconds",
        "biggestSingleSellTradeVolume10Seconds",
        "biggestSingleSellTradeVolume15Seconds",
        "biggestSingleSellTradeVolume30Seconds",
        "biggestSingleSellTradeVolume60Seconds",

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

        "logReturnRatio1Seconds",
        "logReturnRatio3Seconds",
        "logReturnRatio5Seconds",
        "logReturnRatio10Seconds",
        "logReturnRatio15Seconds",
        "logReturnRatio30Seconds",
        "logReturnRatio60Seconds",

        "logKylesLambda1Seconds",
        "logKylesLambda3Seconds",
        "logKylesLambda5Seconds",
        "logKylesLambda10Seconds",
        "logKylesLambda15Seconds",
        "logKylesLambda30Seconds",
        "logKylesLambda60Seconds",

        //"rsi5Seconds",
        //"stochRsi5Seconds",
        //"macd2Seconds"
    };

    orderBookSessionSimulator.computeVariables(csvPath, variables);

    return 0;
}
