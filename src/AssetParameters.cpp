#include "enums/AssetParameters.h"
#include <ostream>

std::ostream& operator<<(std::ostream& os, const Market& market) {
    switch (market) {
        case Market::SPOT:
            os << "SPOT";
        break;
        case Market::USD_M_FUTURES:
            os << "USD_M_FUTURES";
        break;
        case Market::COIN_M_FUTURES:
            os << "COIN_M_FUTURES";
        break;
        default:
            os << "UNKNOWN";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const StreamType& streamType) {
    switch (streamType) {
        case StreamType::DIFFERENCE_DEPTH_STREAM:
            os << "DIFFERENCE_DEPTH_STREAM";
        break;
        case StreamType::TRADE_STREAM:
            os << "TRADE_STREAM";
        break;
        case StreamType::DEPTH_SNAPSHOT:
            os << "DEPTH_SNAPSHOT";
        break;
        default:
            os << "UNKNOWN";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const AssetParameters& params) {
    os << "Market: " << params.market
       << ", Stream: " << params.stream_type
       << ", Pair: " << params.pair
       << ", Date: " << params.date;
    return os;
}
