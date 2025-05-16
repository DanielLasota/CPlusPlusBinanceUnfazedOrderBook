#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>

#include "CSVHeader.h"
#include "DataVectorLoader.h"
#include "enums/AssetParameters.h"
#include "EntryDecoder.h"

std::vector<DecodedEntry> DataVectorLoader::getEntriesFromSingleAssetParametersCSV(const std::string &csvPath) {

    AssetParameters assetParameters = AssetParameters::decodeAssetParametersFromSingleCSVName(csvPath);
    // std::cout << "Found Asset Parameters: " << assetParameters << std::endl;

    std::vector<DecodedEntry> entries;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku: " + csvPath);
    }

    std::string line;
    bool headerSkipped = false;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        try {
            DecodedEntry entry = EntryDecoder::decodeSingleAssetParameterEntry(assetParameters, line);
            entries.push_back(entry);

        } catch (const std::exception &e) {
            std::cerr << "Błąd przetwarzania linii: " << line << " - " << e.what() << std::endl;
        }
    }
    file.close();
    return entries;
}

std::vector<DecodedEntry> DataVectorLoader::getEntriesFromMultiAssetParametersCSV(const std::string &csvPath) {

    std::ifstream file(csvPath);
    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku: " + csvPath);
    }

    std::string headerLine;
    while (std::getline(file, headerLine)) {
        if (headerLine.empty() || headerLine[0] == '#')
            continue;
        break;
    }
    CSVHeader header(headerLine);

    std::vector<DecodedEntry> entries;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        try {
            DecodedEntry entry = EntryDecoder::decodeMultiAssetParameterEntry(line, header);
            entries.push_back(std::move(entry));
        }
        catch (const std::exception &e) {
            std::cerr << "Błąd przetwarzania linii: " << line
                      << " - " << e.what() << std::endl;
        }
    }

    return entries;
}

std::vector<std::string> DataVectorLoader::splitLine(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
