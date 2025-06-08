#include <iostream>
#include <iomanip>

#include "../include/OrderBook.h"
#include "../include/OrderBookSessionSimulator.h"

int main() {
    std::cout << std::fixed << std::setprecision(5);
    OrderBookSessionSimulator orderBookSessionSimulator;

    std::string csvPath = "sample_csvs/binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv";

    std::vector<std::string> variables {
        "timestampOfReceive",
        "market",
        "symbol",
        "bestAskPrice",
        "bestBidPrice",
        "midPrice",
        "bestVolumeImbalance",
        "queueImbalance",
        "volumeImbalance",
        "gap",
        "isAggressorAsk"
    };
    auto arrays = orderBookSessionSimulator.computeVariablesNumpy(csvPath, variables);
    std::cout << "returned " << arrays.size() << " arrays" << std::endl;

    return 0;
}
