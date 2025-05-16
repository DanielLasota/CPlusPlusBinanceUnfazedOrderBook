#pragma once

#include <vector>
#include <string>

#include "EntryDecoder.h"
#include "enums/AssetParameters.h"

class DataVectorLoader {
public:
    DataVectorLoader() = default;

    static std::vector<DecodedEntry> getEntriesFromSingleAssetParametersCSV(const std::string &csvPath);

    static std::vector<DecodedEntry> getEntriesFromMultiAssetParametersCSV(const std::string &csvPath);

    static AssetParameters decodeAssetParametersFromSingleCSVName(const std::string &csvName);

    static std::vector<AssetParameters> decodeAssetParametersFromMergedCSVName(const std::string& csvPath);

private:
    static std::vector<std::string> splitLine(const std::string &line, char delimiter);
};
