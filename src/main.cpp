// src/main.cpp
#include "../include/OrderBook.h"
#include "../include/OrderBookSessionSimulator.h"

void mainOrderbookEntryLoop(OrderBook &orderbook);

void processOrderbook();

int main() {
    OrderBookSessionSimulator orderBookSessionSimulator;

    std::string csvPath = "C:/Users/daniel/Documents/merged_csvs/merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv";
    // std::string csvPath = "python_tests/csv/test_positive_binance_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_24-04-2025.csv";
    std::vector<std::string> variables {
        "timestampOfReceive",
//        "bestAsk",
//        "bestBid",
//        "midPrice",
//        "bestVolumeImbalance",
//        "gap"
    };

    orderBookSessionSimulator.computeVariables(csvPath,variables);
    return 0;
}
