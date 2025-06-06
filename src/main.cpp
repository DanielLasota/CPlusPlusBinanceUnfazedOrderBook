#include <iostream>
#include <iomanip>

#include "../include/OrderBook.h"
#include "../include/OrderBookSessionSimulator.h"

void mainOrderbookEntryLoop(OrderBook &orderbook);

void processOrderbook();

int main() {
    std::cout << std::fixed << std::setprecision(5);
    OrderBookSessionSimulator orderBookSessionSimulator;
    std::string csvPath = "C:/Users/daniel/Documents/merged_csvs/binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv";
//    std::string csvPath = "C:/Users/daniel/Documents/merged_csvs/binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_adausdt_trxusdt_14-04-2025.csv";
//    std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_14-04-2025.csv";

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
    orderBookSessionSimulator.computeVariables(csvPath, variables);

    // OrderBookSessionSimulator orderBookSessionSimulator;
    // std::string csvPath = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_14-04-2025.csv";
    // OrderBook orderbook = orderBookSessionSimulator.computeFinalDepthSnapshot(csvPath);
    // orderbook.printOrderBook();
    //
    // auto e = orderbook.getAsks()[0];
    // std::cout
    //     << e.TimestampOfReceive << " "
    //     << e.Stream << " "
    //     << e.EventType << " "
    //     << e.EventTime << " "
    //     << e.TransactionTime << " "
    //     << e.Symbol << " "
    //     << e.FirstUpdateId << " "
    //     << e.FinalUpdateId << " "
    //     << e.FinalUpdateIdInLastStream << " "
    //     << e.IsAsk << " "
    //     << e.Price << " "
    //     << e.Quantity << " "
    //     << e.PSUnknownField << " "
    //     << "\n";

    return 0;
}
