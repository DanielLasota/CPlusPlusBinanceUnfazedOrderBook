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

    std::vector<int64_t>   col_ts;    col_ts.reserve(n);
    std::vector<uint8_t>   col_mk;    col_mk.reserve(n);
    std::vector<uint8_t>   col_sym;   col_sym.reserve(n);
    std::vector<double>    col_bap;   col_bap.reserve(n);
    std::vector<double>    col_bbp;   col_bbp.reserve(n);
    std::vector<double>    col_mid;   col_mid.reserve(n);
    std::vector<double>    col_bvi;   col_bvi.reserve(n);
    std::vector<double>    col_qi;    col_qi.reserve(n);
    std::vector<double>    col_vi;    col_vi.reserve(n);
    std::vector<double>    col_gap;   col_gap.reserve(n);
    std::vector<uint8_t>   col_agg;   col_agg.reserve(n);

    for (auto const& e : entries_) {
        col_ts .push_back(e.timestampOfReceive);
        col_mk .push_back(e.market);
        col_sym.push_back(e.symbol);
        col_bap.push_back(e.bestAskPrice);
        col_bbp.push_back(e.bestBidPrice);
        col_mid.push_back(e.midPrice);
        col_bvi.push_back(e.bestVolumeImbalance);
        col_qi .push_back(e.queueImbalance);
        col_vi .push_back(e.volumeImbalance);
        col_gap.push_back(e.gap);
        col_agg.push_back(static_cast<uint8_t>(e.isAggressorAsk));
    }

    auto arr_ts  = vector_to_numpy<int64_t>(col_ts);
    auto arr_mk  = vector_to_numpy<uint8_t>(col_mk);
    auto arr_sym = vector_to_numpy<uint8_t>(col_sym);
    auto arr_bap = vector_to_numpy<double>(col_bap);
    auto arr_bbp = vector_to_numpy<double>(col_bbp);
    auto arr_mid = vector_to_numpy<double>(col_mid);
    auto arr_bvi = vector_to_numpy<double>(col_bvi);
    auto arr_qi  = vector_to_numpy<double>(col_qi);
    auto arr_vi  = vector_to_numpy<double>(col_vi);
    auto arr_gap = vector_to_numpy<double>(col_gap);
    auto arr_agg = vector_to_numpy<uint8_t>(col_agg);

    py::dict result;
    for (auto const& var : variables_) {
        if      (var == "timestampOfReceive")      result["timestampOfReceive"]      = arr_ts;
        else if (var == "market")                  result["market"]                  = arr_mk;
        else if (var == "symbol")                  result["symbol"]                  = arr_sym;
        else if (var == "bestAskPrice")            result["bestAskPrice"]            = arr_bap;
        else if (var == "bestBidPrice")            result["bestBidPrice"]            = arr_bbp;
        else if (var == "midPrice")                result["midPrice"]                = arr_mid;
        else if (var == "bestVolumeImbalance")     result["bestVolumeImbalance"]     = arr_bvi;
        else if (var == "queueImbalance")          result["queueImbalance"]          = arr_qi;
        else if (var == "volumeImbalance")         result["volumeImbalance"]         = arr_vi;
        else if (var == "gap")                     result["gap"]                     = arr_gap;
        else if (var == "isAggressorAsk")          result["isAggressorAsk"]          = arr_agg;
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
            if      (var == "timestampOfReceive")  file << e.timestampOfReceive;
            else if (var == "market")              file << static_cast<int>(e.market);
            else if (var == "symbol")              file << static_cast<int>(e.symbol);
            else if (var == "bestAskPrice")        file << e.bestAskPrice;
            else if (var == "bestBidPrice")        file << e.bestBidPrice;
            else if (var == "midPrice")            file << e.midPrice;
            else if (var == "bestVolumeImbalance") file << e.bestVolumeImbalance;
            else if (var == "queueImbalance")      file << e.queueImbalance;
            else if (var == "volumeImbalance")     file << e.volumeImbalance;
            else if (var == "gap")                 file << e.gap;
            else if (var == "isAggressorAsk")      file << (e.isAggressorAsk ? "1" : "0");

            if (j + 1 < variables_.size()) file << ",";
        }
        file << "\n";
    }

    file.close();
}