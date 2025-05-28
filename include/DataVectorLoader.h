#pragma once

#include <vector>
#include <string>

#include "EntryDecoder.h"
#include "enums/AssetParameters.h"

class DataVectorLoader {
public:
    DataVectorLoader() = default;

    std::vector<std::string> splitLine(const std::string& line, char delimiter);
    static std::vector<DecodedEntry> getEntriesFromSingleAssetParametersCSV(const std::string &csvPath);

    static std::vector<DecodedEntry> getEntriesFromMultiAssetParametersCSV(const std::string &csvPath);

private:
};
