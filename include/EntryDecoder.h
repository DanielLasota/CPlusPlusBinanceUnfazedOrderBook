//
// Created by daniel on 2.04.2025.
//

#ifndef ENTRYDECODER_H
#define ENTRYDECODER_H

#include <CSVHeader.h>
#include <string>
#include <vector>
#include <variant>
#include "enums/AssetParameters.h"
#include "enums/OrderBookEntry.h"
#include "enums/TradeEntry.h"

using DecodedEntry = std::variant<DifferenceDepthEntry, TradeEntry>;

class EntryDecoder {
public:
    static DecodedEntry decodeSingleAssetParameterEntry(const AssetParameters &params, const std::string &line);

    static DecodedEntry decodeMultiAssetParameterEntry(const std::string &line, const CSVHeader &h);
private:
    static std::vector<std::string> splitLine(const std::string &line, char delimiter);
};

#endif // ENTRYDECODER_H

