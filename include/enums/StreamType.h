#pragma once

#include <ostream>
#include <cstdint>

enum class StreamType {
    UNKNOWN,
    DIFFERENCE_DEPTH_STREAM,
    TRADE_STREAM,
    DEPTH_SNAPSHOT,
    FINAL_DEPTH_SNAPSHOT
};

inline std::ostream& operator<<(std::ostream& os, StreamType s) {
    switch(s) {
    case StreamType::UNKNOWN:                 return os << "UNKNOWN";
    case StreamType::DIFFERENCE_DEPTH_STREAM: return os << "DIFFERENCE_DEPTH_STREAM";
    case StreamType::TRADE_STREAM:            return os << "TRADE_STREAM";
    case StreamType::DEPTH_SNAPSHOT:          return os << "DEPTH_SNAPSHOT";
    case StreamType::FINAL_DEPTH_SNAPSHOT:    return os << "FINAL_DEPTH_SNAPSHOT";
    }
    return os << int(s);
}

static constexpr StreamType STREAM_TYPE_MAP[5] = {
    StreamType::UNKNOWN,
    StreamType::DIFFERENCE_DEPTH_STREAM,
    StreamType::TRADE_STREAM,
    StreamType::DEPTH_SNAPSHOT,
    StreamType::FINAL_DEPTH_SNAPSHOT
};

inline StreamType parseStreamType(char c) noexcept {
    return STREAM_TYPE_MAP[static_cast<uint8_t>(c - '0')];
}
