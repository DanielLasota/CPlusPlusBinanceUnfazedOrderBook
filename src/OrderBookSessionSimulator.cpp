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
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) {
        ptrEntries.push_back(&entry);
    }

    MetricMask mask = parseMask(variables);
    GlobalMarketState globalMarketState(mask);
    OrderBookMetrics orderBookMetrics(variables);
    orderBookMetrics.reserve(ptrEntries.size());

    auto finish0 = std::chrono::steady_clock::now();
    auto elapsed_ms0 = std::chrono::duration_cast<std::chrono::milliseconds>(finish0 - start0).count();
    std::cout << "csv elapsed: " << elapsed_ms0 << " ms" << std::endl;

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptrEntries) {
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
    std::cout << "ob loop elapsed: " << elapsed_ms << " ms" << std::endl;

    // orderBookMetrics.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");

    return orderBookMetrics.entries();
}

std::vector<OrderBookMetricsEntry> OrderBookSessionSimulator::computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const pybind11::object &python_callback) {

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) {
        ptrEntries.push_back(&entry);
    }

    MetricMask mask = parseMask(variables);
    GlobalMarketState globalMarketState(mask);
    OrderBookMetrics orderBookMetrics(variables);
    orderBookMetrics.reserve(ptrEntries.size());

    // auto start = std::chrono::steady_clock::now();

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.IsLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addEntry(*m);
                python_callback(*m );
            }
        }
    }

    // auto finish = std::chrono::steady_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    // std::cout << "elapsed: " << elapsed_ms << " ms" << std::endl;
    return orderBookMetrics.entries();
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

        return std::move(marketState.orderBook);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return OrderBook{};
    }
}
