#ifndef DATAVECTORLOADER_H
#define DATAVECTORLOADER_H

#include <enums/BinanceEntry.h>
#include <EntryDecoder.h>
#include <enums/AssetParameters.h>
#include <vector>
#include <string>

class DataVectorLoader {
public:
    DataVectorLoader() = default;
    
    static std::vector<BinanceEntry> getEntriesFromSingleAssetCSV(const std::string &csvPath, bool merged);

private:
    static std::vector<std::string> splitLine(const std::string &line, char delimiter);

    static AssetParameters decodeAssetParametersFromCSVName(const std::string &csvName);
};

#endif // DATAVECTORLOADER_H