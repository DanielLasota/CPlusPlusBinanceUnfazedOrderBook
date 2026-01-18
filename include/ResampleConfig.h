#pragma once

#include "enums/ResampleMethod.h"

struct ResampleConfig {
    size_t resolutionUs = 0;
    ResampleMethod method = ResampleMethod::NONE;

    ResampleConfig() = default;

    ResampleConfig(
        size_t resolutionUs,
        ResampleMethod method
    ):
        resolutionUs(resolutionUs),
        method(method)
        {}
};
