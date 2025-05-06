#include <chrono>
#include <iostream>
#include <pybind11/pybind11.h>

#include "OrderbookSessionSimulator.h"
#include "DataVectorLoader.h"
#include "MarketState.h"
#include "OrderBookMetrics.h"

OrderBookSessionSimulator::OrderBookSessionSimulator() {}

namespace py = pybind11;

std::vector<OrderBookMetricsEntry> OrderBookSessionSimulator::computeVariables(const std::string &csvPath, std::vector<std::string> &variables) {

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptr_entries;

    ptr_entries.reserve(entries.size());
    for (auto &entry : entries) {
        ptr_entries.push_back(&entry);
    }

    MarketState marketState;
    MetricMask mask = parseMask(variables);
    OrderBookMetrics orderBookMetrics(variables);
    orderBookMetrics.reserve(ptr_entries.size());

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptr_entries) {
        marketState.update(p);

        const bool isLast = std::visit([](auto const& entry){
                return entry.IsLast;
        }, *p);

        if (isLast == true) {
            if (auto m = marketState.countOrderBookMetrics(mask)) {
                orderBookMetrics.addEntry(*m);
                // marketState.orderBook.printOrderBook();
                // std::visit([](auto const& entry) {
                //     std::cout << entry.TimestampOfReceive << " " << entry.Stream << std::endl;
                // },*p);
                // std::cout << "\n" << std::endl;
                // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            }
        }
    }

    auto finish = std::chrono::steady_clock::now();
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    std::cout << "Start timestamp (ms): " << start_ms << std::endl;
    std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
    std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

    orderBookMetrics.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");
    return orderBookMetrics.entries();
}

void OrderBookSessionSimulator::computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const py::object &python_callback) {
    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) {
        ptrEntries.push_back(&entry);
    }

    MarketState marketState;
    MetricMask mask = parseMask(variables);

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptrEntries) {
        marketState.update(p);

        const bool isLast = std::visit([](auto const& entry){
                return entry.IsLast;
        }, *p);

        if (isLast == true) {
            if (auto m = marketState.countOrderBookMetrics(mask)) {
                python_callback(*m );
                // marketState.orderBook.printOrderBook();
                // std::visit([](auto const& entry) {
                //     std::cout << entry.TimestampOfReceive << " " << entry.Stream << std::endl;
                // },*p);
                // std::cout << "\n" << std::endl;
                // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            }
        }
    }

    auto finish = std::chrono::steady_clock::now();
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    std::cout << "Start timestamp (ms): " << start_ms << std::endl;
    std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
    std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

}

OrderBook OrderBookSessionSimulator::computeFinalDepthSnapshot(const std::string &csvPath) {
    try {
        std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromSingleAssetParametersCSV(csvPath);
        std::vector<DecodedEntry*> ptrEntries;
        ptrEntries.reserve(entries.size());
        for (auto &entry : entries) {
            ptrEntries.push_back(&entry);
        }

        MarketState marketState;

        for (auto* entryPtr : ptrEntries) {
            marketState.update(entryPtr);
        }

        // marketState.orderBook.printOrderBook();

        // std::cout << "best Ask Price " << marketState.orderBook.bestAskPrice() << " Quantity " << marketState.orderBook.bestAskQuantity() << std::endl;
        // std::cout << "best Bid Price " << marketState.orderBook.bestBidPrice() << " Quantity " << marketState.orderBook.bestBidQuantity() << std::endl;

        return std::move(marketState.orderBook);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return OrderBook{};
    }
}
