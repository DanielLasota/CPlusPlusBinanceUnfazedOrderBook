#include "OrderbookSessionSimulator.h"
#include "DataVectorLoader.h"
#include <chrono>
#include <iostream>
#include <VariablesCounter.h>
#include <pybind11/pybind11.h>

OrderbookSessionSimulator::OrderbookSessionSimulator()
    : orderBook() {}

namespace py = pybind11;

void OrderbookSessionSimulator::processOrderbook(const std::string& csvPath, const py::object &python_callback) {

    try {
        std::vector<OrderBookEntry> entries = DataVectorLoader::getEntriesFromSingleAssetCSV(csvPath);
        std::vector<OrderBookEntry*> ptr_entries;

        ptr_entries.reserve(entries.size());
        for (auto &entry : entries) {
            ptr_entries.push_back(&entry);
        }

        OrderBookEntry** data = ptr_entries.data();
        size_t count = ptr_entries.size();

        VariablesCounter variablesCounter(count);

        auto start = std::chrono::steady_clock::now();

        // for (size_t i = 0; i < count; ++i) {
        //     OrderBookEntry* entry = data[i];

        for (auto* entry : ptr_entries) {
            orderBook.addOrder(entry);
            // orderbook.printOrderBook();
            // variablesCounter.update(orderbook);

            // if (orderbook.asks.size() >= 2 && orderbook.bids.size() >= 2) {
            //     if (!python_callback.is_none()) {
            //         // python_callback(2, 1, 3, 7);
            //     }
            // }
        }

        auto finish = std::chrono::steady_clock::now();

        auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

        // variablesCounter.saveVariablesListToCSV("C:/Users/daniel/Documents/OrderBookVariables/x.csv");

        std::cout << "Start timestamp (ms): " << start_ms << std::endl;
        std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
        std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

FinalOrderBookSnapshot OrderbookSessionSimulator::getFinalOrderBookSnapshot(const std::string &csvPath) {
    try {
        std::vector<OrderBookEntry> entries = DataVectorLoader::getEntriesFromSingleAssetCSV(csvPath);
        std::vector<OrderBookEntry*> ptr_entries;
        ptr_entries.reserve(entries.size());
        for (auto &entry : entries) {
            ptr_entries.push_back(&entry);
        }

        size_t count = ptr_entries.size();
        VariablesCounter variablesCounter(count);

        auto start = std::chrono::steady_clock::now();

        for (auto* entry : ptr_entries) {
            orderBook.addOrder(entry);
        }

        auto finish = std::chrono::steady_clock::now();
        auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

        std::cout << "Start timestamp (ms): " << start_ms << std::endl;
        std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
        std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

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
