#include <chrono>
#include <iostream>
// #include <windows.h>
#include <pybind11/pybind11.h>

#include "Simulator.h"
#include "CSVReader.h"
#include "GlobalMarketState.h"
#include "MarketState.h"
#include "MetricsStore.h"

py::dict Simulator::compute(const std::string &csvPath, const std::vector<std::string> &features) {
    // SetThreadAffinityMask(GetCurrentThread(), 0x0000FFFF);

    std::vector<XEvent> entries = CSVReader::readMultiAssetCSV(csvPath);
    std::vector<XEvent*> ptrEntries;
    ptrEntries.reserve(entries.size());
    for (XEvent &e: entries) { ptrEntries.push_back(&e);}

    GlobalMarketState globalMarketState(features);
    MetricsStore metricsStore(features, countMetricsSize(entries));

    // const auto startTs = std::chrono::steady_clock::now();

    for (XEvent* p : ptrEntries) {
        globalMarketState.update(p);
        if (std::visit([](auto const& entry){return entry.isLast;}, *p)) {
            if (std::optional<MetricsEntry> e = globalMarketState.calcMetricsEntry(p)){
                metricsStore.update(*e);
            }
        }
    }

    // const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTs).count();
    // std::cout << "elapsed: " << elapsed << " ms" << std::endl;

    std::vector<XEvent>().swap(entries);
    std::vector<XEvent*>().swap(ptrEntries);

    // metricsStore.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");
    return metricsStore.toNp();
}

py::dict Simulator::backtest(const std::string& csvPath, const std::vector<std::string> &features, const pybind11::object &callback) {

    std::vector<XEvent> entries = CSVReader::readMultiAssetCSV(csvPath);
    std::vector<XEvent*> ptrEntries;

    ptrEntries.reserve(entries.size());
    for (auto &entry : entries) { ptrEntries.push_back(&entry); }
    const size_t metricsSize = countMetricsSize(entries);

    GlobalMarketState globalMarketState(features);
    MetricsStore metricsStore(features, metricsSize);

    for (auto* p : ptrEntries) {
        globalMarketState.update(p);

        if (std::visit([](auto const& entry){return entry.isLast;}, *p)) {
            if (std::optional<MetricsEntry> e = globalMarketState.calcMetricsEntry(p)) {
                metricsStore.update(*e);
                callback(*e );
            }
        }
    }

    std::vector<XEvent>().swap(entries);
    std::vector<XEvent*>().swap(ptrEntries);

    return metricsStore.toNp();
}

OrderBook Simulator::computeFinalOB(const std::string &csvPath) {
    try {
        std::vector<XEvent> entries = CSVReader::readSingleAssetCSV(csvPath);
        std::vector<XEvent*> ptrEntries;
        ptrEntries.reserve(entries.size());
        MarketState marketState;

        for (auto &entry : entries) { ptrEntries.push_back(&entry); }

        for (auto* entryPtr: ptrEntries) { marketState.update(entryPtr); }

        return std::move(marketState.orderBook);

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return OrderBook{};
    }
}

size_t Simulator::countMetricsSize(const std::vector<XEvent>& entries){
    return std::ranges::count_if(
        entries,
        [](const XEvent& e) {
            return std::visit([](auto const& x) { return x.isLast; }, e);
        });
}
