#pragma once

#include <ostream>
#include <cstdint>

enum class Market {
    UNKNOWN,
    SPOT,
    USD_M_FUTURES,
    COIN_M_FUTURES,
};

inline std::ostream& operator<<(std::ostream& os, Market m) {
    switch(m) {
    case Market::UNKNOWN:       return os << "UNKNOWN";
    case Market::SPOT:          return os << "SPOT";
    case Market::USD_M_FUTURES: return os << "USD_M_FUTURES";
    case Market::COIN_M_FUTURES:return os << "COIN_M_FUTURES";
    }
    return os << int(m);
}

static constexpr Market MARKET_MAP[4] = {
    Market::UNKNOWN,
    Market::SPOT,
    Market::USD_M_FUTURES,
    Market::COIN_M_FUTURES,
};

inline Market parseMarket(char c) noexcept {
    return MARKET_MAP[static_cast<uint8_t>(c - '0')];
}

