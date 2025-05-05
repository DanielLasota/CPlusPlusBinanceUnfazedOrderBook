#include "OrderBook.h"
#include <algorithm>
#include <iostream>

OrderBook::OrderBook(size_t maxLevels) {
    arena.resize(maxLevels);
    // build free list
    for (size_t i = 0; i + 1 < maxLevels; ++i) {
        arena[i].next = &arena[i + 1];
    }
    arena[maxLevels - 1].next = nullptr;
    freeListHead_ = &arena[0];
}

auto OrderBook::allocateNode(double price, bool isAsk, double quantity)
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

void OrderBook::deallocateNode(PriceLevel* node) {
    node->next = freeListHead_;
    freeListHead_ = node;
}

void OrderBook::insertHead(PriceLevel*& head, PriceLevel* node) {
    node->next = head;
    if (head) head->prev = node;
    head = node;
}

void OrderBook::removeNode(PriceLevel*& head, PriceLevel* node) {
    if (node->prev) node->prev->next = node->next;
    else            head = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = node->next = nullptr;
}

void OrderBook::update(DifferenceDepthEntry* e) {

    // dispatch
    PriceSide key{ e->Price, e->IsAsk };
    auto it = index_.find(key);

    if (e->Quantity == 0.0) {
        // set level
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
            // existing level - only change quantity
            auto *node = it->second;
            if (node->isAsk) {
                sumAskQty_ += (e->Quantity - node->quantity);
            } else {
                sumBidQty_ += (e->Quantity - node->quantity);
            }
            node->quantity = e->Quantity;

        } else {
            // a new level see also https://youtu.be/RTAZf_MAMYo?si=Mbj3mnvN6f15mA4a&t=12
            auto *node = allocateNode(e->Price, e->IsAsk, e->Quantity);
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
}

void OrderBook::printOrderBook() const {
    std::vector<PriceLevel*> asks;
    asks.reserve(askCount_);
    for (auto *n = askHead_; n; n = n->next)
        asks.push_back(n);

    std::vector<PriceLevel*> bids;
    bids.reserve(bidCount_);
    for (auto *n = bidHead_; n; n = n->next)
        bids.push_back(n);

    std::sort(asks.begin(), asks.end(),
              [](auto *a, auto *b){ return a->price < b->price; });
    std::sort(bids.begin(), bids.end(),
              [](auto *a, auto *b){ return a->price > b->price; });

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
