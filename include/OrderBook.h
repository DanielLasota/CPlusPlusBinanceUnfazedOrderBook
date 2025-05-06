#pragma once

#include <variant>
#include <optional>
#include <cstdint>
#include <OrderBook.h>
#include <vector>
#include <unordered_map>

#include "MetricMask.h"
#include "enums/TradeEntry.h"
#include "enums/DifferenceDepthEntry.h"

using DecodedEntry = std::variant<DifferenceDepthEntry, TradeEntry>;

struct PriceSide {
    double price;
    bool   isAsk;
    bool operator==(PriceSide const& o) const noexcept {
        return price == o.price && isAsk == o.isAsk;
    }
};

struct PriceSideHash {
    size_t operator()(PriceSide const& ps) const noexcept {
        // double+bool byte representation
        auto h1 = std::hash<uint64_t>{}(
            *reinterpret_cast<uint64_t const*>(&ps.price)
        );
        auto h2 = std::hash<bool>{}(ps.isAsk);
        return h1 ^ (h2 << 1);
    }
};

class OrderBook {
public:
    struct PriceLevel {
        double     price;
        bool       isAsk;
        double     quantity;
        PriceLevel* prev;
        PriceLevel* next;
    };

    explicit OrderBook(size_t maxLevels = 100000);

    void update(DifferenceDepthEntry* entryPtr);

    void printOrderBook() const;

    size_t askCount()   const { return askCount_; }
    size_t bidCount()   const { return bidCount_; }
    double sumAskQuantity() const { return sumAskQty_; }
    double sumBidQuantity() const { return sumBidQty_; }
    double bestAskPrice()    const { return askHead_->price; }
    double bestBidPrice()    const { return bidHead_->price; }
    double bestAskQuantity()    const { return askHead_->quantity; }
    double bestBidQuantity()    const { return bidHead_->quantity; }
    double secondAskPrice()  const { return askHead_->next->price; }
    double secondBidPrice()  const { return bidHead_->next->price; }

    std::vector<PriceLevel> getAsks() const;
    std::vector<PriceLevel> getBids() const;

private:

    std::vector<PriceLevel> arena;
    PriceLevel* freeListHead_{nullptr};

    PriceLevel* askHead_{nullptr};
    PriceLevel* bidHead_{nullptr};

    std::unordered_map<PriceSide, PriceLevel*, PriceSideHash> index_;

    size_t askCount_{0}, bidCount_{0};
    double sumAskQty_{0.0}, sumBidQty_{0.0};

    PriceLevel* allocateNode(double price, bool isAsk, double quantity);
    void deallocateNode(PriceLevel* node);

    static void insertHead(PriceLevel*& head, PriceLevel* node);
    static void removeNode(PriceLevel*& head, PriceLevel* node);
};
