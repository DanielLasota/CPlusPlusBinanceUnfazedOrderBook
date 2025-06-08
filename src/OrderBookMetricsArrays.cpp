#include "OrderBookMetricsArrays.h"

void OrderBookMetricsArrays::reserve(size_t n) {
    timestampOfReceive_.reserve(n);
    market_.reserve(n);
    symbol_.reserve(n);
    bestAskPrice_.reserve(n);
    bestBidPrice_.reserve(n);
    midPrice_.reserve(n);
    bestVolumeImbalance_.reserve(n);
    queueImbalance_.reserve(n);
    volumeImbalance_.reserve(n);
    gap_.reserve(n);
    isAggressorAsk_.reserve(n);
}

void OrderBookMetricsArrays::addEntry(const OrderBookMetricsEntry &e) {
    timestampOfReceive_.push_back(e.timestampOfReceive);
    market_.push_back(static_cast<int>(e.market));
    symbol_.push_back(e.symbol);
    bestAskPrice_.push_back(e.bestAskPrice);
    bestBidPrice_.push_back(e.bestBidPrice);
    midPrice_.push_back(e.midPrice);
    bestVolumeImbalance_.push_back(e.bestVolumeImbalance);
    queueImbalance_.push_back(e.queueImbalance);
    volumeImbalance_.push_back(e.volumeImbalance);
    gap_.push_back(e.gap);
    isAggressorAsk_.push_back(static_cast<int8_t>(e.isAggressorAsk));
}

pybind11::dict OrderBookMetricsArrays::toNumpyDict(const std::vector<std::string> &variables) {
    namespace py = pybind11;
    py::dict result;
    for (const auto &var : variables) {
        if (var == "timestampOfReceive") {
            auto *v = new std::vector<int64_t>(std::move(timestampOfReceive_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<int64_t>*>(p);}));
        } else if (var == "market") {
            auto *v = new std::vector<int>(std::move(market_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<int>*>(p);}));
        } else if (var == "symbol") {
            py::list lst;
            for (auto &s : symbol_) lst.append(s);
            symbol_.clear();
            result[var.c_str()] = std::move(lst);
        } else if (var == "bestAskPrice") {
            auto *v = new std::vector<double>(std::move(bestAskPrice_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "bestBidPrice") {
            auto *v = new std::vector<double>(std::move(bestBidPrice_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "midPrice") {
            auto *v = new std::vector<double>(std::move(midPrice_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "bestVolumeImbalance") {
            auto *v = new std::vector<double>(std::move(bestVolumeImbalance_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "queueImbalance") {
            auto *v = new std::vector<double>(std::move(queueImbalance_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "volumeImbalance") {
            auto *v = new std::vector<double>(std::move(volumeImbalance_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "gap") {
            auto *v = new std::vector<double>(std::move(gap_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<double>*>(p);}));
        } else if (var == "isAggressorAsk") {
            auto *v = new std::vector<int8_t>(std::move(isAggressorAsk_));
            result[var.c_str()] = py::array(v->size(), v->data(), py::capsule(v, [](void *p){delete static_cast<std::vector<int8_t>*>(p);}));
        }
    }
    return result;
}

