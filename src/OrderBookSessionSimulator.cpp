#include <chrono>
#include <iostream>
#include <pybind11/pybind11.h>

#include "GlobalMarketState.h"
#include "DataVectorLoader.h"
#include "MarketState.h"
#include "OrderBookMetrics.h"
#include "OrderbookSessionSimulator.h"
#include "OrderBookMetricsCalculator.h"

OrderBookSessionSimulator::OrderBookSessionSimulator() {}

std::vector<OrderBookMetricsEntry> OrderBookSessionSimulator::computeVariables(const std::string &csvPath, std::vector<std::string> &variables) {
    auto start0 = std::chrono::steady_clock::now();

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptr_entries;

    ptr_entries.reserve(entries.size());
    for (auto &entry : entries) {
        ptr_entries.push_back(&entry);
    }

    MetricMask mask = parseMask(variables);
    GlobalMarketState globalMarketState(mask);
    OrderBookMetrics orderBookMetrics(variables);
    orderBookMetrics.reserve(ptr_entries.size());
    auto finish0 = std::chrono::steady_clock::now();
    auto elapsed_ms0 = std::chrono::duration_cast<std::chrono::milliseconds>(finish0 - start0).count();
    std::cout << "elapsed: " << elapsed_ms0 << " ms" << std::endl;

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptr_entries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.IsLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addEntry(*m);
            }
        }
    }

    auto finish = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;

    orderBookMetrics.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");

    return orderBookMetrics.entries();
}

void OrderBookSessionSimulator::computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const pybind11::object &python_callback) {

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) {
        ptrEntries.push_back(&entry);
    }

    MetricMask mask = parseMask(variables);
    GlobalMarketState globalMarketState(mask);

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.IsLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                python_callback(*m );
            }
        }
    }

    auto finish = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
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

        return std::move(marketState.orderBook);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return OrderBook{};
    }
}
