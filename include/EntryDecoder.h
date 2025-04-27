#ifndef ENTRYDECODER_H
#define ENTRYDECODER_H

#include <string>
#include <vector>
#include <variant>

#include "CSVHeader.h"
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

