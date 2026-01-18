#pragma once

#include <string>
#include <pybind11/pybind11.h>

#include "OrderBook.h"
#include "ResampleCfg.h"
#include "enums/XEvent.h"

namespace py = pybind11;

class Simulator {
public:
    Simulator() = delete;

    static py::dict compute(
        const std::string& csvPath,
        const std::vector<std::string>& features,
        const ResampleCfg& resampleCfg = {}
    );

    static py::dict backtest(
        const std::string& csvPath,
        const std::vector<std::string>& features,
        const ResampleCfg& resampleCfg = {},
        const py::object& callback = py::none()
    );

    static OrderBook computeFinalOB(const std::string& csvPath);

private:
    static size_t countMetricsSize(const std::vector<XEvent>& entries);
};
