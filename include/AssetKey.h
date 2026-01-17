#pragma once

#include "enums/DifferenceDepthEntry.h"
#include "enums/Market.h"
#include "enums/Symbol.h"
#include "enums/TradeEntry.h"
#include "enums/XEvent.h"


struct AssetKey {
    Market market;
    Symbol symbol;

    AssetKey(const Market m, const Symbol s) noexcept
      : market(m), symbol(s)
    {}

    explicit AssetKey(const XEvent &d) noexcept {
        if (const auto dd = std::get_if<DifferenceDepthEntry>(&d)) {
            market = dd->market;
            symbol = dd->symbol;
        }
        else if (const auto te = std::get_if<TradeEntry>(&d)) {
            market = te->market;
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
