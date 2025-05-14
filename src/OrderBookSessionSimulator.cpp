#include <chrono>
#include <iostream>
#include <OrderBookMetricsCalculator.h>
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
    OrderBookMetricsCalculator calc(mask);

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptr_entries) {
        marketState.update(p);

        const bool isLast = std::visit([](auto const& entry){
                return entry.IsLast;
        }, *p);

        if (isLast == true) {
            if (auto m = calc.countMarketStateMetrics(marketState)) {
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

void OrderBookSessionSimulator::computeBacktest(const std::string& csvPath, std::vector<std::string> &variables, const py::object &python_callback) {
    std::vector<DecodedEntry> entries = DataVectorLoader::getEntriesFromMultiAssetParametersCSV(csvPath);
    std::vector<DecodedEntry*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) {
        ptrEntries.push_back(&entry);
    }

    MarketState marketState;
    MetricMask mask = parseMask(variables);
    OrderBookMetricsCalculator calc(mask);

    auto start = std::chrono::steady_clock::now();

    for (auto* p : ptrEntries) {
        marketState.update(p);

        const bool isLast = std::visit([](auto const& entry){
                return entry.IsLast;
        }, *p);

        if (isLast == true) {
            if (auto m = calc.countMarketStateMetrics(marketState)) {
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
