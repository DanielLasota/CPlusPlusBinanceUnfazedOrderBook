#include "EntryDecoder.h"
#include <sstream>
#include <stdexcept>
#include <enums/DifferenceDepthEntry.h>
#include <enums/BinanceEntry.h>

BinanceEntry EntryDecoder::decodeSingleCSVEntry(const AssetParameters &params, const std::string &line) {
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
        // DifferenceDepthSnapshotEntry entry;
        // try {
        //     if (params.market == Market::SPOT) {
        //         if (tokens.size() == 6) {
        //             entry.TimestampOfReceive = std::stoll(tokens[0]);
        //             // entry.TimestampOfRequest = std::stoll(tokens[1]);
        //             // entry.LastUpdateId      = std::stoll(tokens[2]);  // LastUpdateId
        //             entry.IsAsk = (std::stoi(tokens[3]) != 0);
        //             entry.Price = std::stod(tokens[4]);
        //             entry.Quantity = std::stod(tokens[5]);
        //         } else {
        //             throw std::runtime_error("Invalid number of tokens for SPOT snapshot: " + line);
        //         }
        //     } else if (params.market == Market::USD_M_FUTURES) {
        //         if (tokens.size() == 8) {
        //             entry.TimestampOfReceive = std::stoll(tokens[0]);
        //             // entry.TimestampOfRequest = std::stoll(tokens[1]);
        //             // entry.MessageOutputTime  = std::stoll(tokens[2]);
        //             entry.TransactionTime = std::stoll(tokens[3]);
        //             // entry.LastUpdateId      = std::stoll(tokens[4]);  // LastUpdateId
        //             entry.IsAsk = (std::stoi(tokens[5]) != 0);
        //             entry.Price = std::stod(tokens[6]);
        //             entry.Quantity = std::stod(tokens[7]);
        //         } else {
        //             throw std::runtime_error("Invalid number of tokens for USD_M_FUTURES snapshot: " + line);
        //         }
        //     } else if (params.market == Market::COIN_M_FUTURES) {
        //         if (tokens.size() == 10) {
        //             entry.TimestampOfReceive = std::stoll(tokens[0]);
        //             // entry.TimestampOfRequest = std::stoll(tokens[1]);
        //             // entry.MessageOutputTime  = std::stoll(tokens[2]);
        //             entry.TransactionTime = std::stoll(tokens[3]);
        //             // entry.LastUpdateId      = std::stoll(tokens[4]);  // LastUpdateId
        //             entry.Symbol = tokens[5];
        //             // entry.Pair               = tokens[6];
        //             entry.IsAsk = (std::stoi(tokens[7]) != 0);
        //             entry.Price = std::stod(tokens[8]);
        //             entry.Quantity = std::stod(tokens[9]);
        //         } else {
        //             throw std::runtime_error("Invalid number of tokens for COIN_M_FUTURES snapshot: " + line);
        //         }
            // } else {
            //     throw std::runtime_error("Unknown market for snapshot decoding");
            // }
        // } catch (const std::exception &e) {
        //     throw std::runtime_error("Error decoding snapshot OrderBookEntry: " + std::string(e.what()));
        // }
        // return entry;
    }
    else if (params.stream_type == StreamType::DIFFERENCE_DEPTH_STREAM) {
        DifferenceDepthEntry entry;
        try {
            if (params.market == Market::SPOT) {
                SpotDifferenceDepthEntry spotEntry;

                spotEntry.TimestampOfReceive = std::stoll(tokens[0]);
                spotEntry.Stream = tokens[1];
                spotEntry.EventType = tokens[2];
                spotEntry.EventTime = std::stoll(tokens[3]);
                spotEntry.Symbol = tokens[4];
                spotEntry.FirstUpdateId = std::stoll(tokens[5]);
                spotEntry.FinalUpdateId = std::stoll(tokens[6]);
                spotEntry.IsAsk = (std::stoi(tokens[7]) != 0);
                spotEntry.Price = std::stod(tokens[8]);
                spotEntry.Quantity = std::stod(tokens[9]);

                entry = spotEntry;
            } else if (params.market == Market::USD_M_FUTURES) {
                USDMFuturesDifferenceDepthEntry usdMFuturesEntry;

                usdMFuturesEntry.TimestampOfReceive = std::stoll(tokens[0]);
                usdMFuturesEntry.Stream = tokens[1];
                usdMFuturesEntry.EventType = tokens[2];
                usdMFuturesEntry.EventTime = std::stoll(tokens[3]);
                usdMFuturesEntry.TransactionTime = std::stoll(tokens[4]);
                usdMFuturesEntry.Symbol = tokens[5];
                usdMFuturesEntry.FirstUpdateId = std::stoll(tokens[6]);
                usdMFuturesEntry.FinalUpdateId = std::stoll(tokens[7]);
                usdMFuturesEntry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                usdMFuturesEntry.IsAsk = (std::stoi(tokens[9]) != 0);
                usdMFuturesEntry.Price = std::stod(tokens[10]);
                usdMFuturesEntry.Quantity = std::stod(tokens[11]);

                entry = usdMFuturesEntry;
            } else if (params.market == Market::COIN_M_FUTURES) {
                CoinMFuturesDifferenceDepthEntry coinMFuturesEntry;

                coinMFuturesEntry.TimestampOfReceive = std::stoll(tokens[0]);
                coinMFuturesEntry.Stream = tokens[1];
                coinMFuturesEntry.EventType = tokens[2];
                coinMFuturesEntry.EventTime = std::stoll(tokens[3]);
                coinMFuturesEntry.TransactionTime = std::stoll(tokens[4]);
                coinMFuturesEntry.Symbol = tokens[5];
                coinMFuturesEntry.FirstUpdateId = std::stoll(tokens[6]);
                coinMFuturesEntry.FinalUpdateId = std::stoll(tokens[7]);
                coinMFuturesEntry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                coinMFuturesEntry.IsAsk = (std::stoi(tokens[9]) != 0);
                coinMFuturesEntry.Price = std::stod(tokens[10]);
                coinMFuturesEntry.Quantity = std::stod(tokens[11]);
                coinMFuturesEntry.PSUnknownField = tokens[12];

                entry = coinMFuturesEntry;
            } else {
                throw std::runtime_error("Unknown Market during decoding OrderBookEntry");
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Error decoding OrderBookEntry: " + std::string(e.what()));
        }
        return entry;
    }
}

BinanceEntry EntryDecoder::decodeMergedCSVEntry(const AssetParameters &params, const std::string &line) {
    auto tokens = splitLine(line, ',');

    try {
        switch (params.stream_type) {
            case StreamType::DIFFERENCE_DEPTH_STREAM:
                if (params.market == Market::SPOT) {
                    SpotDifferenceDepthEntry entry;
                    entry.TimestampOfReceive = std::stoll(tokens[0]);
                    // entry.Stream = tokens[1];
                    // entry.EventType = tokens[2];
                    // entry.EventTime = std::stoll(tokens[3]);
                    // entry.Symbol = tokens[4];
                    // entry.FirstUpdateId = std::stoll(tokens[5]);
                    // entry.FinalUpdateId = std::stoll(tokens[6]);
                    entry.IsAsk = (std::stoi(tokens[9]) != 0);
                    entry.Price = std::stod(tokens[10]);
                    entry.Quantity = std::stod(tokens[11]);

                    return entry;
                } else if (params.market == Market::USD_M_FUTURES) {
                    USDMFuturesDifferenceDepthEntry entry;
                    entry.TimestampOfReceive = std::stoll(tokens[0]);
                    // entry.Stream = tokens[1];
                    // entry.EventType = tokens[2];
                    // entry.EventTime = std::stoll(tokens[3]);
                    // entry.TransactionTime = std::stoll(tokens[4]);
                    // entry.Symbol = tokens[5];
                    // entry.FirstUpdateId = std::stoll(tokens[6]);
                    // entry.FinalUpdateId = std::stoll(tokens[7]);
                    // entry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                    entry.IsAsk = (std::stoi(tokens[9]) != 0);
                    entry.Price = std::stod(tokens[10]);
                    entry.Quantity = std::stod(tokens[11]);

                    return entry;
                } else if (params.market == Market::COIN_M_FUTURES) {
                    CoinMFuturesDifferenceDepthEntry entry;
                    entry.TimestampOfReceive = std::stoll(tokens[0]);
                    // entry.Stream = tokens[1];
                    // entry.EventType = tokens[2];
                    // entry.EventTime = std::stoll(tokens[3]);
                    // entry.TransactionTime = std::stoll(tokens[4]);
                    // entry.Symbol = tokens[5];
                    // entry.FirstUpdateId = std::stoll(tokens[6]);
                    // entry.FinalUpdateId = std::stoll(tokens[7]);
                    // entry.FinalUpdateIdInLastStream = std::stoll(tokens[8]);
                    entry.IsAsk = (std::stoi(tokens[9]) != 0);
                    entry.Price = std::stod(tokens[10]);
                    entry.Quantity = std::stod(tokens[11]);
                    // entry.PSUnknownField = tokens[12];

                    return entry;
                } else {
                    throw std::runtime_error("Unknown Market in Difference Depth decoding");
                }
            default:
                throw std::runtime_error("Unsupported stream type");
        }
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Error decoding entry: ") + e.what());
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
