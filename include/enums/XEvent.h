#pragma once

#include <variant>

#include "enums/DifferenceDepthEntry.h"
#include "enums/TradeEntry.h"

using XEvent = std::variant<DifferenceDepthEntry, TradeEntry>;
