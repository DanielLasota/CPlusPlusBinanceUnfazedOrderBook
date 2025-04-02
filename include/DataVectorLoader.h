#ifndef DATAVECTORLOADER_H
#define DATAVECTORLOADER_H

#include <EntryDecoder.h>
#include <enums/AssetParameters.h>
#include <vector>
#include <string>
#include "enums/OrderBookEntry.h"

class DataVectorLoader {
public:
    DataVectorLoader() = default;
    
    static std::vector<OrderBookEntry> getEntriesFromCSV(const std::string &csvPath);

private:
    static std::vector<std::string> split(const std::string &line, char delimiter);

    static AssetParameters decodeAssetParametersFromCSVName(const std::string &csvName);
};

#endif // DATAVECTORLOADER_H