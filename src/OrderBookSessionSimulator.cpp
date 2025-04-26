#include "OrderbookSessionSimulator.h"
#include "DataVectorLoader.h"
#include <chrono>
#include <iostream>
#include <VariablesCounter.h>
#include <pybind11/pybind11.h>

OrderbookSessionSimulator::OrderbookSessionSimulator()
    : orderBook() {}

namespace py = pybind11;

void OrderbookSessionSimulator::computeBacktest(const std::string& csvPath, const py::object &python_callback) {

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptr_entries;

    ptr_entries.reserve(entries.size());
    for (auto &entry : entries) {
        ptr_entries.push_back(&entry);
    }

    DecodedEntry** data = ptr_entries.data();
    size_t count = ptr_entries.size();

    VariablesCounter variablesCounter(count);

    auto start = std::chrono::steady_clock::now();

    // for (size_t i = 0; i < count; ++i) {
    //     OrderBookEntry* entry = data[i];

    for (auto* entryPtr : ptr_entries) {
        if (auto* diff = std::get_if<DifferenceDepthEntry>(entryPtr)) {
            // std::cout
            //     << "DiffDepthEntry: TS="   << diff->TimestampOfReceive
            //     << ", Price="              << diff->Price
            //     << ", Qty="                << diff->Quantity
            //     << ", FirstUpd="           << diff->FirstUpdateId
            //     << ", FinalUpd="           << diff->FinalUpdateId
            //     << std::endl;
            orderBook.addOrder(diff);
            variablesCounter.update(orderBook);
        }
        else if (auto* trade = std::get_if<TradeEntry>(entryPtr)) {
            variablesCounter.update(orderBook, trade);
        }
        // orderBook.printOrderBook();

        // if (orderbook.asks.size() >= 2 && orderbook.bids.size() >= 2) {
        //     if (!python_callback.is_none()) {
        //         // python_callback(2, 1, 3, 7);
        //     }
        // }
    }

    auto finish = std::chrono::steady_clock::now();

    orderBook.printOrderBook();

    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

    variablesCounter.saveVariablesListToCSV("C:/Users/daniel/Documents/OrderBookVariables/x.csv");

    std::cout << "Start timestamp (ms): " << start_ms << std::endl;
    std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
    std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

}

void OrderbookSessionSimulator::computeVariables(const std::string &csvPath, std::vector<std::string> &variables) {
        std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptr_entries;

    ptr_entries.reserve(entries.size());
    for (auto &entry : entries) {
        ptr_entries.push_back(&entry);
    }

    DecodedEntry** data = ptr_entries.data();
    size_t count = ptr_entries.size();

    VariablesCounter variablesCounter(count);

    auto start = std::chrono::steady_clock::now();

    // for (size_t i = 0; i < count; ++i) {
    //     OrderBookEntry* entry = data[i];

    for (auto* entryPtr : ptr_entries) {
        if (auto* diff = std::get_if<DifferenceDepthEntry>(entryPtr)) {
            // std::cout
            //     << "DiffDepthEntry: TS="   << diff->TimestampOfReceive
            //     << ", Price="              << diff->Price
            //     << ", Qty="                << diff->Quantity
            //     << ", FirstUpd="           << diff->FirstUpdateId
            //     << ", FinalUpd="           << diff->FinalUpdateId
            //     << std::endl;
            orderBook.addOrder(diff);
            variablesCounter.update(orderBook);
        }
        else if (auto* trade = std::get_if<TradeEntry>(entryPtr)) {
            variablesCounter.update(orderBook, trade);
        }
        // orderBook.printOrderBook();

        // if (orderbook.asks.size() >= 2 && orderbook.bids.size() >= 2) {
        //     if (!python_callback.is_none()) {
        //         // python_callback(2, 1, 3, 7);
        //     }
        // }
    }

    auto finish = std::chrono::steady_clock::now();

    orderBook.printOrderBook();

    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

    variablesCounter.saveVariablesListToCSV("C:/Users/daniel/Documents/OrderBookVariables/x.csv");

    std::cout << "Start timestamp (ms): " << start_ms << std::endl;
    std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
    std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

}

FinalOrderBookSnapshot OrderbookSessionSimulator::computeFinalDepthSnapshot(const std::string &csvPath) {
    try {
        std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromSingleAssetParametersCSV(csvPath);
        std::vector<DecodedEntry*> ptr_entries;
        ptr_entries.reserve(entries.size());
        for (auto &entry : entries) {
            ptr_entries.push_back(&entry);
        }

        for (auto* entry : ptr_entries) {
            // orderBook.addOrder(entry);
        }

        FinalOrderBookSnapshot snapshot;
        for (auto* bid : orderBook.bids) {
            if(bid) {
                snapshot.bids.push_back(*bid);
            }
        }
        for (auto* ask : orderBook.asks) {
            if(ask) {
                snapshot.asks.push_back(*ask);
            }
        }

        return snapshot;

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return FinalOrderBookSnapshot{};
    }
}
