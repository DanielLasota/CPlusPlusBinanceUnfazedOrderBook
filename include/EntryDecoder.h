#pragma once

#include <variant>
#include <string_view>
#include <array>

#include "CSVHeader.h"
#include "enums/AssetParameters.h"
#include "enums/DifferenceDepthEntry.h"
#include "enums/TradeEntry.h"

using ColMap = std::array<int, COL_COUNT>;

using DecodedEntry = std::variant<DifferenceDepthEntry, TradeEntry>;

class EntryDecoder {
public:
    static DecodedEntry decodeSingleAssetParameterEntry(const AssetParameters &params, std::string_view line);

    static DecodedEntry decodeMultiAssetParameterEntry(std::string_view line, const ColMap& colMap);
};
