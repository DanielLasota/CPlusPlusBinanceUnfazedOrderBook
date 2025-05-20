#pragma once

struct AssetKey {
    Market      market;
    std::string symbol;

    AssetKey(Market m, std::string s)
      : market(m), symbol(std::move(s))
    {}

    AssetKey(const DecodedEntry &d) {
        if (auto e = std::get_if<DifferenceDepthEntry>(&d)) {
            market = e->Market_;
            symbol = e->Symbol;
        }
        else if (auto e = std::get_if<TradeEntry>(&d)) {
            market = e->Market_;
            symbol = e->Symbol;
        }
        else {
            throw std::runtime_error("Unknown DecodedEntry variant");
        }
    }

    bool operator==(const AssetKey& o) const noexcept {
        return market == o.market && symbol == o.symbol;
    }
};

struct AssetKeyHash {
    size_t operator()(AssetKey const& k) const noexcept {
        auto h1 = std::hash<int>{}(int(k.market));
        auto h2 = std::hash<std::string>{}(k.symbol);
        return h1 ^ (h2 << 1);
    }
};
