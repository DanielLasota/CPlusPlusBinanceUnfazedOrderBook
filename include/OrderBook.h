#pragma once

#include <variant>
#include <optional>
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
        const auto h1 = std::hash<uint64_t>{}(
            *reinterpret_cast<uint64_t const*>(&ps.price)
        );
        const auto h2 = std::hash<bool>{}(ps.isAsk);
        return h1 ^ (h2 << 1);
    }
};

class OrderBook {
public:
    explicit OrderBook(size_t maxLevels = 150'000);

    void update(DifferenceDepthEntry* entryPtr);

    void printOrderBook() const;

    size_t askCount() const { return askCount_; }
    size_t bidCount() const { return bidCount_; }

    double sumAskQuantity() const { return sumAskQuantity_; }
    double sumBidQuantity() const { return sumBidQuantity_; }
    double sumTotalAskBidQuantity() const { return sumAskQuantity_ + sumBidQuantity_; }
    double bestAskPrice() const { return askHead_->price; }
    double bestBidPrice() const { return bidHead_->price; }
    double bestAskQuantity() const { return askHead_->quantity; }
    double bestBidQuantity() const { return bidHead_->quantity; }
    double secondAskPrice() const { return askHead_->next_->price; }
    double secondBidPrice() const { return bidHead_->next_->price; }

    size_t deltaAskCount() const { return deltaAskCount_; }
    size_t deltaBidCount() const { return deltaBidCount_; }
    double deltaBestAskQuantity() const { return deltaBestAskQuantity_; }
    double deltaBestBidQuantity() const { return deltaBestBidQuantity_; }
    double deltaBestAskPrice() const { return deltaBestAskPrice_; }
    double deltaBestBidPrice() const { return deltaBestBidPrice_; }
    double deltaSumAskQuantity() const { return deltaSumAskQuantity_; }
    double deltaSumBidQuantity() const { return deltaSumBidQuantity_; }

    double cumulativeQuantityOfTopNAsks(size_t n) const;
    double cumulativeQuantityOfTopNBids(size_t n) const;
    double sumOfPriceTimesQuantity() const { return sumOfPriceTimesQuantity_; }
    double bestNthAskPrice(size_t K) const;
    double bestNthBidPrice(size_t K) const;

    std::vector<DifferenceDepthEntry> getAsks() const;
    std::vector<DifferenceDepthEntry> getBids() const;

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

    size_t askCount_{0};
    size_t bidCount_{0};

    double sumAskQuantity_{0.0};
    double sumBidQuantity_{0.0};
    double sumOfPriceTimesQuantity_{0.0};

    double prevBestAskPrice_{0.0};
    double prevBestBidPrice_{0.0};
    double prevBestAskQuantity_{0.0};
    double prevBestBidQuantity_{0.0};
    double prevSumAskQuantity_{0.0};
    double prevSumBidQuantity_{0.0};
    size_t prevAskCount_{0};
    size_t prevBidCount_{0};

    double deltaBestAskPrice_{0.0};
    double deltaBestBidPrice_{0.0};
    double deltaBestAskQuantity_{0.0};
    double deltaBestBidQuantity_{0.0};
    double deltaSumAskQuantity_{0.0};
    double deltaSumBidQuantity_{0.0};
    size_t deltaAskCount_{0};
    size_t deltaBidCount_{0};

    DifferenceDepthEntry* allocateNode(double price, bool isAsk, double quantity);
    void deallocateNode(DifferenceDepthEntry* node);

    void addNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node, bool isAsk);
    void removeNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node);
};
