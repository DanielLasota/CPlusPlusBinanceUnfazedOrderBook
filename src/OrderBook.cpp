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
    -> DifferenceDepthEntry*
{
    if (!freeListHead_) throw std::runtime_error("Pool exhausted");
    auto *node = freeListHead_;
    freeListHead_ = freeListHead_->next;
    node->Price    = price;
    node->IsAsk    = isAsk;
    node->Quantity = quantity;
    node->prev = node->next = nullptr;
    return node;
}

void OrderBook::deallocateNode(DifferenceDepthEntry* node) {
    node->next = freeListHead_;
    freeListHead_ = node;
}

// Add node at the correct position based on sorted map
void OrderBook::addNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node, bool isAsk) {
    if (!head) {
        // First node in the list
        head = tail = node;
        node->prev = node->next = nullptr;
        return;
    }

    if (isAsk) {
        // For asks, we maintain a sorted list in ascending order
        auto it = askMap_.find(node->Price);
        if (it == askMap_.end()) {
            // Can't happen, but just in case
            // Add to beginning (lowest price)
            node->next = head;
            head->prev = node;
            node->prev = nullptr;
            head = node;
        }
        else {
            auto nextIt = std::next(it);
            if (nextIt == askMap_.end()) {
                // Add to end
                tail->next = node;
                node->prev = tail;
                node->next = nullptr;
                tail = node;
            } else {
                // Add before nextIt's node
                DifferenceDepthEntry* nextNode = nextIt->second;
                DifferenceDepthEntry* prevNode = nextNode->prev;

                node->next = nextNode;
                node->prev = prevNode;

                if (prevNode) prevNode->next = node;
                else head = node;

                nextNode->prev = node;
            }
        }
    } else {
        // For bids, we maintain a sorted list in descending order
        auto it = bidMap_.find(node->Price);
        if (it == bidMap_.end()) {
            // Can't happen, but just in case
            // Add to beginning (highest price)
            node->next = head;
            head->prev = node;
            node->prev = nullptr;
            head = node;
        } else {
            auto nextIt = std::next(it);
            if (nextIt == bidMap_.end()) {
                // Add to end
                tail->next = node;
                node->prev = tail;
                node->next = nullptr;
                tail = node;
            } else {
                // Add before nextIt's node
                DifferenceDepthEntry* nextNode = nextIt->second;
                DifferenceDepthEntry* prevNode = nextNode->prev;

                node->next = nextNode;
                node->prev = prevNode;

                if (prevNode) prevNode->next = node;
                else head = node;

                nextNode->prev = node;
            }
        }
    }
}

void OrderBook::removeNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node) {
    if (node->prev) node->prev->next = node->next;
    else head = node->next;

    if (node->next) node->next->prev = node->prev;
    else tail = node->prev;

    node->prev = node->next = nullptr;
}

void OrderBook::update(DifferenceDepthEntry* e) {
    // dispatch
    PriceSide key{ e->Price, e->IsAsk };
    auto it = index_.find(key);

    if (e->Quantity == 0.0) {
        // remove level
        if (it != index_.end()) {
            auto *node = it->second;
            // update sum/count
            if (node->IsAsk) {
                --askCount_;
                sumAskQty_ -= node->Quantity;
                askMap_.erase(node->Price);
                removeNode(askHead_, askTail_, node);
            } else {
                --bidCount_;
                sumBidQty_ -= node->Quantity;
                bidMap_.erase(node->Price);
                removeNode(bidHead_, bidTail_, node);
            }
            index_.erase(it);
            deallocateNode(node);
        }
    } else {
        if (it != index_.end()) {
            // existing level - only change quantity
            auto *node = it->second;
            if (node->IsAsk) {
                sumAskQty_ += (e->Quantity - node->Quantity);
            } else {
                sumBidQty_ += (e->Quantity - node->Quantity);
            }
            node->Quantity = e->Quantity;
        } else {
            // a new level
            auto *node = allocateNode(e->Price, e->IsAsk, e->Quantity);

            *node = std::move(*e);
            node->prev = nullptr;
            node->next = nullptr;

            if (node->IsAsk) {
                ++askCount_;
                sumAskQty_ += node->Quantity;

                // Insert into askMap and then update linked list
                auto [mapIt, inserted] = askMap_.insert({node->Price, node});

                // Find the correct position in the linked list
                if (askMap_.size() == 1) {
                    // First element
                    askHead_ = askTail_ = node;
                } else {
                    auto it = mapIt;

                    // If not the first element in the map
                    if (it != askMap_.begin()) {
                        auto prevIt = std::prev(it);
                        DifferenceDepthEntry* prevNode = prevIt->second;

                        // Insert after prevNode
                        node->prev = prevNode;
                        node->next = prevNode->next;

                        if (prevNode->next) prevNode->next->prev = node;
                        else askTail_ = node;

                        prevNode->next = node;
                    } else {
                        // Insert at the beginning
                        node->next = askHead_;
                        askHead_->prev = node;
                        node->prev = nullptr;
                        askHead_ = node;
                    }
                }
            } else {
                ++bidCount_;
                sumBidQty_ += node->Quantity;

                // Insert into bidMap and then update linked list
                auto [mapIt, inserted] = bidMap_.insert({node->Price, node});

                // Find the correct position in the linked list
                if (bidMap_.size() == 1) {
                    // First element
                    bidHead_ = bidTail_ = node;
                } else {
                    auto it = mapIt;

                    // If not the first element in the map
                    if (it != bidMap_.begin()) {
                        auto prevIt = std::prev(it);
                        DifferenceDepthEntry* prevNode = prevIt->second;

                        // Insert after prevNode
                        node->prev = prevNode;
                        node->next = prevNode->next;

                        if (prevNode->next) prevNode->next->prev = node;
                        else bidTail_ = node;

                        prevNode->next = node;
                    } else {
                        // Insert at the beginning
                        node->next = bidHead_;
                        bidHead_->prev = node;
                        node->prev = nullptr;
                        bidHead_ = node;
                    }
                }
            }

            index_[key] = node;
        }
    }
}

void OrderBook::printOrderBook() const {

    std::cout << "ORDERBOOK:\n";
    std::cout << "  Asks (lowest→highest):\n";
    for (auto *n = askHead_; n; n = n->next) {
        std::cout << " Price " << n->Price
                  << " Quantity "   << n->Quantity << "\n";
    }

    std::cout << "  Bids (highest→lowest):\n";
    for (auto *n = bidHead_; n; n = n->next) {
        std::cout << " Price " << n->Price
                  << " Quantity "   << n->Quantity << "\n";
    }
    std::cout << std::endl;
}

std::vector<DifferenceDepthEntry> OrderBook::getAsks() const {
    std::vector<DifferenceDepthEntry> result;
    result.reserve(askCount_);
    for (auto *node = askHead_; node; node = node->next) {
        result.push_back(*node);
    }
    return result;
}

std::vector<DifferenceDepthEntry> OrderBook::getBids() const {
    std::vector<DifferenceDepthEntry> result;
    result.reserve(bidCount_);
    for (auto *node = bidHead_; node; node = node->next) {
        result.push_back(*node);
    }
    return result;
}