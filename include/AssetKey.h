#pragma once

#include <variant>
#include <cstdint>
#include <stdexcept>
#include "enums/DifferenceDepthEntry.h"
#include "enums/TradeEntry.h"
#include "enums/symbol.h"
#include "enums/Market.h"

using DecodedEntry = std::variant<DifferenceDepthEntry, TradeEntry>;

struct AssetKey {
    Market   market;
    Symbol symbol;

    AssetKey(Market m, Symbol s) noexcept
      : market(m), symbol(s)
    {}

    AssetKey(const DecodedEntry &d) noexcept {
        if (auto dd = std::get_if<DifferenceDepthEntry>(&d)) {
            market = dd->Market_;
            symbol = dd->symbol;
        }
        else if (auto te = std::get_if<TradeEntry>(&d)) {
            market = te->Market_;
            symbol = te->symbol;
        }
        else {
            std::terminate();
        }
    }

    bool operator==(const AssetKey &o) const noexcept {
        return market == o.market
            && symbol == o.symbol;
    }
};

struct AssetKeyHash {
    size_t operator()(AssetKey const& k) const noexcept {
        return (static_cast<size_t>(k.market) << 16)
             | static_cast<size_t>(k.symbol);
    }
};
