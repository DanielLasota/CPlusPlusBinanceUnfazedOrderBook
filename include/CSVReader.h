#pragma once

#include <string>
#include <vector>

#include "EntryDecoder.h"

class CSVReader {
public:
    CSVReader() = default;

    std::vector<std::string> splitLine(const std::string& line, char delimiter);

    static std::vector<XEvent> readSingleAssetCSV(const std::string &csvPath);

    static std::vector<XEvent> readMultiAssetCSV(const std::string &csvPath);

};
