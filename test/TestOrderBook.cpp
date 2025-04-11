#include "OrderBook.h"
#include "enums/OrderBookEntry.h"
#include "../include/DataVectorLoader.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <OrderbookSessionSimulator.h>

FinalOrderBookSnapshot getFinalOrderBookSnapshotFromDifferenceDepthCSV(const std::string csvPath) {
    OrderbookSessionSimulator orderbookSessionSimulator;

    FinalOrderBookSnapshot finalOrderBookSnapshot = orderbookSessionSimulator.getFinalOrderBookSnapshot(csvPath);

    std::vector<OrderBookEntry> first1000Bids, last1000Asks;
    size_t numBids = finalOrderBookSnapshot.bids.size();
    if (numBids >= 1000) {
        first1000Bids.assign(
            finalOrderBookSnapshot.bids.begin(),
            finalOrderBookSnapshot.bids.begin() + 1000
        );
    } else {
        std::cerr << "Warning: bids contains less than 1000 elements." << std::endl;
        first1000Bids = finalOrderBookSnapshot.bids;
    }

    size_t numAsks = finalOrderBookSnapshot.asks.size();
    if (numAsks >= 1000) {
        last1000Asks.assign(
            finalOrderBookSnapshot.asks.begin(),
            finalOrderBookSnapshot.asks.begin() + 1000
        );
    } else {
        std::cerr << "Warning: asks contains less than 1000 elements." << std::endl;
        last1000Asks = finalOrderBookSnapshot.asks;
    }

    FinalOrderBookSnapshot modifiedSnapshot;
    modifiedSnapshot.bids = first1000Bids;
    modifiedSnapshot.asks = last1000Asks;

    return modifiedSnapshot;
}

FinalOrderBookSnapshot getFinalOrderBookSnapshotFromSnapshotCSV(const std::string &csvPath){
    OrderbookSessionSimulator orderbookSessionSimulator;
    FinalOrderBookSnapshot finalOrderBookSnapshot = orderbookSessionSimulator.getFinalOrderBookSnapshot(csvPath);

    return finalOrderBookSnapshot;
}

void testAddOrders() {
    OrderBook orderbook;

    OrderBookEntry ask1, ask2, ask3;
    ask1.Price = 100.0; ask1.Quantity = 10.0; ask1.IsAsk = true;
    ask2.Price = 90.0;  ask2.Quantity = 10.0; ask2.IsAsk = true;
    ask3.Price = 110.0; ask3.Quantity = 10.0; ask3.IsAsk = true;

    orderbook.addOrder(&ask1);
    orderbook.addOrder(&ask2);
    orderbook.addOrder(&ask3);

    assert(orderbook.asks.size() == 3);
    assert(orderbook.asks[0]->Price == 90.0);
    assert(orderbook.asks[1]->Price == 100.0);
    assert(orderbook.asks[2]->Price == 110.0);

    std::cout << "test_add_orders passed." << std::endl;
}

void testOrderBookSmallCSVSample() {
    OrderbookSessionSimulator orderbookSessionSimulator;
    std::string csvPath = "test/sample_csvs/binance_difference_depth_stream_usd_m_futures_trxusdt_09-04-2025_short.csv";

    FinalOrderBookSnapshot finalOrderBookSnapshot = orderbookSessionSimulator.getFinalOrderBookSnapshot(csvPath);

    std::vector<std::pair<double, double>> expectedBids = {
        {0.23043, 1974},
        {0.23041, 13055},
        {0.23040, 15462},
        {0.23027, 23951},
        {0.22122, 614}
    };

    std::vector<std::pair<double, double>> expectedAsks = {
        // asks attribute of class object is reverse to bids
        {0.23048, 29695},
        {0.23051, 33313},
        {0.23088, 10170},
        {0.23092, 37823}
    };

    assert(finalOrderBookSnapshot.bids.size() == expectedBids.size());
    for (size_t i = 0; i < expectedBids.size(); ++i) {
        double price = finalOrderBookSnapshot.bids[i].Price;
        double quantity = finalOrderBookSnapshot.bids[i].Quantity;
		assert(price == expectedBids[i].first);
		assert(quantity == expectedBids[i].second);
    }

    assert(finalOrderBookSnapshot.asks.size() == expectedAsks.size());
    for (size_t i = 0; i < expectedAsks.size(); ++i) {
        double price = finalOrderBookSnapshot.asks[i].Price;
        double quantity = finalOrderBookSnapshot.asks[i].Quantity;

		assert(price == expectedAsks[i].first);
        assert(quantity == expectedAsks[i].second);
    }

    std::cout << "test_orderbook_with_csv_sample passed." << std::endl;
}

void testWholeSpotTRXUSDTDatFinalOrderBookSnapshotWillReturnIdentical1000LevelsToOriginalBinanceSnapshot() {
    /*
     * 1. binance_difference_depth_stream_usd_m_futures_trxusdt_09-04-2025_3_cols_till_next_day_firsts.csv
     *  are: 09-04-2025 + first entries from 10-04-2025
     *  till 10-04-2025 snapshot: that means till LastUpdateId of 7236316022230
     * 2. binance_depth_snapshot_usd_m_futures_trxusdt_10-04-2025_first_snapshot.csv"
     *  is first snapshot from Thursday, 10 April 2025 00:00:00.289
     *  with LastUpdateId of 7236316022230
     *
     */

    std::string differenceDepthCSVPath = "test/sample_csvs/complexOrderBookTestOnSingleDay/binance_difference_depth_stream_usd_m_futures_trxusdt_09-04-2025_3_cols_till_next_day_firsts.csv";
    FinalOrderBookSnapshot finalOrderBookSnapshotFromDifferenceDepth = getFinalOrderBookSnapshotFromDifferenceDepthCSV(differenceDepthCSVPath);

    // finalOrderBookSnapshotFromDifferenceDepth.printFinalOrderBookSnapshot();

    std::string snapshotCSVPath = "test/sample_csvs/complexOrderBookTestOnSingleDay/binance_depth_snapshot_usd_m_futures_trxusdt_10-04-2025_first_snapshot.csv";
    FinalOrderBookSnapshot finalOrderBookSnapshotFromSnapshotCSV = getFinalOrderBookSnapshotFromSnapshotCSV(snapshotCSVPath);

    for (size_t i = 0; i < finalOrderBookSnapshotFromDifferenceDepth.asks.size(); ++i) {
        const auto& diffAsk = finalOrderBookSnapshotFromDifferenceDepth.asks[i];
        const auto& snapAsk = finalOrderBookSnapshotFromSnapshotCSV.asks[i];

        if(diffAsk.Price != snapAsk.Price) {
            std::cerr << "Asks price mismatch at level " << i << ":\n"
						<< "  DifferenceDepth Price: " << diffAsk.Price << "\n"
                        << "  DifferenceDepth Quantity: " << diffAsk.Quantity << "\n"
                        << "  Snapshot Price: " << snapAsk.Price << "\n"
                        << "  Snapshot Quantity: " << snapAsk.Quantity << "\n";
            assert(false);
        }
        if(diffAsk.Quantity != snapAsk.Quantity) {
            std::cerr << "Asks quantity mismatch at level " << i << ":\n"
						<< "  DifferenceDepth Price: " << diffAsk.Price << "\n"
                        << "  DifferenceDepth Quantity: " << diffAsk.Quantity << "\n"
                        << "  Snapshot Price:         " << snapAsk.Price << "\n"
                        << "  Snapshot Quantity:         " << snapAsk.Quantity << "\n";
            assert(false);
        }
    }

    for (size_t i = 0; i < finalOrderBookSnapshotFromDifferenceDepth.bids.size(); ++i) {
        const auto& diffBid = finalOrderBookSnapshotFromDifferenceDepth.bids[i];
        const auto& snapBid = finalOrderBookSnapshotFromSnapshotCSV.bids[i];

        if(diffBid.Price != snapBid.Price) {
            std::cerr << "Bids quantity mismatch at level " << i << ":\n"
                    << "  DifferenceDepth Price: " << diffBid.Price << "\n"
                    << "  DifferenceDepth Quantity: " << diffBid.Quantity << "\n"
                    << "  Snapshot Price: " << snapBid.Price << "\n"
                    << "  Snapshot Quantity: " << snapBid.Quantity << "\n";
            assert(false);
        }
        if(diffBid.Quantity != snapBid.Quantity) {
            std::cerr << "Bids quantity mismatch at level " << i << ":\n"
                    << "  DifferenceDepth Price: " << diffBid.Price << "\n"
                    << "  DifferenceDepth Quantity: " << diffBid.Quantity << "\n"
                    << "  Snapshot Price:         " << snapBid.Price << "\n"
                    << "  Snapshot Quantity:         " << snapBid.Quantity << "\n";
            assert(false);
        }
    }

    std::cout << "test_orderbook_with_csv_sample passed." << std::endl;
}


int main() {
    testAddOrders();
    testOrderBookSmallCSVSample();
    testWholeSpotTRXUSDTDatFinalOrderBookSnapshotWillReturnIdentical1000LevelsToOriginalBinanceSnapshot();
    std::cout << "\033[32m" << "All OrderBook tests passed successfully." << std::endl;
    return 0;
}

