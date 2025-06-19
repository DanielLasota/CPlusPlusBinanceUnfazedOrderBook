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

    std::vector<double> col_tradeCountImbalance1Seconds; col_tradeCountImbalance1Seconds.reserve(n);
    std::vector<double> col_tradeCountImbalance3Seconds; col_tradeCountImbalance3Seconds.reserve(n);
    std::vector<double> col_tradeCountImbalance5Seconds; col_tradeCountImbalance5Seconds.reserve(n);
    std::vector<double> col_tradeCountImbalance10Seconds; col_tradeCountImbalance10Seconds.reserve(n);
    std::vector<double> col_tradeCountImbalance15Seconds; col_tradeCountImbalance15Seconds.reserve(n);
    std::vector<double> col_tradeCountImbalance30Seconds; col_tradeCountImbalance30Seconds.reserve(n);
    std::vector<double> col_tradeCountImbalance60Seconds; col_tradeCountImbalance60Seconds.reserve(n);

    std::vector<double> col_cumulativeDelta1Seconds; col_cumulativeDelta1Seconds.reserve(n);
    std::vector<double> col_cumulativeDelta3Seconds; col_cumulativeDelta3Seconds.reserve(n);
    std::vector<double> col_cumulativeDelta5Seconds; col_cumulativeDelta5Seconds.reserve(n);
    std::vector<double> col_cumulativeDelta10Seconds; col_cumulativeDelta10Seconds.reserve(n);
    std::vector<double> col_cumulativeDelta15Seconds; col_cumulativeDelta15Seconds.reserve(n);
    std::vector<double> col_cumulativeDelta30Seconds; col_cumulativeDelta30Seconds.reserve(n);
    std::vector<double> col_cumulativeDelta60Seconds; col_cumulativeDelta60Seconds.reserve(n);

    std::vector<double> col_priceDifference1Seconds; col_priceDifference1Seconds.reserve(n);
    std::vector<double> col_priceDifference3Seconds; col_priceDifference3Seconds.reserve(n);
    std::vector<double> col_priceDifference5Seconds; col_priceDifference5Seconds.reserve(n);
    std::vector<double> col_priceDifference10Seconds; col_priceDifference10Seconds.reserve(n);
    std::vector<double> col_priceDifference15Seconds; col_priceDifference15Seconds.reserve(n);
    std::vector<double> col_priceDifference30Seconds; col_priceDifference30Seconds.reserve(n);
    std::vector<double> col_priceDifference60Seconds; col_priceDifference60Seconds.reserve(n);

    std::vector<double> col_rateOfReturn1Seconds;   col_rateOfReturn1Seconds.reserve(n);
    std::vector<double> col_rateOfReturn3Seconds;   col_rateOfReturn3Seconds.reserve(n);
    std::vector<double> col_rateOfReturn5Seconds;   col_rateOfReturn5Seconds.reserve(n);
    std::vector<double> col_rateOfReturn10Seconds;  col_rateOfReturn10Seconds.reserve(n);
    std::vector<double> col_rateOfReturn15Seconds;  col_rateOfReturn15Seconds.reserve(n);
    std::vector<double> col_rateOfReturn30Seconds;  col_rateOfReturn30Seconds.reserve(n);
    std::vector<double> col_rateOfReturn60Seconds;  col_rateOfReturn60Seconds.reserve(n);

    std::vector<double> col_differenceDepthVolatilityImbalance1Seconds;   col_differenceDepthVolatilityImbalance1Seconds.reserve(n);
    std::vector<double> col_differenceDepthVolatilityImbalance3Seconds;   col_differenceDepthVolatilityImbalance3Seconds.reserve(n);
    std::vector<double> col_differenceDepthVolatilityImbalance5Seconds;   col_differenceDepthVolatilityImbalance5Seconds.reserve(n);
    std::vector<double> col_differenceDepthVolatilityImbalance10Seconds;  col_differenceDepthVolatilityImbalance10Seconds.reserve(n);
    std::vector<double> col_differenceDepthVolatilityImbalance15Seconds;  col_differenceDepthVolatilityImbalance15Seconds.reserve(n);
    std::vector<double> col_differenceDepthVolatilityImbalance30Seconds;  col_differenceDepthVolatilityImbalance30Seconds.reserve(n);
    std::vector<double> col_differenceDepthVolatilityImbalance60Seconds;  col_differenceDepthVolatilityImbalance60Seconds.reserve(n);

    std::vector<double> col_rsi5Seconds;                                  col_rsi5Seconds.reserve(n);
    std::vector<double> col_stochRsi5Seconds;                             col_stochRsi5Seconds.reserve(n);

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

        col_tradeCountImbalance1Seconds.push_back(e.tradeCountImbalance1Seconds);
        col_tradeCountImbalance3Seconds.push_back(e.tradeCountImbalance3Seconds);
        col_tradeCountImbalance5Seconds.push_back(e.tradeCountImbalance5Seconds);
        col_tradeCountImbalance10Seconds.push_back(e.tradeCountImbalance10Seconds);
        col_tradeCountImbalance15Seconds.push_back(e.tradeCountImbalance15Seconds);
        col_tradeCountImbalance30Seconds.push_back(e.tradeCountImbalance30Seconds);
        col_tradeCountImbalance60Seconds.push_back(e.tradeCountImbalance60Seconds);

        col_cumulativeDelta1Seconds.push_back(e.cumulativeDelta1Seconds);
        col_cumulativeDelta3Seconds.push_back(e.cumulativeDelta3Seconds);
        col_cumulativeDelta5Seconds.push_back(e.cumulativeDelta5Seconds);
        col_cumulativeDelta10Seconds.push_back(e.cumulativeDelta10Seconds);
        col_cumulativeDelta15Seconds.push_back(e.cumulativeDelta15Seconds);
        col_cumulativeDelta30Seconds.push_back(e.cumulativeDelta30Seconds);
        col_cumulativeDelta60Seconds.push_back(e.cumulativeDelta60Seconds);

        col_priceDifference1Seconds.push_back(e.priceDifference1Seconds);
        col_priceDifference3Seconds.push_back(e.priceDifference3Seconds);
        col_priceDifference5Seconds.push_back(e.priceDifference5Seconds);
        col_priceDifference10Seconds.push_back(e.priceDifference10Seconds);
        col_priceDifference15Seconds.push_back(e.priceDifference15Seconds);
        col_priceDifference30Seconds.push_back(e.priceDifference30Seconds);
        col_priceDifference60Seconds.push_back(e.priceDifference60Seconds);

        col_rateOfReturn1Seconds.push_back(e.rateOfReturn1Seconds);
        col_rateOfReturn3Seconds.push_back(e.rateOfReturn3Seconds);
        col_rateOfReturn5Seconds.push_back(e.rateOfReturn5Seconds);
        col_rateOfReturn10Seconds.push_back(e.rateOfReturn10Seconds);
        col_rateOfReturn15Seconds.push_back(e.rateOfReturn15Seconds);
        col_rateOfReturn30Seconds.push_back(e.rateOfReturn30Seconds);
        col_rateOfReturn60Seconds.push_back(e.rateOfReturn60Seconds);

        col_differenceDepthVolatilityImbalance1Seconds.push_back(e.differenceDepthVolatilityImbalance1Seconds);
        col_differenceDepthVolatilityImbalance3Seconds.push_back(e.differenceDepthVolatilityImbalance3Seconds);
        col_differenceDepthVolatilityImbalance5Seconds.push_back(e.differenceDepthVolatilityImbalance5Seconds);
        col_differenceDepthVolatilityImbalance10Seconds.push_back(e.differenceDepthVolatilityImbalance10Seconds);
        col_differenceDepthVolatilityImbalance15Seconds.push_back(e.differenceDepthVolatilityImbalance15Seconds);
        col_differenceDepthVolatilityImbalance30Seconds.push_back(e.differenceDepthVolatilityImbalance30Seconds);
        col_differenceDepthVolatilityImbalance60Seconds.push_back(e.differenceDepthVolatilityImbalance60Seconds);

        col_rsi5Seconds.push_back(e.rsi5Seconds);
        col_stochRsi5Seconds.push_back(e.stochRsi5Seconds);
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

    auto arr_tradeCountImbalance1Seconds = vector_to_numpy<double>(col_tradeCountImbalance1Seconds);
    auto arr_tradeCountImbalance3Seconds = vector_to_numpy<double>(col_tradeCountImbalance3Seconds);
    auto arr_tradeCountImbalance5Seconds = vector_to_numpy<double>(col_tradeCountImbalance5Seconds);
    auto arr_tradeCountImbalance10Seconds = vector_to_numpy<double>(col_tradeCountImbalance10Seconds);
    auto arr_tradeCountImbalance15Seconds = vector_to_numpy<double>(col_tradeCountImbalance15Seconds);
    auto arr_tradeCountImbalance30Seconds = vector_to_numpy<double>(col_tradeCountImbalance30Seconds);
    auto arr_tradeCountImbalance60Seconds = vector_to_numpy<double>(col_tradeCountImbalance60Seconds);

    auto arr_cumulativeDelta1Seconds = vector_to_numpy<double>(col_cumulativeDelta1Seconds);
    auto arr_cumulativeDelta3Seconds = vector_to_numpy<double>(col_cumulativeDelta3Seconds);
    auto arr_cumulativeDelta5Seconds = vector_to_numpy<double>(col_cumulativeDelta5Seconds);
    auto arr_cumulativeDelta10Seconds = vector_to_numpy<double>(col_cumulativeDelta10Seconds);
    auto arr_cumulativeDelta15Seconds = vector_to_numpy<double>(col_cumulativeDelta15Seconds);
    auto arr_cumulativeDelta30Seconds = vector_to_numpy<double>(col_cumulativeDelta30Seconds);
    auto arr_cumulativeDelta60Seconds = vector_to_numpy<double>(col_cumulativeDelta60Seconds);

    auto arr_priceDifference1Seconds = vector_to_numpy<double>(col_priceDifference1Seconds);
    auto arr_priceDifference3Seconds = vector_to_numpy<double>(col_priceDifference3Seconds);
    auto arr_priceDifference5Seconds = vector_to_numpy<double>(col_priceDifference5Seconds);
    auto arr_priceDifference10Seconds = vector_to_numpy<double>(col_priceDifference10Seconds);
    auto arr_priceDifference15Seconds = vector_to_numpy<double>(col_priceDifference15Seconds);
    auto arr_priceDifference30Seconds = vector_to_numpy<double>(col_priceDifference30Seconds);
    auto arr_priceDifference60Seconds = vector_to_numpy<double>(col_priceDifference60Seconds);

    auto arr_rateOfReturn1Seconds  = vector_to_numpy<double>(col_rateOfReturn1Seconds);
    auto arr_rateOfReturn3Seconds  = vector_to_numpy<double>(col_rateOfReturn3Seconds);
    auto arr_rateOfReturn5Seconds  = vector_to_numpy<double>(col_rateOfReturn5Seconds);
    auto arr_rateOfReturn10Seconds = vector_to_numpy<double>(col_rateOfReturn10Seconds);
    auto arr_rateOfReturn15Seconds = vector_to_numpy<double>(col_rateOfReturn15Seconds);
    auto arr_rateOfReturn30Seconds = vector_to_numpy<double>(col_rateOfReturn30Seconds);
    auto arr_rateOfReturn60Seconds = vector_to_numpy<double>(col_rateOfReturn60Seconds);

    auto arr_differenceDepthVolatilityImbalance1Seconds  = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance1Seconds);
    auto arr_differenceDepthVolatilityImbalance3Seconds  = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance3Seconds);
    auto arr_differenceDepthVolatilityImbalance5Seconds  = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance5Seconds);
    auto arr_differenceDepthVolatilityImbalance10Seconds = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance10Seconds);
    auto arr_differenceDepthVolatilityImbalance15Seconds = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance15Seconds);
    auto arr_differenceDepthVolatilityImbalance30Seconds = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance30Seconds);
    auto arr_differenceDepthVolatilityImbalance60Seconds = vector_to_numpy<double>(col_differenceDepthVolatilityImbalance60Seconds);

    auto arr_rsi5Seconds                                 = vector_to_numpy<double>(col_rsi5Seconds);
    auto arr_stochRsi5Seconds                                          = vector_to_numpy<double>(col_stochRsi5Seconds);

    py::dict result;
    for (auto const& var : variables_) {
        if      (var == "timestampOfReceive") result["timestampOfReceive"]                      = arr_timestampOfReceive;
        else if (var == "market") result["market"]                                              = arr_market;
        else if (var == "symbol") result["symbol"]                                              = arr_symbol;
        else if (var == "bestAskPrice") result["bestAskPrice"]                                  = arr_bestAskPrice;
        else if (var == "bestBidPrice") result["bestBidPrice"]                                  = arr_bestBidPrice;
        else if (var == "midPrice") result["midPrice"]                                          = arr_midPrice;
        else if (var == "bestVolumeImbalance") result["bestVolumeImbalance"]                    = arr_bestVolumeImbalance;
        else if (var == "bestVolumeRatio") result["bestVolumeRatio"]                            = arr_bestVolumeRatio;
        else if (var == "bestTwoVolumeImbalance") result["bestTwoVolumeImbalance"]              = arr_bestTwoVolumeImbalance;
        else if (var == "bestThreeVolumeImbalance") result["bestThreeVolumeImbalance"]          = arr_bestThreeVolumeImbalance;
        else if (var == "bestFiveVolumeImbalance") result["bestFiveVolumeImbalance"]            = arr_bestFiveVolumeImbalance;
        else if (var == "queueImbalance") result["queueImbalance"]                              = arr_queueImbalance;
        else if (var == "volumeImbalance") result["volumeImbalance"]                            = arr_volumeImbalance;
        else if (var == "gap") result["gap"]                                                    = arr_gap;
        else if (var == "isAggressorAsk") result["isAggressorAsk"]                              = arr_isAggressorAsk;
        else if (var == "vwapDeviation") result["vwapDeviation"]                                = arr_vwapDeviation;
        else if (var == "simplifiedSlopeImbalance") result["simplifiedSlopeImbalance"]          = arr_simplifiedSlopeImbalance;

        else if (var == "tradeCountImbalance1Seconds")  result["tradeCountImbalance1Seconds"]   = arr_tradeCountImbalance1Seconds;
        else if (var == "tradeCountImbalance3Seconds")  result["tradeCountImbalance3Seconds"]   = arr_tradeCountImbalance3Seconds;
        else if (var == "tradeCountImbalance5Seconds")  result["tradeCountImbalance5Seconds"]   = arr_tradeCountImbalance5Seconds;
        else if (var == "tradeCountImbalance10Seconds") result["tradeCountImbalance10Seconds"]  = arr_tradeCountImbalance10Seconds;
        else if (var == "tradeCountImbalance15Seconds") result["tradeCountImbalance15Seconds"]  = arr_tradeCountImbalance15Seconds;
        else if (var == "tradeCountImbalance30Seconds") result["tradeCountImbalance30Seconds"]  = arr_tradeCountImbalance30Seconds;
        else if (var == "tradeCountImbalance60Seconds") result["tradeCountImbalance60Seconds"]  = arr_tradeCountImbalance60Seconds;

        else if (var == "cumulativeDelta1Seconds")     result["cumulativeDelta1Seconds"]        = arr_cumulativeDelta1Seconds;
        else if (var == "cumulativeDelta3Seconds")     result["cumulativeDelta3Seconds"]        = arr_cumulativeDelta3Seconds;
        else if (var == "cumulativeDelta5Seconds")     result["cumulativeDelta5Seconds"]        = arr_cumulativeDelta5Seconds;
        else if (var == "cumulativeDelta10Seconds")    result["cumulativeDelta10Seconds"]       = arr_cumulativeDelta10Seconds;
        else if (var == "cumulativeDelta15Seconds")    result["cumulativeDelta15Seconds"]       = arr_cumulativeDelta15Seconds;
        else if (var == "cumulativeDelta30Seconds")    result["cumulativeDelta30Seconds"]       = arr_cumulativeDelta30Seconds;
        else if (var == "cumulativeDelta60Seconds")    result["cumulativeDelta60Seconds"]       = arr_cumulativeDelta60Seconds;

        else if (var == "priceDifference1Seconds")     result["priceDifference1Seconds"]        = arr_priceDifference1Seconds;
        else if (var == "priceDifference3Seconds")     result["priceDifference3Seconds"]        = arr_priceDifference3Seconds;
        else if (var == "priceDifference5Seconds")     result["priceDifference5Seconds"]        = arr_priceDifference5Seconds;
        else if (var == "priceDifference10Seconds")    result["priceDifference10Seconds"]       = arr_priceDifference10Seconds;
        else if (var == "priceDifference15Seconds")    result["priceDifference15Seconds"]       = arr_priceDifference15Seconds;
        else if (var == "priceDifference30Seconds")    result["priceDifference30Seconds"]       = arr_priceDifference30Seconds;
        else if (var == "priceDifference60Seconds")    result["priceDifference60Seconds"]       = arr_priceDifference60Seconds;

        else if (var == "rateOfReturn1Seconds")   result["rateOfReturn1Seconds"]                = arr_rateOfReturn1Seconds;
        else if (var == "rateOfReturn3Seconds")   result["rateOfReturn3Seconds"]                = arr_rateOfReturn3Seconds;
        else if (var == "rateOfReturn5Seconds")   result["rateOfReturn5Seconds"]                = arr_rateOfReturn5Seconds;
        else if (var == "rateOfReturn10Seconds")  result["rateOfReturn10Seconds"]               = arr_rateOfReturn10Seconds;
        else if (var == "rateOfReturn15Seconds")  result["rateOfReturn15Seconds"]               = arr_rateOfReturn15Seconds;
        else if (var == "rateOfReturn30Seconds")  result["rateOfReturn30Seconds"]               = arr_rateOfReturn30Seconds;
        else if (var == "rateOfReturn60Seconds")  result["rateOfReturn60Seconds"]               = arr_rateOfReturn60Seconds;

        else if (var == "differenceDepthVolatilityImbalance1Seconds")   result["differenceDepthVolatilityImbalance1Seconds"]                = arr_differenceDepthVolatilityImbalance1Seconds;
        else if (var == "differenceDepthVolatilityImbalance3Seconds")   result["differenceDepthVolatilityImbalance3Seconds"]                = arr_differenceDepthVolatilityImbalance3Seconds;
        else if (var == "differenceDepthVolatilityImbalance5Seconds")   result["differenceDepthVolatilityImbalance5Seconds"]                = arr_differenceDepthVolatilityImbalance5Seconds;
        else if (var == "differenceDepthVolatilityImbalance10Seconds")  result["differenceDepthVolatilityImbalance10Seconds"]               = arr_differenceDepthVolatilityImbalance10Seconds;
        else if (var == "differenceDepthVolatilityImbalance15Seconds")  result["differenceDepthVolatilityImbalance15Seconds"]               = arr_differenceDepthVolatilityImbalance15Seconds;
        else if (var == "differenceDepthVolatilityImbalance30Seconds")  result["differenceDepthVolatilityImbalance30Seconds"]               = arr_differenceDepthVolatilityImbalance30Seconds;
        else if (var == "differenceDepthVolatilityImbalance60Seconds")  result["differenceDepthVolatilityImbalance60Seconds"]               = arr_differenceDepthVolatilityImbalance60Seconds;

        else if (var == "rsi5Sseconds")                                 result["rsi5Sseconds"]                                              = arr_rsi5Seconds;
        else if (var == "stochRsi5Seconds")                             result["stochRsi5Seconds"]                                          = arr_stochRsi5Seconds;

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
            if      (var == "timestampOfReceive")           file << e.timestampOfReceive;
            else if (var == "market")                       file << static_cast<int>(e.market);
            else if (var == "symbol")                       file << static_cast<int>(e.symbol);
            else if (var == "bestAskPrice")                 file << e.bestAskPrice;
            else if (var == "bestBidPrice")                 file << e.bestBidPrice;
            else if (var == "midPrice")                     file << e.midPrice;
            else if (var == "bestVolumeImbalance")          file << e.bestVolumeImbalance;
            else if (var == "bestVolumeRatio")              file << e.bestVolumeRatio;
            else if (var == "bestTwoVolumeImbalance")       file << e.bestTwoVolumeImbalance;
            else if (var == "bestThreeVolumeImbalance")     file << e.bestThreeVolumeImbalance;
            else if (var == "bestFiveVolumeImbalance")      file << e.bestFiveVolumeImbalance;
            else if (var == "volumeImbalance")              file << e.volumeImbalance;
            else if (var == "queueImbalance")               file << e.queueImbalance;
            else if (var == "gap")                          file << e.gap;
            else if (var == "isAggressorAsk")               file << (e.isAggressorAsk ? "1" : "0");
            else if (var == "vwapDeviation")                file << e.vwapDeviation;
            else if (var == "simplifiedSlopeImbalance")     file << e.simplifiedSlopeImbalance;

            else if (var == "tradeCountImbalance1Seconds")  file << e.tradeCountImbalance1Seconds;
            else if (var == "tradeCountImbalance3Seconds")  file << e.tradeCountImbalance3Seconds;
            else if (var == "tradeCountImbalance5Seconds")  file << e.tradeCountImbalance5Seconds;
            else if (var == "tradeCountImbalance10Seconds") file << e.tradeCountImbalance10Seconds;
            else if (var == "tradeCountImbalance15Seconds") file << e.tradeCountImbalance15Seconds;
            else if (var == "tradeCountImbalance30Seconds") file << e.tradeCountImbalance30Seconds;
            else if (var == "tradeCountImbalance60Seconds") file << e.tradeCountImbalance60Seconds;

            else if (var == "cumulativeDelta1Seconds")      file << e.cumulativeDelta1Seconds;
            else if (var == "cumulativeDelta3Seconds")      file << e.cumulativeDelta3Seconds;
            else if (var == "cumulativeDelta5Seconds")      file << e.cumulativeDelta5Seconds;
            else if (var == "cumulativeDelta10Seconds")     file << e.cumulativeDelta10Seconds;
            else if (var == "cumulativeDelta15Seconds")     file << e.cumulativeDelta15Seconds;
            else if (var == "cumulativeDelta30Seconds")     file << e.cumulativeDelta30Seconds;
            else if (var == "cumulativeDelta60Seconds")     file << e.cumulativeDelta60Seconds;

            else if (var == "priceDifference1Seconds")      file << e.priceDifference1Seconds;
            else if (var == "priceDifference3Seconds")      file << e.priceDifference3Seconds;
            else if (var == "priceDifference5Seconds")      file << e.priceDifference5Seconds;
            else if (var == "priceDifference10Seconds")     file << e.priceDifference10Seconds;
            else if (var == "priceDifference15Seconds")     file << e.priceDifference15Seconds;
            else if (var == "priceDifference30Seconds")     file << e.priceDifference30Seconds;
            else if (var == "priceDifference60Seconds")     file << e.priceDifference60Seconds;

            else if (var == "rateOfReturn1Seconds")         file << e.rateOfReturn1Seconds;
            else if (var == "rateOfReturn3Seconds")         file << e.rateOfReturn3Seconds;
            else if (var == "rateOfReturn5Seconds")         file << e.rateOfReturn5Seconds;
            else if (var == "rateOfReturn10Seconds")        file << e.rateOfReturn10Seconds;
            else if (var == "rateOfReturn15Seconds")        file << e.rateOfReturn15Seconds;
            else if (var == "rateOfReturn30Seconds")        file << e.rateOfReturn30Seconds;
            else if (var == "rateOfReturn60Seconds")        file << e.rateOfReturn60Seconds;

            else if (var == "differenceDepthVolatilityImbalance1Seconds")         file << e.differenceDepthVolatilityImbalance1Seconds;
            else if (var == "differenceDepthVolatilityImbalance3Seconds")         file << e.differenceDepthVolatilityImbalance3Seconds;
            else if (var == "differenceDepthVolatilityImbalance5Seconds")         file << e.differenceDepthVolatilityImbalance5Seconds;
            else if (var == "differenceDepthVolatilityImbalance10Seconds")        file << e.differenceDepthVolatilityImbalance10Seconds;
            else if (var == "differenceDepthVolatilityImbalance15Seconds")        file << e.differenceDepthVolatilityImbalance15Seconds;
            else if (var == "differenceDepthVolatilityImbalance30Seconds")        file << e.differenceDepthVolatilityImbalance30Seconds;
            else if (var == "differenceDepthVolatilityImbalance60Seconds")        file << e.differenceDepthVolatilityImbalance60Seconds;

            else if (var == "rsi5Seconds")                                        file << e.rsi5Seconds;
            else if (var == "stochRsi5Seconds")                                   file << e.stochRsi5Seconds;

            if (j + 1 < variables_.size()) file << ",";
        }
        file << "\n";
    }

    file.close();
}
