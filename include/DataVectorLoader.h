#ifndef DATAVECTORLOADER_H
#define DATAVECTORLOADER_H

#include <EntryDecoder.h>
#include <enums/AssetParameters.h>
#include <enums/OrderBookEntry.h>
#include <vector>
#include <string>

class DataVectorLoader {
public:
    DataVectorLoader() = default;

    static std::vector<DecodedEntry> getEntriesFromSingleAssetParametersCSV(const std::string &csvPath);

    static std::vector<DecodedEntry> getEntriesFromMultiAssetParametersCSV(const std::string &csvPath);

private:
    static std::vector<std::string> splitLine(const std::string &line, char delimiter);

    static AssetParameters decodeAssetParametersFromCSVName(const std::string &csvName);
};

#endif // DATAVECTORLOADER_H