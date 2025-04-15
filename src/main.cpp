// src/main.cpp
#include "../include/OrderBook.h"
#include "../include/OrderBookSessionSimulator.h"

void mainOrderbookEntryLoop(OrderBook &orderbook);

void processOrderbook();

int main() {
    OrderbookSessionSimulator orderbookSessionSimulator;

//    std::string csvPath = "C:/Users/daniel/Documents/sample_merged_csvs_for_ob/merged_depth_snapshot_difference_depth_stream_trade_stream_spot_trxusdt_10-04-2025.csv";
     std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_01-04-2025.csv";

    orderbookSessionSimulator.processOrderbook(csvPath);

    return 0;
}
