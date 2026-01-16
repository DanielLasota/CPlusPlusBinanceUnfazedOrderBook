#include <chrono>
#include <iostream>
// #include <windows.h>
#include <pybind11/pybind11.h>

#include "Simulator.h"
#include "CSVReader.h"
#include "GlobalMarketState.h"
#include "MarketState.h"
#include "MetricsStore.h"

py::dict Simulator::compute(
    const std::string& csvPath,
    const std::vector<std::string>& features,
    const ResampleConfig resampleCfg
) {
    // SetThreadAffinityMask(GetCurrentThread(), 0x0000FFFF);

    std::vector<XEvent> entries = CSVReader::readMultiAssetCSV(csvPath);
    std::vector<XEvent*> pEntries;
    pEntries.reserve(entries.size());
    for (XEvent &e: entries) { pEntries.push_back(&e);}

    GlobalMarketState gms(features, resampleCfg);
    MetricsStore store(features, countMetricsSize(entries));

    const auto startTs = std::chrono::steady_clock::now();

    for (auto *p: pEntries) {
        gms.update(p);

        if (std::visit([](auto const& e) { return e.isLast; }, *p)) {
            if (std::optional<MetricsEntry> me = gms.calcMetricsEntry(p)) {
                store.update(*me);
            }
        }
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTs).count();
    std::cout << "elapsed: " << elapsed << " ms" << std::endl;

    std::vector<XEvent>().swap(entries);
    std::vector<XEvent*>().swap(pEntries);

    // store.toCSV("C:/Users/daniel/Documents/orderBookMetrics/sample.csv");
    return store.toNp();
}

py::dict Simulator::backtest(
    const std::string& csvPath,
    const std::vector<std::string> &features,
    const ResampleConfig resampleCfg,
    const pybind11::object &callback
) {
    std::vector<XEvent> entries = CSVReader::readMultiAssetCSV(csvPath);
    std::vector<XEvent*> pEntries;
    pEntries.reserve(entries.size());
    for (auto &e : entries) { pEntries.push_back(&e); }

    GlobalMarketState gms(features, resampleCfg);
    MetricsStore store(features, countMetricsSize(entries));

    for (auto *p: pEntries) {
        gms.update(p);

        if (std::visit([](auto const& entry){ return entry.isLast; }, *p)) {
            if (std::optional<MetricsEntry> me = gms.calcMetricsEntry(p)) {
                store.update(*me);
                callback(*me );
            }
        }
    }

    std::vector<XEvent>().swap(entries);
    std::vector<XEvent*>().swap(pEntries);

    return store.toNp();
}

OrderBook Simulator::computeFinalOB(const std::string &csvPath) {
    try {
        std::vector<XEvent> entries = CSVReader::readSingleAssetCSV(csvPath);
        std::vector<XEvent*> pEntries;
        pEntries.reserve(entries.size());
        MarketState ms;

        for (auto &entry : entries) { pEntries.push_back(&entry); }

        for (auto* entryPtr: pEntries) { ms.update(entryPtr); }

        return std::move(ms.ob);

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
