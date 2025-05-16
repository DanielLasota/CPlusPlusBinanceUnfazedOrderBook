#include <sstream>
#include <stdexcept>

#include "CSVHeader.h"
#include "EntryDecoder.h"

DecodedEntry EntryDecoder::decodeSingleAssetParameterEntry(const AssetParameters &params, const std::string &line) {
    auto tokens = splitLine(line, ',');

    if (params.streamType == StreamType::TRADE_STREAM) {
        TradeEntry trade;
        try {
            if (params.market == Market::SPOT) {
                trade.TimestampOfReceive = std::stoll(tokens[0]);
                trade.Stream = tokens[1];
                trade.EventType = tokens[2];
                trade.EventTime = std::stoll(tokens[3]);
                trade.TransactionTime = std::stoll(tokens[4]);
                trade.Symbol = tokens[5];
                trade.TradeId = std::stoll(tokens[6]);
                trade.Price = std::stod(tokens[7]);
                trade.Quantity = std::stod(tokens[8]);
                trade.IsBuyerMarketMaker = std::stoi(tokens[9]);
                trade.MUnknownParameter = tokens[10];
            } else if (params.market == Market::USD_M_FUTURES || params.market == Market::COIN_M_FUTURES) {
                trade.TimestampOfReceive = std::stoll(tokens[0]);
                trade.Stream = tokens[1];
                trade.EventType = tokens[2];
                trade.EventTime = std::stoll(tokens[3]);
                trade.TransactionTime = std::stoll(tokens[4]);
                trade.Symbol = tokens[5];
                trade.TradeId = std::stoll(tokens[6]);
                trade.Price = std::stod(tokens[7]);
                trade.Quantity = std::stod(tokens[8]);
                trade.IsBuyerMarketMaker = std::stoi(tokens[9]);
                trade.XUnknownParameter = tokens[10];
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Error decoding TradeEntry: " + std::string(e.what()));
        }
        return trade;
    }
    else if (params.streamType == StreamType::DEPTH_SNAPSHOT) {
        DifferenceDepthEntry entry;
        try {
            if (params.market == Market::SPOT) {
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                // entry.TimestampOfRequest = std::stoll(tokens[1]);
                // entry.LastUpdateId      = std::stoll(tokens[2]);  // LastUpdateId
                entry.IsAsk = (std::stoi(tokens[3]) != 0);
                entry.Price = std::stod(tokens[4]);
                entry.Quantity = std::stod(tokens[5]);
            } else if (params.market == Market::USD_M_FUTURES) {
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                // entry.TimestampOfRequest = std::stoll(tokens[1]);
                // entry.MessageOutputTime  = std::stoll(tokens[2]);
                entry.TransactionTime = std::stoll(tokens[3]);
                // entry.LastUpdateId      = std::stoll(tokens[4]);  // LastUpdateId
                entry.IsAsk = (std::stoi(tokens[5]) != 0);
                entry.Price = std::stod(tokens[6]);
                entry.Quantity = std::stod(tokens[7]);
            } else if (params.market == Market::COIN_M_FUTURES) {
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                // entry.TimestampOfRequest = std::stoll(tokens[1]);
                // entry.MessageOutputTime  = std::stoll(tokens[2]);
                entry.TransactionTime = std::stoll(tokens[3]);
                // entry.LastUpdateId      = std::stoll(tokens[4]);  // LastUpdateId
                entry.Symbol = tokens[5];
                // entry.Pair               = tokens[6];
                entry.IsAsk = (std::stoi(tokens[7]) != 0);
                entry.Price = std::stod(tokens[8]);
                entry.Quantity = std::stod(tokens[9]);
            } else {
                throw std::runtime_error("Unknown market for snapshot decoding");
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Error decoding snapshot OrderBookEntry: " + std::string(e.what()));
        }
        return entry;
    }
    else if (params.streamType == StreamType::DIFFERENCE_DEPTH_STREAM) {
        DifferenceDepthEntry entry;
        try {
            if (params.market == Market::SPOT) {
                if (tokens.size() < 10) {
                    throw std::runtime_error("Not enough tokens to decode: OrderBookEntry (SPOT): " + line);
                }
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                entry.Stream = tokens[1];
                entry.EventType = tokens[2];
                entry.EventTime = std::stoll(tokens[3]);
                entry.Symbol = tokens[4];
                entry.FirstUpdateId = std::stoll(tokens[5]);
                entry.FinalUpdateId = std::stoll(tokens[6]);
                entry.IsAsk = (std::stoi(tokens[7]) != 0);
                entry.Price = std::stod(tokens[8]);
                entry.Quantity = std::stod(tokens[9]);
            } else if (params.market == Market::USD_M_FUTURES) {
                if (tokens.size() < 11) {
                    throw std::runtime_error("Not enough tokens to decode: OrderBookEntry (FUTURES): " + line);
                }
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                entry.Stream = tokens[1];
                entry.EventType = tokens[2];
                entry.EventTime = std::stoll(tokens[3]);
                entry.TransactionTime = std::stoll(tokens[4]);
                entry.Symbol = tokens[5];
                entry.FirstUpdateId = std::stoll(tokens[6]);
                entry.FinalUpdateId = std::stoll(tokens[7]);
                entry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                entry.IsAsk = (std::stoi(tokens[9]) != 0);
                entry.Price = std::stod(tokens[10]);
                entry.Quantity = std::stod(tokens[11]);
            } else if (params.market == Market::COIN_M_FUTURES) {
                if (tokens.size() < 12) {
                    throw std::runtime_error("Not enough tokens to decode: OrderBookEntry (FUTURES): " + line);
                }
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                entry.Stream = tokens[1];
                entry.EventType = tokens[2];
                entry.EventTime = std::stoll(tokens[3]);
                entry.TransactionTime = std::stoll(tokens[4]);
                entry.Symbol = tokens[5];
                entry.FirstUpdateId = std::stoll(tokens[6]);
                entry.FinalUpdateId = std::stoll(tokens[7]);
                entry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                entry.IsAsk = (std::stoi(tokens[9]) != 0);
                entry.Price = std::stod(tokens[10]);
                entry.Quantity = std::stod(tokens[11]);
                entry.PSUnknownField = tokens[12];
            } else {
                throw std::runtime_error("Unknown Market during decoding OrderBookEntry");
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Error decoding OrderBookEntry: " + std::string(e.what()));
        }
        return entry;
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
                    TradeEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Stream                = tokens[h["Stream"]];
                    e.EventType             = tokens[h["EventType"]];
                    e.EventTime             = std::stoll(tokens[h["EventTime"]]);
                    e.TransactionTime       = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.TradeId               = std::stoll(tokens[h["TradeId"]]);
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsBuyerMarketMaker    = (tokens[h["IsBuyerMarketMaker"]] == "1");
                    e.MUnknownParameter     = tokens[h["MUnknownParameter"]];
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::SPOT;
                    return e;
                }
                case Market::USD_M_FUTURES: {
                    TradeEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Stream                = tokens[h["Stream"]];
                    e.EventType             = tokens[h["EventType"]];
                    e.EventTime             = std::stoll(tokens[h["EventTime"]]);
                    e.TransactionTime       = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.TradeId               = std::stoll(tokens[h["TradeId"]]);
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsBuyerMarketMaker    = (tokens[h["IsBuyerMarketMaker"]] == "1");
                    e.XUnknownParameter     = tokens[h["XUnknownParameter"]];
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::USD_M_FUTURES;
                    return e;
                }
                case Market::COIN_M_FUTURES: {
                    TradeEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Stream                = tokens[h["Stream"]];
                    e.EventType             = tokens[h["EventType"]];
                    e.EventTime             = std::stoll(tokens[h["EventTime"]]);
                    e.TransactionTime       = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.TradeId               = std::stoll(tokens[h["TradeId"]]);
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsBuyerMarketMaker    = (tokens[h["IsBuyerMarketMaker"]] == "1");
                    e.XUnknownParameter     = tokens[h["XUnknownParameter"]];
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::COIN_M_FUTURES;
                    return e;
                }
            }
        }

        case StreamType::DEPTH_SNAPSHOT: {
            switch (mk) {
                case Market::SPOT: {
                    DifferenceDepthEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.IsAsk                 = (tokens[h["IsAsk"]] == "1");
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::SPOT;
                    return e;
                }
                case Market::USD_M_FUTURES: {
                    DifferenceDepthEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.TransactionTime       = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.IsAsk                 = (tokens[h["IsAsk"]] == "1");
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::USD_M_FUTURES;
                    return e;
                }
                case Market::COIN_M_FUTURES: {
                    DifferenceDepthEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.TransactionTime       = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.IsAsk                 = (tokens[h["IsAsk"]] == "1");
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::COIN_M_FUTURES;
                    return e;
                }
            }
        }

        case StreamType::DIFFERENCE_DEPTH_STREAM: {
            switch (mk) {
                case Market::SPOT: {
                    DifferenceDepthEntry e;
                    e.TimestampOfReceive    = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Stream                = tokens[h["Stream"]];
                    e.EventType             = tokens[h["EventType"]];
                    e.EventTime             = std::stoll(tokens[h["EventTime"]]);
                    e.Symbol                = tokens[h["Symbol"]];
                    e.FirstUpdateId         = std::stoll(tokens[h["FirstUpdateId"]]);
                    e.FinalUpdateId         = std::stoll(tokens[h["FinalUpdateId"]]);
                    e.IsAsk                 = (std::stoi(tokens[h["IsAsk"]]) != 0);
                    e.Price                 = std::stod(tokens[h["Price"]]);
                    e.Quantity              = std::stod(tokens[h["Quantity"]]);
                    e.IsLast                = (tokens[h["IsLast"]] == "1");
                    e.market_               = Market::SPOT;
                    return e;
                }
                case Market::USD_M_FUTURES: {
                    DifferenceDepthEntry e;
                    e.TimestampOfReceive        = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Stream                    = tokens[h["Stream"]];
                    e.EventType                 = tokens[h["EventType"]];
                    e.EventTime                 = std::stoll(tokens[h["EventTime"]]);
                    e.TransactionTime           = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                    = tokens[h["Symbol"]];
                    e.FirstUpdateId             = std::stoll(tokens[h["FirstUpdateId"]]);
                    e.FinalUpdateId             = std::stoll(tokens[h["FinalUpdateId"]]);
                    e.FinalUpdateIdInLastStream = std::stoll(tokens[h["FinalUpdateIdInLastStream"]]);
                    e.IsAsk                     = (std::stoi(tokens[h["IsAsk"]]) != 0);
                    e.Price                     = std::stod(tokens[h["Price"]]);
                    e.Quantity                  = std::stod(tokens[h["Quantity"]]);
                    e.IsLast                    = (tokens[h["IsLast"]] == "1");
                    e.market_                   = Market::USD_M_FUTURES;
                    return e;
                }
                case Market::COIN_M_FUTURES: {
                    DifferenceDepthEntry e;
                    e.TimestampOfReceive        = std::stoll(tokens[h["TimestampOfReceiveUS"]]);
                    e.Stream                    = tokens[h["Stream"]];
                    e.EventType                 = tokens[h["EventType"]];
                    e.EventTime                 = std::stoll(tokens[h["EventTime"]]);
                    e.TransactionTime           = std::stoll(tokens[h["TransactionTime"]]);
                    e.Symbol                    = tokens[h["Symbol"]];
                    e.FirstUpdateId             = std::stoll(tokens[h["FirstUpdateId"]]);
                    e.FinalUpdateId             = std::stoll(tokens[h["FinalUpdateId"]]);
                    e.FinalUpdateIdInLastStream = std::stoll(tokens[h["FinalUpdateIdInLastStream"]]);
                    e.IsAsk                     = (std::stoi(tokens[h["IsAsk"]]) != 0);
                    e.Price                     = std::stod(tokens[h["Price"]]);
                    e.Quantity                  = std::stod(tokens[h["Quantity"]]);
                    e.PSUnknownField            = tokens[h["PSUnknownField"]];
                    e.IsLast                    = (tokens[h["IsLast"]] == "1");
                    e.market_                   = Market::COIN_M_FUTURES;
                    return e;
                }
            }
        }
    }
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
