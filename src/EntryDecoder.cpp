#include <sstream>
#include <stdexcept>

#include "CSVHeader.h"
#include "EntryDecoder.h"

DecodedEntry EntryDecoder::decodeSingleAssetParameterEntry(const AssetParameters &params, const std::string &line) {
    auto tokens = splitLine(line, ',');

    if (params.streamType == StreamType::TRADE_STREAM) {
        if (params.market == Market::SPOT) {
            return TradeEntry(
                std::stoll(tokens[0]),
                tokens[1],
                tokens[2],
                std::stoll(tokens[3]),
                std::stoll(tokens[4]),
                tokens[5],
                std::stoll(tokens[6]),
                std::stod(tokens[7]),
                std::stod(tokens[8]),
                std::stoi(tokens[9]),
                tokens[10],
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES || params.market == Market::COIN_M_FUTURES) {
            return TradeEntry(
                std::stoll(tokens[0]),
                tokens[1],
                tokens[2],
                std::stoll(tokens[3]),
                std::stoll(tokens[4]),
                tokens[5],
                std::stoll(tokens[6]),
                std::stod(tokens[7]),
                std::stod(tokens[8]),
                std::stoi(tokens[9]),
                "",
                tokens[10],
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
                std::stoll(tokens[0]),
                "",
                "",
                0,
                0,
                "",
                0,
                0,
                0,
                (std::stoi(tokens[3]) != 0),
                std::stod(tokens[4]),
                std::stod(tokens[5]),
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES) {
            return DifferenceDepthEntry(
                std::stoll(tokens[0]),
                "",
                "",
                0,
                std::stoll(tokens[3]),
                "", // Symbol
                0,
                0,
                0,
                (std::stoi(tokens[5]) != 0),
                std::stod(tokens[6]),
                std::stod(tokens[7]),
                "",
                false,
                Market::USD_M_FUTURES
            );
        }
        else if (params.market == Market::COIN_M_FUTURES) {
            return DifferenceDepthEntry(
                std::stoll(tokens[0]),
                "",
                "",
                0,
                std::stoll(tokens[3]),
                tokens[5],
                0,
                0,
                0,
                (std::stoi(tokens[7]) != 0),
                std::stod(tokens[8]),
                std::stod(tokens[9]),
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
                std::stoll(tokens[0]),
                tokens[1],
                tokens[2],
                std::stoll(tokens[3]),
                0,
                tokens[4],
                std::stoll(tokens[5]),
                std::stoll(tokens[6]),
                0,
                (std::stoi(tokens[7]) != 0),
                std::stod(tokens[8]),
                std::stod(tokens[9]),
                "",
                false,
                Market::SPOT
            );
        }
        else if (params.market == Market::USD_M_FUTURES) {
            return DifferenceDepthEntry(
                std::stoll(tokens[0]),
                tokens[1],
                tokens[2],
                std::stoll(tokens[3]),
                std::stoll(tokens[4]),
                tokens[5],
                std::stoll(tokens[6]),
                std::stoll(tokens[7]),
                std::stoll(tokens[8]),
                (std::stoi(tokens[9]) != 0),
                std::stod(tokens[10]),
                std::stod(tokens[11]),
                "",
                false,
                Market::USD_M_FUTURES
            );
        }
        else if (params.market == Market::COIN_M_FUTURES) {
            return DifferenceDepthEntry(
                std::stoll(tokens[0]),
                tokens[1],
                tokens[2],
                std::stoll(tokens[3]),
                std::stoll(tokens[4]),
                tokens[5],
                std::stoll(tokens[6]),
                std::stoll(tokens[7]),
                std::stoll(tokens[8]),
                (std::stoi(tokens[9]) != 0),
                std::stod(tokens[10]),
                std::stod(tokens[11]),
                tokens[12],
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

static StreamType parseStreamType(const std::string &s) {
    if (s == "TRADE_STREAM") return StreamType::TRADE_STREAM;
    if (s == "DIFFERENCE_DEPTH_STREAM") return StreamType::DIFFERENCE_DEPTH_STREAM;
    if (s == "DEPTH_SNAPSHOT") return StreamType::DEPTH_SNAPSHOT;
    if (s == "FINAL_DEPTH_SNAPSHOT") return StreamType::DEPTH_SNAPSHOT;
    throw std::invalid_argument("Unknown StreamType: " + s);
}

static Market parseMarket(const std::string &s) {
    if (s == "SPOT") return Market::SPOT;
    if (s == "USD_M_FUTURES") return Market::USD_M_FUTURES;
    if (s == "COIN_M_FUTURES")return Market::COIN_M_FUTURES;
    throw std::invalid_argument("Unknown Market: " + s);
}

DecodedEntry EntryDecoder::decodeMultiAssetParameterEntry(const std::string &line, const CSVHeader &h) {
    auto tokens = splitLine(line, ',');

    StreamType st = parseStreamType(tokens[h["StreamType"]]);
    Market     mk = parseMarket(tokens[h["Market"]]);

    switch (st) {
        case StreamType::TRADE_STREAM: {
            switch (mk) {
                case Market::SPOT: {
                    return TradeEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        tokens[h["Stream"]],
                        tokens[h["EventType"]],
                        std::stoll(tokens[h["EventTime"]]),
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        std::stoll(tokens[h["TradeId"]]),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        (tokens[h["IsBuyerMarketMaker"]] == "1"),
                        tokens[h["MUnknownParameter"]],
                        "",
                        (tokens[h["IsLast"]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return TradeEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        tokens[h["Stream"]],
                        tokens[h["EventType"]],
                        std::stoll(tokens[h["EventTime"]]),
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        std::stoll(tokens[h["TradeId"]]),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        (tokens[h["IsBuyerMarketMaker"]] == "1"),
                        "",
                        tokens[h["XUnknownParameter"]],
                        (tokens[h["IsLast"]] == "1"),
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return TradeEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        tokens[h["Stream"]],
                        tokens[h["EventType"]],
                        std::stoll(tokens[h["EventTime"]]),
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        std::stoll(tokens[h["TradeId"]]),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        (tokens[h["IsBuyerMarketMaker"]] == "1"),
                        "",
                        tokens[h["XUnknownParameter"]],
                        (tokens[h["IsLast"]] == "1"),
                        Market::COIN_M_FUTURES
                    );
                }
            }
        }

        case StreamType::DEPTH_SNAPSHOT: {
            switch (mk) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        "",
                        "",
                        0,
                        0,
                        tokens[h["Symbol"]],
                        0,
                        0,
                        0,
                        (tokens[h["IsAsk"]] == "1"),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        "",
                        (tokens[h["IsLast"]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return DifferenceDepthEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        "",
                        "",
                        0,
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        0,
                        0,
                        0,
                        (tokens[h["IsAsk"]] == "1"),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        "",
                        (tokens[h["IsLast"]] == "1"),
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        "",
                        "",
                        0,
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        0,
                        0,
                        0,
                        (tokens[h["IsAsk"]] == "1"),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        "",
                        (tokens[h["IsLast"]] == "1"),
                        Market::COIN_M_FUTURES
                    );
                }
            }
        }

        case StreamType::DIFFERENCE_DEPTH_STREAM: {
            switch (mk) {
                case Market::SPOT: {
                    return DifferenceDepthEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        tokens[h["Stream"]],
                        tokens[h["EventType"]],
                        std::stoll(tokens[h["EventTime"]]),
                        0,
                        tokens[h["Symbol"]],
                        std::stoll(tokens[h["FirstUpdateId"]]),
                        std::stoll(tokens[h["FinalUpdateId"]]),
                        0,
                        (std::stoi(tokens[h["IsAsk"]]) != 0),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        "",
                        (tokens[h["IsLast"]] == "1"),
                        Market::SPOT
                    );
                }
                case Market::USD_M_FUTURES: {
                    return DifferenceDepthEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        tokens[h["Stream"]],
                        tokens[h["EventType"]],
                        std::stoll(tokens[h["EventTime"]]),
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        std::stoll(tokens[h["FirstUpdateId"]]),
                        std::stoll(tokens[h["FinalUpdateId"]]),
                        std::stoll(tokens[h["FinalUpdateIdInLastStream"]]),
                        (std::stoi(tokens[h["IsAsk"]]) != 0),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        "",
                        (tokens[h["IsLast"]] == "1"),
                        Market::USD_M_FUTURES
                    );
                }
                case Market::COIN_M_FUTURES: {
                    return DifferenceDepthEntry(
                        std::stoll(tokens[h["TimestampOfReceiveUS"]]),
                        tokens[h["Stream"]],
                        tokens[h["EventType"]],
                        std::stoll(tokens[h["EventTime"]]),
                        std::stoll(tokens[h["TransactionTime"]]),
                        tokens[h["Symbol"]],
                        std::stoll(tokens[h["FirstUpdateId"]]),
                        std::stoll(tokens[h["FinalUpdateId"]]),
                        std::stoll(tokens[h["FinalUpdateIdInLastStream"]]),
                        (std::stoi(tokens[h["IsAsk"]]) != 0),
                        std::stod(tokens[h["Price"]]),
                        std::stod(tokens[h["Quantity"]]),
                        tokens[h["PSUnknownField"]],
                        (tokens[h["IsLast"]] == "1"),
                        Market::COIN_M_FUTURES
                    );
                }
            }
        }
    }
    throw std::runtime_error("decodeMultiAssetParameterEntry: Unhandled StreamType/Market");
}

std::vector<std::string> EntryDecoder::splitLine(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
