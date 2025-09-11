#include <chrono>
#include <iostream>
#include <pybind11/pybind11.h>

#include "GlobalMarketState.h"
#include "DataVectorLoader.h"
#include "MarketState.h"
#include "OrderBookMetrics.h"
#include "OrderbookSessionSimulator.h"

OrderBookSessionSimulator::OrderBookSessionSimulator() = default;

size_t OrderBookSessionSimulator::countOrderBookMetricsSize(const std::vector<DecodedEntry>& entries){
    return std::ranges::count_if(
        entries,
        [](const DecodedEntry& e) {
            return std::visit([](auto const& x) { return x.isLast; }, e);
        });
}

py::dict OrderBookSessionSimulator::computeVariables(const std::string &csvPath, const std::vector<std::string> &variables) {
    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;
    ptrEntries.reserve(entries.size());
    for (DecodedEntry &entry: entries) { ptrEntries.push_back(&entry);}

    GlobalMarketState globalMarketState(variables);
    OrderBookMetrics orderBookMetrics(variables, countOrderBookMetricsSize(entries));

    // const auto loopStart = std::chrono::steady_clock::now();

    for (DecodedEntry* p : ptrEntries) {
        globalMarketState.update(p);
        if (std::visit([](auto const& entry){return entry.isLast;}, *p)) {
            if (std::optional<OrderBookMetricsEntry> e = globalMarketState.countMarketStateMetricsByEntry(p)){
                orderBookMetrics.addOrderBookMetricsEntry(*e);
            }
        }
    }

    // const auto loopElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - loopStart).count();
    // std::cout << "loop elapsed: " << loopElapsed << " ms" << std::endl;

    std::vector<DecodedEntry>().swap(entries);
    std::vector<DecodedEntry*>().swap(ptrEntries);

    orderBookMetrics.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");
    return orderBookMetrics.convertToNumpyArrays();
}

py::dict OrderBookSessionSimulator::computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const pybind11::object &python_callback) {

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) { ptrEntries.push_back(&entry); }
    const size_t orderBookMetricsEntrySize = countOrderBookMetricsSize(entries);

    GlobalMarketState globalMarketState(variables);
    OrderBookMetrics orderBookMetrics(variables, orderBookMetricsEntrySize);

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        if (std::visit([](auto const& entry){return entry.isLast;}, *p)) {
            if (std::optional<OrderBookMetricsEntry> e = globalMarketState.countMarketStateMetricsByEntry(p)) {
                orderBookMetrics.addOrderBookMetricsEntry(*e);
                python_callback(*e );
            }
        }
    }

    std::vector<DecodedEntry>().swap(entries);
    std::vector<DecodedEntry*>().swap(ptrEntries);

    return orderBookMetrics.convertToNumpyArrays();
}

OrderBook OrderBookSessionSimulator::computeFinalDepthSnapshot(const std::string &csvPath) {
    try {
        std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromSingleAssetParametersCSV(csvPath);
        std::vector<DecodedEntry*> ptrEntries;
        ptrEntries.reserve(entries.size());

        for (auto &entry : entries) { ptrEntries.push_back(&entry); }

        MarketState marketState;

        for (auto* entryPtr: ptrEntries) { marketState.update(entryPtr); }

        return std::move(marketState.orderBook);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return OrderBook{};
    }
}
