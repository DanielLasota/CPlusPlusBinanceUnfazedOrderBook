#pragma once

#include <variant>
#include <optional>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>

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
    explicit OrderBook(size_t maxLevels = 100'000);

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
    double secondAskPrice()  const { return askHead_->next_->price; }
    double secondBidPrice()  const { return bidHead_->next_->price; }

    std::vector<DifferenceDepthEntry> getAsks() const;
    std::vector<DifferenceDepthEntry> getBids() const;

    double sumTopAskQuantity(size_t n) const;
    double sumTopBidQuantity(size_t n) const;

    double sumPriceQty() const { return sumPriceQty_; }

private:

    std::vector<DifferenceDepthEntry> arena;
    DifferenceDepthEntry* freeListHead_{nullptr};

    DifferenceDepthEntry* askHead_{nullptr};
    DifferenceDepthEntry* askTail_{nullptr};
    DifferenceDepthEntry* bidHead_{nullptr};
    DifferenceDepthEntry* bidTail_{nullptr};

    std::map<double, DifferenceDepthEntry*> askMap_;
    std::map<double, DifferenceDepthEntry*, std::greater<double>> bidMap_;

    std::unordered_map<PriceSide, DifferenceDepthEntry*, PriceSideHash> index_;

    size_t askCount_{0}, bidCount_{0};
    double sumAskQty_{0.0}, sumBidQty_{0.0};
    double sumPriceQty_{0.0};

    DifferenceDepthEntry* allocateNode(double price, bool isAsk, double quantity);
    void deallocateNode(DifferenceDepthEntry* node);

    // Fast operations on doubly-linked list
    void addNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node, bool isAsk);
    void removeNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node);
};
