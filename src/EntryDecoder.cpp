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
                parseSymbol(tokens[1]),
                parse_double(tokens[2]),
                parse_double(tokens[3]),
                tokens[4] == "1",
                std::string(tokens[5]),
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES || params.market == Market::COIN_M_FUTURES) {
            return TradeEntry(
                parse_int(tokens[0]),
                parseSymbol(tokens[1]),
                parse_double(tokens[2]),
                parse_double(tokens[3]),
                tokens[4] == "1",
                "",
                std::string(tokens[5]),
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
                parseSymbol(""),
                (tokens[3] == "1"),
                parse_double(tokens[4]),
                parse_double(tokens[5]),
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                parseSymbol(""), // Symbol
                (tokens[5] == "1"),
                parse_double(tokens[6]),
                parse_double(tokens[7]),
                false,
                Market::USD_M_FUTURES
            );
        }
        else if (params.market == Market::COIN_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                parseSymbol(tokens[5]),
                (tokens[7] == "1"),
                parse_double(tokens[8]),
                parse_double(tokens[9]),
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
                parseSymbol(tokens[4]),
                (tokens[7] == "1"),
                parse_double(tokens[8]),
                parse_double(tokens[9]),
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                parseSymbol(tokens[5]),
                (tokens[9] == "1"),
                parse_double(tokens[10]),
                parse_double(tokens[11]),
                false,
                Market::USD_M_FUTURES
            );
        }
        else if (params.market == Market::COIN_M_FUTURES) {
            return DifferenceDepthEntry(
                parse_int(tokens[0]),
                parseSymbol(tokens[5]),
                (tokens[9] == "1"),
                parse_double(tokens[10]),
                parse_double(tokens[11]),
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

DecodedEntry EntryDecoder::decodeMultiAssetParameterEntry(std::string_view line, const ColMap& h) {
    auto tokens = splitLineSV(line, ',');

    StreamType streamType = parseStreamType(tokens[h[COL_StreamType]][0]);
    Market     market = parseMarket(tokens[h[COL_Market]][0]);

    switch (streamType) {
        case StreamType::TRADE_STREAM: {
            switch (market) {
                case Market::SPOT: {
                    return TradeEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        parseSymbol(tokens[h[COL_Symbol]]),
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
                        parseSymbol(tokens[h[COL_Symbol]]),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsBuyerMarketMaker]] == "1"),
                        "",
                        std::string(tokens[h[COL_XUnknownParameter]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        market
                    );
                }
            }
        }
        case StreamType::DEPTH_SNAPSHOT: {
            switch (market) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        parseSymbol(tokens[h[COL_Symbol]]),
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES:
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        parseSymbol(tokens[h[COL_Symbol]]),
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        market
                    );
                }
            }
        }
        case StreamType::FINAL_DEPTH_SNAPSHOT:
        case StreamType::DIFFERENCE_DEPTH_STREAM: {
            switch (market) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        parseSymbol(tokens[h[COL_Symbol]]),
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        parseSymbol(tokens[h[COL_Symbol]]),
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens[h[COL_TimestampOfReceiveUS]]),
                        parseSymbol(tokens[h[COL_Symbol]]),
                        (tokens[h[COL_IsAsk]] == "1"),
                        parse_double(tokens[h[COL_Price]]),
                        parse_double(tokens[h[COL_Quantity]]),
                        (tokens[h[COL_IsLast]] == "1"),
                        Market::COIN_M_FUTURES
                    );
                }
            }
        }
    }
    throw std::runtime_error("decodeMultiAssetParameterEntry: Unhandled StreamType/Market");
}
