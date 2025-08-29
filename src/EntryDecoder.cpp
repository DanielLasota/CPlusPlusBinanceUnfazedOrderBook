#include <stdexcept>
#include <charconv>
#include <vector>

#include "EntryDecoder.h"
#include "CSVHeader.h"


[[nodiscard]] inline int64_t parse_int(std::string_view sv) {
    int64_t out = 0;
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), out, 10);
    if (res.ec == std::errc{}) return out;
    throw std::runtime_error(std::string("parse_int failed for '") + std::string(sv) + "'");
}

[[nodiscard]] inline double parse_double(std::string_view sv) {
    double out = 0.0;
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), out);
    if (res.ec == std::errc{}) return out;
    throw std::runtime_error(std::string("parse_double failed for '") + std::string(sv) + "'");
}

DecodedEntry EntryDecoder::decodeSingleAssetParameterEntry(const AssetParameters &params, std::string_view line) {
    auto tokens = splitLineSV(line, ',');

    switch (params.streamType) {
        case StreamType::TRADE_STREAM: {
            switch (params.market) {
                case Market::SPOT: {
                    return TradeEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(tokens.at(5)),
                        parse_double(tokens.at(7)),
                        parse_double(tokens.at(8)),
                        tokens.at(9) == "1",
                        false,
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES:
                case Market::COIN_M_FUTURES: {
                    return TradeEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(tokens.at(5)),
                        parse_double(tokens.at(7)),
                        parse_double(tokens.at(8)),
                        tokens.at(9) == "1",
                        false,
                        params.market
                    );
                }
                default:
                    throw std::runtime_error("decodeSingleAssetParameterEntry/TRADE_STREAM: Unknown market");
            }
        }

        case StreamType::DEPTH_SNAPSHOT: {
            switch (params.market) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(""),
                        (tokens.at(3) == "1"),
                        parse_double(tokens.at(4)),
                        parse_double(tokens.at(5)),
                        false,
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(""),
                        (tokens.at(5) == "1"),
                        parse_double(tokens.at(6)),
                        parse_double(tokens.at(7)),
                        false,
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(tokens.at(5)),
                        (tokens.at(7) == "1"),
                        parse_double(tokens.at(8)),
                        parse_double(tokens.at(9)),
                        false,
                        Market::COIN_M_FUTURES
                    );
                }
                default:
                    throw std::runtime_error("decodeSingleAssetParameterEntry/DEPTH_SNAPSHOT: Unknown market");
            }
        }

        case StreamType::DIFFERENCE_DEPTH_STREAM: {
            switch (params.market) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(tokens.at(4)),
                        (tokens.at(7) == "1"),
                        parse_double(tokens.at(8)),
                        parse_double(tokens.at(9)),
                        false,
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(tokens.at(5)),
                        (tokens.at(9) == "1"),
                        parse_double(tokens.at(10)),
                        parse_double(tokens.at(11)),
                        false,
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(0)),
                        parseSymbolFromName(tokens.at(5)),
                        (tokens.at(9) == "1"),
                        parse_double(tokens.at(10)),
                        parse_double(tokens.at(11)),
                        false,
                        Market::COIN_M_FUTURES
                    );
                }
                default:
                    throw std::runtime_error("decodeSingleAssetParameterEntry/DIFFERENCE_DEPTH_STREAM: Unknown market");
            }
        }

        default:
            throw std::runtime_error("decodeSingleAssetParameterEntry: Unknown StreamType");
    }
}

DecodedEntry EntryDecoder::decodeMultiAssetParameterEntry(std::string_view line, const ColMap& h) {
    auto tokens = splitLineSV(line, ',');

    StreamType streamType = parseStreamType(tokens.at(h.at(COL_StreamType))[0]);
    Market     market = parseMarket(tokens.at(h.at(COL_Market))[0]);

    switch (streamType) {
        case StreamType::TRADE_STREAM: {
            switch (market) {
                case Market::SPOT: {
                    return TradeEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsBuyerMarketMaker)) == "1"),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES:
                case Market::COIN_M_FUTURES: {
                    return TradeEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsBuyerMarketMaker)) == "1"),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        market
                    );
                }
            default:
                throw std::runtime_error("decodeMultiAssetParameterEntry/TRADE_STREAM: Unknown market");
            }
        }
        case StreamType::DEPTH_SNAPSHOT: {
            switch (market) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        (tokens.at(h.at(COL_IsAsk)) == "1"),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES:
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        (tokens.at(h.at(COL_IsAsk)) == "1"),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        market
                    );
                }
            default:
                throw std::runtime_error("decodeMultiAssetParameterEntry/DEPTH_SNAPSHOT: Unknown market");
            }
        }
        case StreamType::FINAL_DEPTH_SNAPSHOT:
        case StreamType::DIFFERENCE_DEPTH_STREAM: {
            switch (market) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        (tokens.at(h.at(COL_IsAsk)) == "1"),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        (tokens.at(h.at(COL_IsAsk)) == "1"),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        parse_int(tokens.at(h.at(COL_TimestampOfReceiveUS))),
                        parseSymbol(tokens.at(h.at(COL_Symbol))),
                        (tokens.at(h.at(COL_IsAsk)) == "1"),
                        parse_double(tokens.at(h.at(COL_Price))),
                        parse_double(tokens.at(h.at(COL_Quantity))),
                        (tokens.at(h.at(COL_IsLast)) == "1"),
                        Market::COIN_M_FUTURES
                    );
                }
            default:
                throw std::runtime_error("decodeMultiAssetParameterEntry/DIFF|FINAL_SNAPSHOT: Unknown market");
            }
        }
    default:
        throw std::runtime_error("decodeMultiAssetParameterEntry: Unknown StreamType");
    }
}
