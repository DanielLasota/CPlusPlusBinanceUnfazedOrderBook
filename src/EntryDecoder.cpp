#include <stdexcept>
#include <charconv>
#include <vector>

#include "EntryDecoder.h"
#include "CSVHeader.h"


inline int64_t parse_int(std::string_view sv) {
    int64_t out = 0;
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), out);
    if (res.ec != std::errc()) throw std::runtime_error("parse_int error");
    return out;
}

inline double parse_double(std::string_view sv) {
    double out = 0;
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), out);
    if (res.ec != std::errc()) throw std::runtime_error("parse_double error");
    return out;
}

DecodedEntry EntryDecoder::decodeSingleAssetParameterEntry(const AssetParameters &params, std::string_view line) {
    auto tokens = splitLineSV(line, ',');

    if (params.streamType == StreamType::TRADE_STREAM) {
        if (params.market == Market::SPOT) {
            return TradeEntry(
                parse_int(tokens[0]),
                std::string(tokens[1]),
                std::string(tokens[2]),
                parse_int(tokens[3]),
                parse_int(tokens[4]),
                std::string(tokens[5]),
                parse_int(tokens[6]),
                parse_double(tokens[7]),
                parse_double(tokens[8]),
                tokens[9] == "1",
                std::string(tokens[10]),
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES || params.market == Market::COIN_M_FUTURES) {
            return TradeEntry(
                parse_int(tokens[0]),
                std::string(tokens[1]),
                std::string(tokens[2]),
                parse_int(tokens[3]),
                parse_int(tokens[4]),
                std::string(tokens[5]),
                parse_int(tokens[6]),
                parse_double(tokens[7]),
                parse_double(tokens[8]),
                tokens[9] == "1",
                "",
                std::string(tokens[10]),
                false,
                params.market
            );
        }
        else {
            throw std::runtime_error("Unknown market for trade decoding");
        }
    }
    else if (params.streamType == StreamType::DEPTH_SNAPSHOT) {
        if (params.market == Market::SPOT) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                "",
                "",
                0,
                0,
                "",
                0,
                0,
                0,
                (tokens[3] == "1"),
                parse_double(tokens[4]),
                parse_double(tokens[5]),
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                "",
                "",
                0,
                parse_int(tokens[3]),
                "", // Symbol
                0,
                0,
                0,
                (tokens[5] == "1"),
                parse_double(tokens[6]),
                parse_double(tokens[7]),
                "",
                false,
                Market::USD_M_FUTURES
            );
        }
        else if (params.market == Market::COIN_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                "",
                "",
                0,
                parse_int(tokens[3]),
                std::string(tokens[5]),
                0,
                0,
                0,
                (tokens[7] == "1"),
                parse_double(tokens[8]),
                parse_double(tokens[9]),
                "",
                false,
                Market::COIN_M_FUTURES
            );
        }
        else {
            throw std::runtime_error("Unknown market for snapshot decoding");
        }
    }
    else if (params.streamType == StreamType::DIFFERENCE_DEPTH_STREAM) {
        if (params.market == Market::SPOT) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                std::string(tokens[1]),
                std::string(tokens[2]),
                parse_int(tokens[3]),
                0,
                std::string(tokens[4]),
                parse_int(tokens[5]),
                parse_int(tokens[6]),
                0,
                (tokens[7] == "1"),
                parse_double(tokens[8]),
                parse_double(tokens[9]),
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                std::string(tokens[1]),
                std::string(tokens[2]),
                parse_int(tokens[3]),
                parse_int(tokens[4]),
                std::string(tokens[5]),
                parse_int(tokens[6]),
                parse_int(tokens[7]),
                parse_int(tokens[8]),
                (tokens[9] == "1"),
                parse_double(tokens[10]),
                parse_double(tokens[11]),
                "",
                false,
                Market::USD_M_FUTURES
            );
        }
        else if (params.market == Market::COIN_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                std::string(tokens[1]),
                std::string(tokens[2]),
                parse_int(tokens[3]),
                parse_int(tokens[4]),
                std::string(tokens[5]),
                parse_int(tokens[6]),
                parse_int(tokens[7]),
                parse_int(tokens[8]),
                (tokens[9] == "1"),
                parse_double(tokens[10]),
                parse_double(tokens[11]),
                std::string(tokens[12]),
                false,
                Market::COIN_M_FUTURES
            );
        }
        else {
            throw std::runtime_error("Unknown Market during decoding OrderBookEntry");
        }
    }
    else {
        throw std::runtime_error("Unknown StreamType in decodeSingleAssetParameterEntry");
    }
}

static StreamType parseStreamType(std::string_view s) {
    if (s == "TRADE_STREAM") return StreamType::TRADE_STREAM;
    if (s == "DIFFERENCE_DEPTH_STREAM") return StreamType::DIFFERENCE_DEPTH_STREAM;
    if (s == "DEPTH_SNAPSHOT") return StreamType::DEPTH_SNAPSHOT;
    if (s == "FINAL_DEPTH_SNAPSHOT") return StreamType::DEPTH_SNAPSHOT;
    throw std::invalid_argument("Unknown StreamType: " + std::string(s));
}

static Market parseMarket(std::string_view s) {
    if (s == "SPOT") return Market::SPOT;
    if (s == "USD_M_FUTURES") return Market::USD_M_FUTURES;
    if (s == "COIN_M_FUTURES")return Market::COIN_M_FUTURES;
    throw std::invalid_argument("Unknown Market: " + std::string(s));
}

DecodedEntry EntryDecoder::decodeMultiAssetParameterEntry(std::string_view line, const ColMap& h) {
    auto tokens = splitLineSV(line, ',');

    StreamType st = parseStreamType(tokens[h[COL_StreamType]]);
    Market     mk = parseMarket(tokens[h[COL_Market]]);

    switch (st) {
        case StreamType::TRADE_STREAM: {
            switch (mk) {
                case Market::SPOT: {
                    return TradeEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        std::string(tokens[h[COL_Stream]]),
                        std::string(tokens[h[COL_EventType]]),
                        parse_int(tokens[h[COL_EventTime]]),
                        parse_int(tokens[h[COL_TransactionTime]]),
                        std::string(tokens[h[COL_Symbol]]),
                        parse_int(tokens[h[COL_TradeId]]),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsBuyerMarketMaker]] == "1"),
                        std::string(tokens[h[COL_MUnknownParameter]]),
                        "",
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES:
                case Market::COIN_M_FUTURES: {
                    return TradeEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        std::string(tokens[h[COL_Stream]]),
                        std::string(tokens[h[COL_EventType]]),
                        parse_int(tokens[h[COL_EventTime]]),
                        parse_int(tokens[h[COL_TransactionTime]]),
                        std::string(tokens[h[COL_Symbol]]),
                        parse_int(tokens[h[COL_TradeId]]),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsBuyerMarketMaker]] == "1"),
                        "",
                        std::string(tokens[h[COL_XUnknownParameter]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        mk
                    );
                }
            }
        }
        case StreamType::DEPTH_SNAPSHOT: {
            switch (mk) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        "",
                        "",
                        0,
                        0,
                        std::string(tokens[h[COL_Symbol]]),
                        0,
                        0,
                        0,
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        "",
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES:
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        "",
                        "",
                        0,
                        parse_int(tokens[h[COL_TransactionTime]]),
                        std::string(tokens[h[COL_Symbol]]),
                        0,
                        0,
                        0,
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        "",
                        (tokens[h[COL_IsLast]] == "1"),
                        mk
                    );
                }
            }
        }
        case StreamType::DIFFERENCE_DEPTH_STREAM: {
            switch (mk) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        std::string(tokens[h[COL_Stream]]),
                        std::string(tokens[h[COL_EventType]]),
                        parse_int(tokens[h[COL_EventTime]]),
                        0,
                        std::string(tokens[h[COL_Symbol]]),
                        parse_int(tokens[h[COL_FirstUpdateId]]),
                        parse_int(tokens[h[COL_FinalUpdateId]]),
                        0,
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        "",
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                            return DifferenceDepthEntry(
                                parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                                std::string(tokens[h[COL_Stream]]),
                                std::string(tokens[h[COL_EventType]]),
                                parse_int(tokens[h[COL_EventTime]]),
                                parse_int(tokens[h[COL_TransactionTime]]),
                                std::string(tokens[h[COL_Symbol]]),
                                parse_int(tokens[h[COL_FirstUpdateId]]),
                                parse_int(tokens[h[COL_FinalUpdateId]]),
                                parse_int(tokens[h[COL_FinalUpdateIdInLastStream]]),
                                (tokens[h[COL_IsAsk]] == "1"),
                                parse_double(tokens[h[COL_Price]]),
                                parse_double(tokens[h[COL_Quantity]]),
                                "",
                                (tokens[h[COL_IsLast]] == "1"),
                                Market::USD_M_FUTURES
                            );
                }
                case Market::COIN_M_FUTURES: {
                            return DifferenceDepthEntry(
                                parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                                std::string(tokens[h[COL_Stream]]),
                                std::string(tokens[h[COL_EventType]]),
                                parse_int(tokens[h[COL_EventTime]]),
                                parse_int(tokens[h[COL_TransactionTime]]),
                                std::string(tokens[h[COL_Symbol]]),
                                parse_int(tokens[h[COL_FirstUpdateId]]),
                                parse_int(tokens[h[COL_FinalUpdateId]]),
                                parse_int(tokens[h[COL_FinalUpdateIdInLastStream]]),
                                (tokens[h[COL_IsAsk]] == "1"),
                                parse_double(tokens[h[COL_Price]]),
                                parse_double(tokens[h[COL_Quantity]]),
                                h[COL_PSUnknownField] != -1 ? std::string(tokens[h[COL_PSUnknownField]]) : "",
                                (tokens[h[COL_IsLast]] == "1"),
                                Market::COIN_M_FUTURES
                            );
                }
            }
        }
    }
    throw std::runtime_error("decodeMultiAssetParameterEntry: Unhandled StreamType/Market");
}