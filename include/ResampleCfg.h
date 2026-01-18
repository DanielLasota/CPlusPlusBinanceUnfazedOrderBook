#pragma once

#include "enums/ResampleMethod.h"

struct ResampleCfg {
    const size_t resolutionUs = 0;
    const ResampleMethod method = ResampleMethod::NONE;

    ResampleCfg() = default;

    ResampleCfg(
        const size_t resolutionUs,
        const ResampleMethod method
    ):
        resolutionUs(resolutionUs),
        method(method)
        {}
};
