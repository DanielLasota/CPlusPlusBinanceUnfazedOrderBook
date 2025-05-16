#pragma once

#include <ostream>

enum class StreamType {
    DIFFERENCE_DEPTH_STREAM,
    TRADE_STREAM,
    DEPTH_SNAPSHOT
};

inline std::ostream& operator<<(std::ostream& os, StreamType s) {
    switch(s) {
    case StreamType::DIFFERENCE_DEPTH_STREAM: return os << "DIFFERENCE_DEPTH_STREAM";
    case StreamType::TRADE_STREAM:            return os << "TRADE_STREAM";
    case StreamType::DEPTH_SNAPSHOT:          return os << "DEPTH_SNAPSHOT";
    }
    return os << int(s);
}
