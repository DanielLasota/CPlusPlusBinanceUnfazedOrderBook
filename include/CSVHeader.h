#pragma once

#include <array>
#include <string_view>
#include <vector>

enum CSVHeader {
    COL_TimestampOfReceiveUS,
    COL_Stream,
    COL_EventType,
    COL_EventTime,
    COL_TransactionTime,
    COL_Symbol,
    COL_FirstUpdateId,
    COL_FinalUpdateId,
    COL_FinalUpdateIdInLastStream,
    COL_IsAsk,
    COL_Price,
    COL_Quantity,
    COL_PSUnknownField,
    COL_TradeId,
    COL_IsBuyerMarketMaker,
    COL_MUnknownParameter,
    COL_XUnknownParameter,
    COL_TimestampOfRequest,
    COL_MessageOutputTime,
    COL_LastUpdateId,
    COL_StreamType,
    COL_Market,
    COL_IsLast,
    COL_ServiceId,
    COL_COUNT
};

constexpr std::array<std::string_view, COL_COUNT> CSVHeaderFieldNames = {{
    "TimestampOfReceiveUS",
    "Stream",
    "EventType",
    "EventTime",
    "TransactionTime",
    "Symbol",
    "FirstUpdateId",
    "FinalUpdateId",
    "FinalUpdateIdInLastStream",
    "IsAsk",
    "Price",
    "Quantity",
    "PSUnknownField",
    "TradeId",
    "IsBuyerMarketMaker",
    "MUnknownParameter",
    "XUnknownParameter",
    "TimestampOfRequest",
    "MessageOutputTime",
    "LastUpdateId",
    "StreamType",
    "Market",
    "IsLast",
    "ServiceId"
}};

using ColMap = std::array<int, COL_COUNT>;

std::vector<std::string_view> splitLineSV(std::string_view line, char delimiter = ',');

ColMap buildColMap(const std::vector<std::string_view>& headerTokens);
