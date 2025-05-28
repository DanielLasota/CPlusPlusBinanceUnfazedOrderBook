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

static Market parse_market(const std::string &s) {
    if (s == "spot")             return Market::SPOT;
    if (s == "usd_m_futures")    return Market::USD_M_FUTURES;
    if (s == "coin_m_futures")   return Market::COIN_M_FUTURES;
    throw std::invalid_argument("Unknown market: " + s);
}

static StreamType parse_stream(const std::string &s) {
    if (s == "difference_depth_stream") return StreamType::DIFFERENCE_DEPTH_STREAM;
    if (s == "depth_snapshot")          return StreamType::DEPTH_SNAPSHOT;
    if (s == "trade_stream")            return StreamType::TRADE_STREAM;
    throw std::invalid_argument("Unknown stream type: " + s);
}

std::vector<AssetParameters> AssetParameters::decodeAssetParametersFromMergedCSVName(const std::string &csvPath) {
    namespace fs = std::filesystem;

    std::string name = fs::path(csvPath).stem().string();
    const std::string prefix = "binance_merged_";
    if (name.rfind(prefix, 0) != 0)
        throw std::invalid_argument("Niepoprawna nazwa merged CSV: " + name);
    std::string rem = name.substr(prefix.size());

    static const std::vector<std::string> known_streams = {
        "depth_snapshot",
        "difference_depth_stream",
        "trade_stream"
    };
    static const std::vector<std::string> known_markets = {
        "usd_m_futures",
        "coin_m_futures",
        "spot"
    };

    std::vector<std::string> streams;
    size_t pos = 0;
    while (pos < rem.size()) {
        bool matched = false;
        for (auto const &st : known_streams) {
            if (rem.compare(pos, st.size(), st) == 0) {
                streams.push_back(st);
                pos += st.size();
                if (pos < rem.size() && rem[pos] == '_') ++pos;
                matched = true;
                break;
            }
        }
        if (!matched) break;
    }

    std::vector<std::string> markets;
    while (pos < rem.size()) {
        bool matched = false;
        for (auto const &mk : known_markets) {
            if (rem.compare(pos, mk.size(), mk) == 0) {
                markets.push_back(mk);
                pos += mk.size();
                if (pos < rem.size() && rem[pos] == '_') ++pos;
                matched = true;
                break;
            }
        }
        if (!matched) break;
    }

    std::string tail = rem.substr(pos);
    if (tail.size() < 1 || tail[0] == '_') tail.erase(0, 1);

    auto last_us = tail.rfind('_');
    if (last_us == std::string::npos)
        throw std::invalid_argument("Brak daty w merged CSV name: " + name);
    std::string date      = tail.substr(last_us+1);       // e.g. "14-04-2025"
    std::string pairs_str = tail.substr(0, last_us);      // e.g. "adausdt_trxusdt"

    std::vector<std::string> pairs;
    {
      std::istringstream iss(pairs_str);
      std::string tok;
      while (std::getline(iss, tok, '_')) {
        if (!tok.empty()) pairs.push_back(tok);
      }
    }

    std::vector<AssetParameters> result;
    result.reserve(streams.size() * markets.size() * pairs.size());
    for (auto const &st : streams) {
      for (auto const &mk : markets) {
        for (auto const &sym : pairs) {
          AssetParameters ap;
          ap.market     = parse_market(mk);
          ap.streamType = parse_stream(st);
          ap.symbol     = sym;
          ap.date       = date;
          result.push_back(ap);
        }
      }
    }

    return result;
}
