// src/main.cpp
#include "../include/OrderBook.h"
#include "../include/OrderBookSessionSimulator.h"

void mainOrderbookEntryLoop(OrderBook &orderbook);

void processOrderbook();

int main() {
    OrderbookSessionSimulator orderbookSessionSimulator;

    std::string csvPath = "C:/Users/daniel/Documents/merged_csvs/merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv";
    std::vector<std::string> variables;

    orderbookSessionSimulator.computeBacktest(csvPath, variables);
    return 0;
}
