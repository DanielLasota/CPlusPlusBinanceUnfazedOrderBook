#include <fstream>
#include <iostream>

#include "OrderBookMetrics.h"
#include <cstdlib>
#include <cstring>

namespace py = pybind11;

template<typename T>
static py::array_t<T> vector_to_numpy(const std::vector<T>& vec) {
    size_t n = vec.size();
    T* data = static_cast<T*>(std::malloc(n * sizeof(T)));
    std::memcpy(data, vec.data(), n * sizeof(T));

    py::capsule free_when_done(data, [](void* f){ std::free(f); });
    return py::array_t<T>(
        { n },
        { sizeof(T) },
        data,
        free_when_done
    );
}

py::dict OrderBookMetrics::convertToNumpyArrays() const {

    auto start0 = std::chrono::steady_clock::now();

    size_t n = entries_.size();

    std::vector<int64_t> col_timestampOfReceive; col_timestampOfReceive.reserve(n);
    std::vector<uint8_t> col_market; col_market.reserve(n);
    std::vector<uint8_t> col_symbol; col_symbol.reserve(n);
    std::vector<double> col_bestAskPrice; col_bestAskPrice.reserve(n);
    std::vector<double> col_bestBidPrice; col_bestBidPrice.reserve(n);
    std::vector<double> col_midPrice; col_midPrice.reserve(n);
    std::vector<double> col_bestVolumeImbalance; col_bestVolumeImbalance.reserve(n);
    std::vector<double> col_bestVolumeRatio; col_bestVolumeRatio.reserve(n);
    std::vector<double> col_bestTwoVolumeImbalance; col_bestTwoVolumeImbalance.reserve(n);
    std::vector<double> col_bestThreeVolumeImbalance; col_bestThreeVolumeImbalance.reserve(n);
    std::vector<double> col_bestFiveVolumeImbalance; col_bestFiveVolumeImbalance.reserve(n);
    std::vector<double> col_queueImbalance; col_queueImbalance.reserve(n);
    std::vector<double> col_volumeImbalance; col_volumeImbalance.reserve(n);
    std::vector<double> col_gap; col_gap.reserve(n);
    std::vector<uint8_t> col_isAggressorAsk; col_isAggressorAsk.reserve(n);
    std::vector<double> col_vwapDeviation; col_vwapDeviation.reserve(n);
    std::vector<double> col_simplifiedSlopeImbalance; col_simplifiedSlopeImbalance.reserve(n);
    std::vector<double> col_tradeCountImbalance1S; col_tradeCountImbalance1S.reserve(n);
    std::vector<double> col_cumulativeDelta10s; col_cumulativeDelta10s.reserve(n);


    for (auto const& e : entries_) {
        col_timestampOfReceive.push_back(e.timestampOfReceive);
        col_market.push_back(e.market);
        col_symbol.push_back(e.symbol);
        col_bestAskPrice.push_back(e.bestAskPrice);
        col_bestBidPrice.push_back(e.bestBidPrice);
        col_midPrice.push_back(e.midPrice);
        col_bestVolumeImbalance.push_back(e.bestVolumeImbalance);
        col_bestVolumeRatio.push_back(e.bestVolumeRatio);
        col_bestTwoVolumeImbalance.push_back(e.bestTwoVolumeImbalance);
        col_bestThreeVolumeImbalance.push_back(e.bestThreeVolumeImbalance);
        col_bestFiveVolumeImbalance.push_back(e.bestFiveVolumeImbalance);
        col_queueImbalance.push_back(e.queueImbalance);
        col_volumeImbalance.push_back(e.volumeImbalance);
        col_gap.push_back(e.gap);
        col_isAggressorAsk.push_back(static_cast<uint8_t>(e.isAggressorAsk));
        col_vwapDeviation.push_back(e.vwapDeviation);
        col_simplifiedSlopeImbalance.push_back(e.simplifiedSlopeImbalance);
        col_tradeCountImbalance1S.push_back(e.tradeCountImbalance1S);
        col_cumulativeDelta10s.push_back(e.cumulativeDelta10s);
    }

    auto arr_timestampOfReceive = vector_to_numpy<int64_t>(col_timestampOfReceive);
    auto arr_market = vector_to_numpy<uint8_t>(col_market);
    auto arr_symbol = vector_to_numpy<uint8_t>(col_symbol);
    auto arr_bestAskPrice = vector_to_numpy<double>(col_bestAskPrice);
    auto arr_bestBidPrice = vector_to_numpy<double>(col_bestBidPrice);
    auto arr_midPrice = vector_to_numpy<double>(col_midPrice);
    auto arr_bestVolumeImbalance  = vector_to_numpy<double>(col_bestVolumeImbalance);
    auto arr_bestVolumeRatio = vector_to_numpy<double>(col_bestVolumeRatio);
    auto arr_bestTwoVolumeImbalance = vector_to_numpy<double>(col_bestTwoVolumeImbalance);
    auto arr_bestThreeVolumeImbalance = vector_to_numpy<double>(col_bestThreeVolumeImbalance);
    auto arr_bestFiveVolumeImbalance = vector_to_numpy<double>(col_bestFiveVolumeImbalance);
    auto arr_queueImbalance = vector_to_numpy<double>(col_queueImbalance);
    auto arr_volumeImbalance = vector_to_numpy<double>(col_volumeImbalance);
    auto arr_gap = vector_to_numpy<double>(col_gap);
    auto arr_isAggressorAsk = vector_to_numpy<uint8_t>(col_isAggressorAsk);
    auto arr_vwapDeviation = vector_to_numpy<double>(col_vwapDeviation);
    auto arr_simplifiedSlopeImbalance = vector_to_numpy<double>(col_simplifiedSlopeImbalance);
    auto arr_tradeCountImbalance1S = vector_to_numpy<double>(col_tradeCountImbalance1S);
    auto arr_cumulativeDelta10s = vector_to_numpy<double>(col_cumulativeDelta10s);

    py::dict result;
    for (auto const& var : variables_) {
        if      (var == "timestampOfReceive") result["timestampOfReceive"] = arr_timestampOfReceive;
        else if (var == "market") result["market"] = arr_market;
        else if (var == "symbol") result["symbol"] = arr_symbol;
        else if (var == "bestAskPrice") result["bestAskPrice"] = arr_bestAskPrice;
        else if (var == "bestBidPrice") result["bestBidPrice"] = arr_bestBidPrice;
        else if (var == "midPrice") result["midPrice"] = arr_midPrice;
        else if (var == "bestVolumeImbalance") result["bestVolumeImbalance"] = arr_bestVolumeImbalance;
        else if (var == "bestVolumeRatio") result["bestVolumeRatio"] = arr_bestVolumeRatio;
        else if (var == "bestTwoVolumeImbalance") result["bestTwoVolumeImbalance"] = arr_bestTwoVolumeImbalance;
        else if (var == "bestThreeVolumeImbalance") result["bestThreeVolumeImbalance"] = arr_bestThreeVolumeImbalance;
        else if (var == "bestFiveVolumeImbalance") result["bestFiveVolumeImbalance"] = arr_bestFiveVolumeImbalance;
        else if (var == "queueImbalance") result["queueImbalance"] = arr_queueImbalance;
        else if (var == "volumeImbalance") result["volumeImbalance"] = arr_volumeImbalance;
        else if (var == "gap") result["gap"] = arr_gap;
        else if (var == "isAggressorAsk") result["isAggressorAsk"] = arr_isAggressorAsk;
        else if (var == "vwapDeviation") result["vwapDeviation"] = arr_vwapDeviation;
        else if (var == "simplifiedSlopeImbalance") result["simplifiedSlopeImbalance"] = arr_simplifiedSlopeImbalance;
        else if (var == "tradeCountImbalance1S") result["tradeCountImbalance1S"] = arr_tradeCountImbalance1S;
        else if (var == "cumulativeDelta10s") result["cumulativeDelta10s"] = arr_cumulativeDelta10s;
    }

    auto finish = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start0).count();
    std::cout << "numpy conversion: " << elapsed_ms << " ms" << std::endl;

    return result;
}

void OrderBookMetrics::toCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << path << std::endl;
        return;
    }

    for (size_t i = 0; i < variables_.size(); ++i) {
        file << variables_[i];
        if (i + 1 < variables_.size()) file << ",";
    }
    file << "\n";

    for (const auto &e : entries_) {
        for (size_t j = 0; j < variables_.size(); ++j) {
            const auto &var = variables_[j];
            if      (var == "timestampOfReceive")       file << e.timestampOfReceive;
            else if (var == "market")                   file << static_cast<int>(e.market);
            else if (var == "symbol")                   file << static_cast<int>(e.symbol);
            else if (var == "bestAskPrice")             file << e.bestAskPrice;
            else if (var == "bestBidPrice")             file << e.bestBidPrice;
            else if (var == "midPrice")                 file << e.midPrice;
            else if (var == "bestVolumeImbalance")      file << e.bestVolumeImbalance;
            else if (var == "bestVolumeRatio")          file << e.bestVolumeRatio;
            else if (var == "bestTwoVolumeImbalance")   file << e.bestTwoVolumeImbalance;
            else if (var == "bestThreeVolumeImbalance") file << e.bestThreeVolumeImbalance;
            else if (var == "bestFiveVolumeImbalance")  file << e.bestFiveVolumeImbalance;
            else if (var == "volumeImbalance")          file << e.volumeImbalance;
            else if (var == "queueImbalance")           file << e.queueImbalance;
            else if (var == "gap")                      file << e.gap;
            else if (var == "isAggressorAsk")           file << (e.isAggressorAsk ? "1" : "0");
            else if (var == "vwapDeviation")            file << e.vwapDeviation;
            else if (var == "simplifiedSlopeImbalance") file << e.simplifiedSlopeImbalance;
            else if (var == "tradeCountImbalance1S")    file << e.tradeCountImbalance1S;
            else if (var == "cumulativeDelta10s")       file << e.cumulativeDelta10s;

            if (j + 1 < variables_.size()) file << ",";
        }
        file << "\n";
    }

    file.close();
}
