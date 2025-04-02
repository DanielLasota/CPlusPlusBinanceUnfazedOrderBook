#include "EntryDecoder.h"
#include <sstream>
#include <stdexcept>


OrderBookEntry EntryDecoder::decodeEntry(const AssetParameters &params, const std::string &line) {
    auto tokens = split(line, ',');

    if (params.stream_type == StreamType::TRADE_STREAM) {

        // if (tokens.size() < 11) {
        //     throw std::runtime_error("Niewystarczająca liczba tokenów do dekodowania TradeEntry: " + line);
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
        //     throw std::runtime_error("Błąd dekodowania TradeEntry: " + std::string(e.what()));
        // }
        // return trade;
    }
    else {
        OrderBookEntry entry;
        try {

            if (params.market == Market::SPOT) {
                if (tokens.size() < 10) {
                    throw std::runtime_error("Niewystarczająca liczba tokenów do dekodowania OrderBookEntry (SPOT): " + line);
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
                    throw std::runtime_error("Niewystarczająca liczba tokenów do dekodowania OrderBookEntry (FUTURES): " + line);
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
                    throw std::runtime_error("Niewystarczająca liczba tokenów do dekodowania OrderBookEntry (FUTURES): " + line);
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
                throw std::runtime_error("Nieznany rynek podczas dekodowania OrderBookEntry");
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Błąd dekodowania OrderBookEntry: " + std::string(e.what()));
        }
        return entry;
    }
}

std::vector<std::string> EntryDecoder::split(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
