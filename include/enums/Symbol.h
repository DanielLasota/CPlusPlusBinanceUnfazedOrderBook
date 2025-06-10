#pragma once

#include <array>
#include <string>
#include <string_view>
#include <ostream>
#include <cstdint>

enum class Symbol : uint16_t {
    UNKNOWN,
    BTCUSDT,
    ETHUSDT,
    BNBUSDT,
    SOLUSDT,
    XRPUSDT,
    DOGEUSDT,
    ADAUSDT,
    SHIBUSDT,
    LTCUSDT,
    AVAXUSDT,
    TRXUSDT,
    DOTUSDT,
    BCHUSDT,
    SUIUSDT,
};

// liczbę elementów wyliczamy od najwyższego wartościowego
static constexpr size_t SymbolEnCount =
    static_cast<size_t>(Symbol::SUIUSDT) + 1;

// tablica musi mieć dokładnie SymbolEnCount elementów
static constexpr std::array<const char*, SymbolEnCount> symbolEnNames = {{
    "UNKNOWN",
    "BTCUSDT",
    "ETHUSDT",
    "BNBUSDT",
    "SOLUSDT",
    "XRPUSDT",
    "DOGEUSDT",
    "ADAUSDT",
    "SHIBUSDT",
    "LTCUSDT",
    "AVAXUSDT",
    "TRXUSDT",
    "DOTUSDT",
    "BCHUSDT",
    "SUIUSDT"
}};

inline std::string to_string(Symbol s) {
    auto idx = static_cast<size_t>(s);
    if (idx < SymbolEnCount)
        return symbolEnNames[idx];
    // fallback
    return symbolEnNames[0];
}

inline std::ostream& operator<<(std::ostream& os, Symbol s) {
    return os << to_string(s);
}

inline Symbol parseSymbol(std::string_view sv) noexcept {
    uint16_t v = 0;
    switch (sv.size()) {
    case 1:
        v = sv[0] - '0';
        break;
    case 2:
        v = (sv[0] - '0') * 10
          + (sv[1] - '0');
        break;
    case 3:
        v = (sv[0] - '0') * 100
          + (sv[1] - '0') * 10
          + (sv[2] - '0');
        break;
    default:
        return Symbol::UNKNOWN;
    }
    return (v < SymbolEnCount)
         ? static_cast<Symbol>(v)
         : Symbol::UNKNOWN;
}

inline Symbol parseSymbolFromName(std::string_view name) noexcept {
    for (size_t i = 0; i < SymbolEnCount; ++i) {
        if (symbolEnNames[i] == name)
            return static_cast<Symbol>(i);
    }
    return Symbol::UNKNOWN;
}
