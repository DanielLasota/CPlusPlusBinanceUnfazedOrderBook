// src/main.cpp
#include "OrderBook.h"
#include "OrderBookSessionSimulator.h"

void mainOrderbookEntryLoop(OrderBook &orderbook);

void processOrderbook();

int main() {
    OrderbookSessionSimulator orderbookSessionSimulator;

//    std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_spot_trxusdt_01-04-2025.csv";
    std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_01-04-2025.csv";
//    std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_coin_m_futures_trxusd_perp_01-04-2025.csv";

    orderbookSessionSimulator.processOrderbook(csvPath);

    return 0;
}
