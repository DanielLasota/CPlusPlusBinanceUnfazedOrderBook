// src/main.cpp
#include <iostream>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "CSVParser.h"
#include "OrderBookSessionSimulator.h"

void mainOrderbookEntryLoop(OrderBook &orderbook);

void processOrderbook();

int main() {
    OrderbookSessionSimulator simulator;

    std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_25-03-2025.csv";

    simulator.processOrderbook(csvPath);

    return 0;
}
