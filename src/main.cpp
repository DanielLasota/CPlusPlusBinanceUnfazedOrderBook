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
        "gap",
        "isAggressorAsk",
        "vwapDeviation",
        "simplifiedSlopeImbalance"
    };
    orderBookSessionSimulator.computeVariablesNumPy(csvPath, variables);

    return 0;
}
