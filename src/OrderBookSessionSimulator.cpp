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

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.isLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addEntry(*m);
            }
        }
    }

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

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.isLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addEntry(*m);
                python_callback(*m );
            }
        }
    }

    return orderBookMetrics.entries();
}

py::dict OrderBookSessionSimulator::computeVariablesNumPy(const std::string &csvPath, std::vector<std::string> &variables) {
    // auto csvReadStart = std::chrono::steady_clock::now();

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

    // auto csvReadFinish = std::chrono::steady_clock::now();
    // auto csvReadElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(csvReadFinish - csvReadStart).count();
    // std::cout << "csv elapsed: " << csvReadElapsed << " ms" << std::endl;

    // auto loopStart = std::chrono::steady_clock::now();
    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.isLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addEntry(*m);
            }
        }
    }

    // auto loopFinish = std::chrono::steady_clock::now();
    // auto loopElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(loopFinish - loopStart).count();
    // std::cout << "loop elapsed: " << loopElapsed << " ms" << std::endl;

    // orderBookMetrics.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");
    return orderBookMetrics.convertToNumpyArrays();
}

py::dict OrderBookSessionSimulator::computeBacktestNumPy(const std::string& csvPath, std::vector<std::string> &variables, const pybind11::object &python_callback) {

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

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.isLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addEntry(*m);
                python_callback(*m );
            }
        }
    }

    return orderBookMetrics.convertToNumpyArrays();
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
