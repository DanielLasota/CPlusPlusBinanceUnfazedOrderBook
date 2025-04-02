#ifndef DATAVECTORLOADER_H
#define DATAVECTORLOADER_H

#include <enums/AssetParameters.h>
#include <vector>
#include <string>
#include "OrderBookEntry.h"

class DataVectorLoader {
public:
    DataVectorLoader() = default;
    
    static std::vector<OrderBookEntry> getOrderbookEntriesFromCSV(const std::string &csvPath);

private:
    static std::vector<std::string> split(const std::string &line, char delimiter);

    static AssetParameters decodeAssetParametersFromCSVName(const std::string &csvName);
};

#endif // DATAVECTORLOADER_H