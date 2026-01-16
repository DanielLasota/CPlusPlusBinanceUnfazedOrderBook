#pragma once

#include "enums/ResampleMethod.h"

struct ResampleConfig {
    const size_t resolutionUs = 0;
    const ResampleMethod method = ResampleMethod::NONE;
};
