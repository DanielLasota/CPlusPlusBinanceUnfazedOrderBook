#include <sstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>

#include "enums/AssetParameters.h"
#include "EntryDecoder.h"

std::vector<std::string> splitLine(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

AssetParameters AssetParameters::decodeAssetParametersFromSingleCSVName(const std::string& csvPath) {
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
