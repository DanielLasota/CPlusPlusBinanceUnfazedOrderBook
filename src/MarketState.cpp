#include "MarketState.h"
#include "SingleVariableCounter.h"
#include <algorithm>
#include <iostream>

// ----------------------------------------------------------------
// 1) Raw‐pointers version – bez zmian
// ----------------------------------------------------------------
MarketState<false>::MarketState() = default;

void MarketState<false>::update(DecodedEntry* entryPtr) {
    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entryPtr);

    if (auto* dde = std::get_if<DifferenceDepthEntry>(entryPtr)) {
        orderBook.addOrder(dde);
    }
    if (auto* te = std::get_if<TradeEntry>(entryPtr)) {
        lastTradeStorage = *te;
        lastTradePtr     = &lastTradeStorage;
        hasLastTrade     = true;
    }
}

std::optional<OrderBookMetricsEntry>
MarketState<false>::countOrderBookMetrics(MetricMask mask) const {
    if (orderBook.bids.size() < 2 ||
        orderBook.asks.size() < 2 ||
        !hasLastTrade)
        return std::nullopt;

    OrderBookMetricsEntry o{};
    if (mask & TimestampOfReceive)   o.timestampOfReceive  = lastTimestampOfReceive;
    if (mask & BestAsk)              o.bestAsk             = SingleVariableCounter::calculateBestAskPrice(orderBook);
    if (mask & BestBid)              o.bestBid             = SingleVariableCounter::calculateBestBidPrice(orderBook);
    if (mask & MidPrice)             o.midPrice            = SingleVariableCounter::calculateMidPrice(orderBook);
    if (mask & BestVolumeImbalance)  o.bestVolumeImbalance = SingleVariableCounter::calculateBestVolumeImbalance(orderBook);
    if (mask & QueueImbalance)       o.queueImbalance      = SingleVariableCounter::calculateQueueImbalance(orderBook);
    if (mask & VolumeImbalance)      o.volumeImbalance     = SingleVariableCounter::calculateVolumeImbalance(orderBook);
    if (mask & Gap)                  o.gap                 = SingleVariableCounter::calculateGap(orderBook);
    if (mask & IsAggressorAsk)       o.isAggressorAsk      = SingleVariableCounter::calculateIsAggressorAsk(lastTradePtr);
    return o;
}

// ----------------------------------------------------------------
// 2) Pooled & optimized version
// ----------------------------------------------------------------
MarketState<true>::MarketState(size_t maxLevels): lastTradeStorage() {
    arena.resize(maxLevels);
    // build free list
    for (size_t i = 0; i + 1 < maxLevels; ++i) {
        arena[i].next = &arena[i + 1];
    }
    arena[maxLevels - 1].next = nullptr;
    freeListHead_ = &arena[0];
}

auto MarketState<true>::allocateNode(double price, bool isAsk, double quantity)
    -> PriceLevel*
{
    if (!freeListHead_) throw std::runtime_error("Pool exhausted");
    auto *node = freeListHead_;
    freeListHead_ = freeListHead_->next;
    node->price    = price;
    node->isAsk    = isAsk;
    node->quantity = quantity;
    node->prev = node->next = nullptr;
    return node;
}

void MarketState<true>::deallocateNode(PriceLevel* node) {
    node->next = freeListHead_;
    freeListHead_ = node;
}

void MarketState<true>::insertHead(PriceLevel*& head, PriceLevel* node) {
    node->next = head;
    if (head) head->prev = node;
    head = node;
}

void MarketState<true>::removeNode(PriceLevel*& head, PriceLevel* node) {
    if (node->prev) node->prev->next = node->next;
    else            head = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = node->next = nullptr;
}

void MarketState<true>::update(DecodedEntry* entryPtr) {
    // odśwież timestamp
    std::visit([this](auto const& e){
        this->lastTimestampOfReceive = e.TimestampOfReceive;
    }, *entryPtr);

    // dispatch
    if (auto* dde = std::get_if<DifferenceDepthEntry>(entryPtr)) {
        PriceSide key{ dde->Price, dde->IsAsk };
        auto it = index_.find(key);

        if (dde->Quantity == 0.0) {
            // usuwanie poziomu
            if (it != index_.end()) {
                auto *node = it->second;
                // update sum/count
                if (node->isAsk) { --askCount_; sumAskQty_ -= node->quantity; }
                else             { --bidCount_; sumBidQty_ -= node->quantity; }
                removeNode(node->isAsk ? askHead_ : bidHead_, node);
                index_.erase(it);
                deallocateNode(node);
            }

        } else {
            if (it != index_.end()) {
                // istniejący poziom – tylko zmień quantity
                auto *node = it->second;
                if (node->isAsk) {
                    sumAskQty_ += (dde->Quantity - node->quantity);
                } else {
                    sumBidQty_ += (dde->Quantity - node->quantity);
                }
                node->quantity = dde->Quantity;

            } else {
                // nowy poziom
                auto *node = allocateNode(dde->Price, dde->IsAsk, dde->Quantity);
                if (node->isAsk) {
                    ++askCount_; sumAskQty_ += node->quantity;
                    insertHead(askHead_, node);
                } else {
                    ++bidCount_; sumBidQty_ += node->quantity;
                    insertHead(bidHead_, node);
                }
                index_[key] = node;
            }
        }

    } else if (auto* te = std::get_if<TradeEntry>(entryPtr)) {
        // trade → kopiujemy
        lastTradeStorage = *te;
        lastTradePtr     = &lastTradeStorage;
        hasLastTrade     = true;
    }
}

std::optional<OrderBookMetricsEntry>
MarketState<true>::countOrderBookMetrics(MetricMask mask) const {
    // potrzebujemy co najmniej 2 poziomy po obu stronach i trade
    if (!hasLastTrade ||
        askCount_ < 2 ||
        bidCount_ < 2)
        return std::nullopt;

    OrderBookMetricsEntry o{};
    if (mask & TimestampOfReceive) {
        o.timestampOfReceive = lastTimestampOfReceive;
    }
    if (mask & BestAsk) {
        o.bestAsk = askHead_->price;
    }
    if (mask & BestBid) {
        o.bestBid = bidHead_->price;
    }
    if (mask & MidPrice) {
        o.midPrice = (o.bestAsk + o.bestBid) * 0.5;
    }
    if (mask & BestVolumeImbalance) {
        double a = askHead_->quantity, b = bidHead_->quantity;
        o.bestVolumeImbalance = (b - a) / (b + a);
    }
    if (mask & QueueImbalance) {
        o.queueImbalance = double(bidCount_ - askCount_) / double(bidCount_ + askCount_);
    }
    if (mask & VolumeImbalance) {
        o.volumeImbalance = (sumBidQty_ - sumAskQty_) / (sumBidQty_ + sumAskQty_);
    }
    if (mask & Gap) {
        double secondBid = bidHead_->next->price;
        double secondAsk = askHead_->next->price;
        o.gap = (secondBid + secondAsk) - (o.bestBid + o.bestAsk);
    }
    if (mask & IsAggressorAsk) {
        // inline zamiast całego OrderBook
        o.isAggressorAsk = lastTradePtr->IsBuyerMarketMaker;
    }
    return o;
}

void MarketState<true>::printOrderBook() const {
    // Zbierz wskaźniki na wszystkie poziomy
    std::vector<PriceLevel*> asks;
    asks.reserve(askCount_);
    for (auto *n = askHead_; n; n = n->next)
        asks.push_back(n);

    std::vector<PriceLevel*> bids;
    bids.reserve(bidCount_);
    for (auto *n = bidHead_; n; n = n->next)
        bids.push_back(n);

    // Posortuj do wyświetlenia
    std::sort(asks.begin(), asks.end(),
              [](auto *a, auto *b){ return a->price < b->price; });
    std::sort(bids.begin(), bids.end(),
              [](auto *a, auto *b){ return a->price > b->price; });

    // Wypisz
    std::cout << "ORDERBOOK:\n";
    std::cout << "  Asks (lowest→highest):\n";
    for (auto *n : asks) {
        std::cout << "    Price: " << n->price
                  << ", Qty: "   << n->quantity << "\n";
    }

    std::cout << "  Bids (highest→lowest):\n";
    for (auto *n : bids) {
        std::cout << "    Price: " << n->price
                  << ", Qty: "   << n->quantity << "\n";
    }
    std::cout << std::endl;
}

