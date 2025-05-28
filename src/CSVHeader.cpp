#include "CSVHeader.h"

ColMap buildColMap(const std::vector<std::string_view>& headerTokens) {
    ColMap colMap;
    colMap.fill(-1);
    for (size_t i = 0; i < headerTokens.size(); ++i) {
        for (size_t j = 0; j < CSVHeaderFieldNames.size(); ++j) {
            if (headerTokens[i] == CSVHeaderFieldNames[j]) {
                colMap[j] = static_cast<int>(i);
                break;
            }
        }
    }
    return colMap;
}

std::vector<std::string_view> splitLineSV(std::string_view line, char delimiter) {
    std::vector<std::string_view> tokens;
    size_t pos = 0;
    while (pos < line.size()) {
        size_t next = line.find(delimiter, pos);
        if (next == std::string_view::npos) {
            tokens.emplace_back(line.substr(pos));
            break;
        }
        tokens.emplace_back(line.substr(pos, next - pos));
        pos = next + 1;
    }
    return tokens;
}
