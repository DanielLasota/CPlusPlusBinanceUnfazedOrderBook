#include <iostream>
#include <variant>

#include "GlobalMarketState.h"

GlobalMarketState::GlobalMarketState(MetricMask mask)
    : mask_(mask), calculator_(mask)
{}

void GlobalMarketState::update(DecodedEntry* entry) {
    AssetKey key = extractKey(*entry);

    if (auto e = std::get_if<DifferenceDepthEntry>(entry)){
        // std::cout << e->market_ << " " << e->Symbol << std::endl;
        if (e->Symbol.empty())
        {
            std::cout << "update diff " << e->TimestampOfReceive << " symbol: " << e->Symbol << " " << e->Price << std::endl;
        }
    }

    states_[key].update(entry);

    if (auto e = std::get_if<DifferenceDepthEntry>(entry)){
        // std::cout << e->market_ << " " << e->Symbol << std::endl;
        if (e->Symbol.empty())
        {
            std::cout << "2update diff " << e->TimestampOfReceive << " symbol: " << e->Symbol << " " << e->Price << std::endl;
        }
    }

}

std::optional<OrderBookMetricsEntry> GlobalMarketState::countMarketStateMetrics(DecodedEntry* entry) {
    AssetKey key = extractKey(*entry);
    if (auto e = std::get_if<DifferenceDepthEntry>(entry)){
        // std::cout << e->market_ << " " << e->Symbol << std::endl;
        if (e->Symbol.empty())
        {
            std::cout << "countMarketStateMetrics diff " << e->TimestampOfReceive << " symbol: " << e->Symbol << " " << e->Price << std::endl;
        }
    }
    return calculator_.countMarketStateMetrics(states_[key]);
}

AssetKey GlobalMarketState::extractKey(const DecodedEntry& d) const {
    if (auto e = std::get_if<DifferenceDepthEntry>(&d)){
        // std::cout << e->market_ << " " << e->Symbol << std::endl;
        if (e->Symbol.empty())
        {
            std::cout << "extractKey diff " << e->TimestampOfReceive << " symbol: " << e->Symbol << " " << e->Price << std::endl;
        }
        return AssetKey{ e->market_, e->Symbol };
    }

    else if (auto e = std::get_if<TradeEntry>(&d)){
        // std::cout << e->market_ << " " << e->Symbol << std::endl;
        if (e->Symbol.empty())
        {
            std::cout << "trade " << e->TimestampOfReceive << " symbol: " << e->Symbol << " " << e->Price << std::endl;
        }
        return AssetKey{ e->market_, e->Symbol };
    }
    else
        throw std::runtime_error("Unknown DecodedEntry variant");
}