#include <chrono>
#include <iostream>
#include <OrderBookMetricsCalculator.h>
#include <pybind11/pybind11.h>

#include "GlobalMarketState.h"
#include "DataVectorLoader.h"
#include "MarketState.h"
#include "OrderBookMetrics.h"
#include "OrderbookSessionSimulator.h"

OrderBookSessionSimulator::OrderBookSessionSimulator() {}

std::vector<OrderBookMetricsEntry> OrderBookSessionSimulator::computeVariables(const std::string &csvPath, std::vector<std::string> &variables) {

    std::vector<AssetParameters> assetParameters = AssetParameters::decodeAssetParametersFromMergedCSVName(csvPath);

    for (auto &ap : assetParameters)
    {
        if (ap.streamType == StreamType::DIFFERENCE_DEPTH_STREAM){
            std::cout << ap << std::endl;
        }
    }

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

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptr_entries) {
        globalMarketState.update(p);

        const bool isLast = std::visit([](auto const& entry){return entry.IsLast;}, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetrics(p)) {
                orderBookMetrics.addEntry(*m);
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

void OrderBookSessionSimulator::computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const pybind11::object &python_callback) {
    // std::vector<AssetParameters> assetParameters = AssetParameters::decodeAssetParametersFromMergedCSVName(csvPath);

    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) {
        ptrEntries.push_back(&entry);
    }

    MarketState marketState;
    MetricMask mask = parseMask(variables);
    GlobalMarketState globalMarketState(mask);

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptrEntries) {
        marketState.update(p);

        const bool isLast = std::visit([](auto const& entry){
                return entry.IsLast;
        }, *p);

        if (isLast == true) {
            if (auto m = globalMarketState.countMarketStateMetrics(p)) {
                python_callback(*m );
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

        return std::move(marketState.orderBook);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return OrderBook{};
    }
}
