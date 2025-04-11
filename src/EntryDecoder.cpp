#include "EntryDecoder.h"
#include <sstream>
#include <stdexcept>


OrderBookEntry EntryDecoder::decodeEntry(const AssetParameters &params, const std::string &line) {
    auto tokens = splitLine(line, ',');

    if (params.stream_type == StreamType::TRADE_STREAM) {

        // if (tokens.size() < 11) {
        //     throw std::runtime_error("Error decoding TradeEntry: " + line);
        // }
        //
        // TradeEntry trade;
        // try {
        //     if (params.market == Market::SPOT) {
        //         trade.TimestampOfReceive = std::stoll(tokens[0]);
        //         trade.Stream             = tokens[1];
        //         trade.EventType          = tokens[2];
        //         trade.EventTime          = std::stoll(tokens[3]);
        //         trade.TransactionTime    = std::stoll(tokens[4]);
        //         trade.Symbol             = tokens[5];
        //         trade.TradeId            = std::stoll(tokens[6]);
        //         trade.Price              = std::stod(tokens[7]);
        //         trade.Quantity           = std::stod(tokens[8]);
        //         trade.IsBuyerMarketMaker = std::stoi(tokens[9]);
        //         trade.MUnknownParameter   = tokens[10];
        //     }
        //     else if (params.market == Market::USD_M_FUTURES || params.market == Market::COIN_M_FUTURES) {
        //         trade.TimestampOfReceive = std::stoll(tokens[0]);
        //         trade.Stream             = tokens[1];
        //         trade.EventType          = tokens[2];
        //         trade.EventTime          = std::stoll(tokens[3]);
        //         trade.TransactionTime    = std::stoll(tokens[4]);
        //         trade.Symbol             = tokens[5];
        //         trade.TradeId            = std::stoll(tokens[6]);
        //         trade.Price              = std::stod(tokens[7]);
        //         trade.Quantity           = std::stod(tokens[8]);
        //         trade.IsBuyerMarketMaker = std::stoi(tokens[9]);
        //         trade.XUnknownParameter   = tokens[10];
        //     }
        //
        // } catch (const std::exception &e) {
        //     throw std::runtime_error("Error decoding TradeEntry: " + std::string(e.what()));
        // }
        // return trade;
    }
    else if (params.stream_type == StreamType::DEPTH_SNAPSHOT) {
        OrderBookEntry entry;
        try {
            if (params.market == Market::SPOT) {
                if (tokens.size() == 6) {
                    entry.TimestampOfReceive = std::stoll(tokens[0]);
                    // entry.TimestampOfRequest = std::stoll(tokens[1]);
                    // entry.LastUpdateId      = std::stoll(tokens[2]);  // LastUpdateId
                    entry.IsAsk              = (std::stoi(tokens[3]) != 0);
                    entry.Price              = std::stod(tokens[4]);
                    entry.Quantity           = std::stod(tokens[5]);
                } else {
                    throw std::runtime_error("Invalid number of tokens for SPOT snapshot: " + line);
                }
            }
            else if (params.market == Market::USD_M_FUTURES) {
                if (tokens.size() == 8) {
                    entry.TimestampOfReceive = std::stoll(tokens[0]);
                    // entry.TimestampOfRequest = std::stoll(tokens[1]);
                    // entry.MessageOutputTime  = std::stoll(tokens[2]);
                    entry.TransactionTime    = std::stoll(tokens[3]);
                    // entry.LastUpdateId      = std::stoll(tokens[4]);  // LastUpdateId
                    entry.IsAsk              = (std::stoi(tokens[5]) != 0);
                    entry.Price              = std::stod(tokens[6]);
                    entry.Quantity           = std::stod(tokens[7]);
                } else {
                    throw std::runtime_error("Invalid number of tokens for USD_M_FUTURES snapshot: " + line);
                }
            }
            else if (params.market == Market::COIN_M_FUTURES) {
                if (tokens.size() == 10) {
                    entry.TimestampOfReceive = std::stoll(tokens[0]);
                    // entry.TimestampOfRequest = std::stoll(tokens[1]);
                    // entry.MessageOutputTime  = std::stoll(tokens[2]);
                    entry.TransactionTime    = std::stoll(tokens[3]);
                    // entry.LastUpdateId      = std::stoll(tokens[4]);  // LastUpdateId
                    entry.Symbol             = tokens[5];
                    // entry.Pair               = tokens[6];
                    entry.IsAsk              = (std::stoi(tokens[7]) != 0);
                    entry.Price              = std::stod(tokens[8]);
                    entry.Quantity           = std::stod(tokens[9]);
                } else {
                    throw std::runtime_error("Invalid number of tokens for COIN_M_FUTURES snapshot: " + line);
                }
            }
            else {
                throw std::runtime_error("Unknown market for snapshot decoding");
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Error decoding snapshot OrderBookEntry: " + std::string(e.what()));
        }
        return entry;
    }
    else if (params.stream_type == StreamType::DIFFERENCE_DEPTH_STREAM) {
        OrderBookEntry entry;
        try {

            if (params.market == Market::SPOT) {
                if (tokens.size() < 10) {
                    throw std::runtime_error("Not enough tokens to decode: OrderBookEntry (SPOT): " + line);
                }
                entry.TimestampOfReceive = std::stoll(tokens[0]);
                entry.Stream             = tokens[1];
                entry.EventType          = tokens[2];
                entry.EventTime          = std::stoll(tokens[3]);
                entry.Symbol             = tokens[4];
                entry.FirstUpdateId      = std::stoll(tokens[5]);
                entry.FinalUpdateId      = std::stoll(tokens[6]);
                entry.IsAsk              = (std::stoi(tokens[7]) != 0);
                entry.Price              = std::stod(tokens[8]);
                entry.Quantity           = std::stod(tokens[9]);
            }
            else if (params.market == Market::USD_M_FUTURES) {
                if (tokens.size() < 11) {
                    throw std::runtime_error("Not enough tokens to decode: OrderBookEntry (FUTURES): " + line);
                }
                entry.TimestampOfReceive        = std::stoll(tokens[0]);
                entry.Stream                    = tokens[1];
                entry.EventType                 = tokens[2];
                entry.EventTime                 = std::stoll(tokens[3]);
                entry.TransactionTime           = std::stoll(tokens[4]);
                entry.Symbol                    = tokens[5];
                entry.FirstUpdateId             = std::stoll(tokens[6]);
                entry.FinalUpdateId             = std::stoll(tokens[7]);
                entry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                entry.IsAsk                     = (std::stoi(tokens[9]) != 0);
                entry.Price                     = std::stod(tokens[10]);
                entry.Quantity                  = std::stod(tokens[11]);
            }
            else if (params.market == Market::COIN_M_FUTURES) {
                if (tokens.size() < 12) {
                    throw std::runtime_error("Not enough tokens to decode: OrderBookEntry (FUTURES): " + line);
                }
                entry.TimestampOfReceive        = std::stoll(tokens[0]);
                entry.Stream                    = tokens[1];
                entry.EventType                 = tokens[2];
                entry.EventTime                 = std::stoll(tokens[3]);
                entry.TransactionTime           = std::stoll(tokens[4]);
                entry.Symbol                    = tokens[5];
                entry.FirstUpdateId             = std::stoll(tokens[6]);
                entry.FinalUpdateId             = std::stoll(tokens[7]);
                entry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                entry.IsAsk                     = (std::stoi(tokens[9]) != 0);
                entry.Price                     = std::stod(tokens[10]);
                entry.Quantity                  = std::stod(tokens[11]);
                entry.PSUnknownField            = tokens[12];
            }
            else {
                throw std::runtime_error("Unknown Market during decoding OrderBookEntry");
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Error decoding OrderBookEntry: " + std::string(e.what()));
        }
        return entry;
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
