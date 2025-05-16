#pragma once

#include <string>
#include <vector>

#include "Market.h"
#include "StreamType.h"

struct AssetParameters {
    Market      market;
    StreamType  streamType;
    std::string symbol;
    std::string date;

    bool operator==(AssetParameters const& o) const noexcept {
        return market == o.market
            && streamType == o.streamType
            && symbol == o.symbol
            && date == o.date;
    }

    static AssetParameters decodeAssetParametersFromSingleCSVName(const std::string& csvPath);
    static std::vector<AssetParameters> decodeAssetParametersFromMergedCSVName(const std::string& csvPath);
};

inline std::ostream& operator<<(std::ostream& os, AssetParameters const& ap) {
    os << "Market: "      << ap.market
       << ", StreamType: "<< ap.streamType
       << ", Symbol: "    << ap.symbol
       << ", Date: "      << ap.date;
    return os;
}

namespace std {
    template<>
    struct hash<AssetParameters> {
        size_t operator()(AssetParameters const& ap) const noexcept {
            size_t h = std::hash<int>{}(int(ap.market));
            h ^= std::hash<int>{}(int(ap.streamType)) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
            h ^= std::hash<std::string>{}(ap.symbol) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
            h ^= std::hash<std::string>{}(ap.date) + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2);
            return h;
        }
    };
}
