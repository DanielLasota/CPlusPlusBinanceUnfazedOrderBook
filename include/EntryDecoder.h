#pragma once

#include <array>
#include <string_view>

#include "CSVHeader.h"
#include "enums/AssetParameters.h"
#include "enums/XEvent.h"

using ColMap = std::array<int, COL_COUNT>;

class EntryDecoder {
public:
    static XEvent decodeSingleAssetParameterEntry(const AssetParameters &params, std::string_view line);

    static XEvent decodeMultiAssetParameterEntry(std::string_view line, const ColMap& colMap);
};
