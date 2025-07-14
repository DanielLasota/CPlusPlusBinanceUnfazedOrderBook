#pragma once

#include <array>
#include <string_view>
#include <vector>

enum CSVHeader {
    COL_TimestampOfReceiveUS,
    COL_Symbol,
    COL_IsAsk,
    COL_Price,
    COL_Quantity,
    COL_IsBuyerMarketMaker,
    COL_StreamType,
    COL_Market,
    COL_IsLast,
    COL_COUNT
};

constexpr std::array<std::string_view, COL_COUNT> CSVHeaderFieldNames = {{
    "TimestampOfReceiveUS",
    "Symbol",
    "IsAsk",
    "Price",
    "Quantity",
    "IsBuyerMarketMaker",
    "StreamType",
    "Market",
    "IsLast"
}};

using ColMap = std::array<int, COL_COUNT>;

std::vector<std::string_view> splitLineSV(std::string_view line, char delimiter = ',');

ColMap buildColMap(const std::vector<std::string_view>& headerTokens);
