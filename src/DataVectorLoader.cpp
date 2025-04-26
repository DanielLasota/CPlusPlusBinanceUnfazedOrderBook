#include <CSVHeader.h>
#include <DataVectorLoader.h>
#include <enums/AssetParameters.h>
#include <EntryDecoder.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>


std::vector<DecodedEntry> DataVectorLoader::getEntriesFromSingleAssetParametersCSV(const std::string &csvPath) {

    AssetParameters assetParameters = decodeAssetParametersFromCSVName(csvPath);
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

AssetParameters DataVectorLoader::decodeAssetParametersFromCSVName(const std::string& csvPath) {
    std::string csvName = std::filesystem::path(csvPath).filename().string();

    std::string base = csvName;
    const std::string suffix = ".csv";
    if (base.size() >= suffix.size() &&
        base.compare(base.size() - suffix.size(), suffix.size(), suffix) == 0) {
        base = base.substr(0, base.size() - suffix.size());
    }

    Market market;
    if (base.find("usd_m_futures") != std::string::npos) {
        market = Market::USD_M_FUTURES;
    } else if (base.find("coin_m_futures") != std::string::npos) {
        market = Market::COIN_M_FUTURES;
    } else if (base.find("spot") != std::string::npos) {
        market = Market::SPOT;
    } else {
        throw std::invalid_argument("Unknown market in CSV name: " + base);
    }

    StreamType stream_type;
    if (base.find("difference_depth") != std::string::npos) {
        stream_type = StreamType::DIFFERENCE_DEPTH_STREAM;
    } else if (base.find("trade") != std::string::npos) {
        stream_type = StreamType::TRADE_STREAM;
    } else if (base.find("depth_snapshot") != std::string::npos) {
        stream_type = StreamType::DEPTH_SNAPSHOT;
    } else {
        throw std::invalid_argument("Unknown stream type in CSV name: " + base);
    }

    std::vector<std::string> parts = splitLine(base, '_');
    if (parts.size() < 3) {
        throw std::invalid_argument("CSV name format is incorrect: " + base);
    }

    std::string pair;
    if (market == Market::COIN_M_FUTURES) {
        if (parts.size() < 3) {
            throw std::invalid_argument("CSV name format is incorrect for COIN_M_FUTURES: " + base);
        }
        pair = parts[parts.size() - 3] + "_" + parts[parts.size() - 2];
    } else {
        pair = parts[parts.size() - 2];
    }

    std::string date = parts[parts.size() - 1];

    return AssetParameters { market, stream_type, pair, date };
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
