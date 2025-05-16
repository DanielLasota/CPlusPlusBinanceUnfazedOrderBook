#pragma once

#include <ostream>

enum class Market {
    SPOT,
    USD_M_FUTURES,
    COIN_M_FUTURES
};

inline std::ostream& operator<<(std::ostream& os, Market m) {
    switch(m) {
    case Market::SPOT:          return os << "SPOT";
    case Market::USD_M_FUTURES: return os << "USD_M_FUTURES";
    case Market::COIN_M_FUTURES:return os << "COIN_M_FUTURES";
    }
    return os << int(m);
}
