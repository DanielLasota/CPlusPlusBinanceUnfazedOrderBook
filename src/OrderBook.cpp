#include <algorithm>
#include <iostream>
#include <iomanip>

#include "OrderBook.h"

OrderBook::OrderBook(size_t maxLevels) {
    arena.resize(maxLevels);
    for (size_t i = 0; i + 1 < maxLevels; ++i) {
        arena[i].next_ = &arena[i + 1];
    }
    arena[maxLevels - 1].next_ = nullptr;
    freeListHead_ = &arena[0];
}

auto OrderBook::allocateNode(double price, bool isAsk, double quantity)
    -> DifferenceDepthEntry*
{
    if (!freeListHead_) throw std::runtime_error("Pool exhausted");
    auto *node = freeListHead_;
    freeListHead_ = freeListHead_->next_;
    node->price    = price;
    node->isAsk    = isAsk;
    node->quantity = quantity;
    node->prev_ = node->next_ = nullptr;
    return node;
}

void OrderBook::deallocateNode(DifferenceDepthEntry* node) {
    node->next_ = freeListHead_;
    freeListHead_ = node;
}

// Add node at the correct position based on sorted map
void OrderBook::addNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node, bool isAsk) {
    if (!head) {
        // First node in the list
        head = tail = node;
        node->prev_ = node->next_ = nullptr;
        return;
    }

    if (isAsk) {
        // For asks, we maintain a sorted list in ascending order
        auto it = askMap_.find(node->price);
        if (it == askMap_.end()) {
            // Can't happen, but just in case
            // Add to beginning (lowest price)
            node->next_ = head;
            head->prev_ = node;
            node->prev_ = nullptr;
            head = node;
        }
        else {
            auto nextIt = std::next(it);
            if (nextIt == askMap_.end()) {
                // Add to end
                tail->next_ = node;
                node->prev_ = tail;
                node->next_ = nullptr;
                tail = node;
            } else {
                // Add before nextIt's node
                DifferenceDepthEntry* nextNode = nextIt->second;
                DifferenceDepthEntry* prevNode = nextNode->prev_;

                node->next_ = nextNode;
                node->prev_ = prevNode;

                if (prevNode) prevNode->next_ = node;
                else head = node;

                nextNode->prev_ = node;
            }
        }
    } else {
        // For bids, we maintain a sorted list in descending order
        auto it = bidMap_.find(node->price);
        if (it == bidMap_.end()) {
            // Can't happen, but just in case
            // Add to beginning (highest price)
            node->next_ = head;
            head->prev_ = node;
            node->prev_ = nullptr;
            head = node;
        } else {
            auto nextIt = std::next(it);
            if (nextIt == bidMap_.end()) {
                // Add to end
                tail->next_ = node;
                node->prev_ = tail;
                node->next_ = nullptr;
                tail = node;
            } else {
                // Add before nextIt's node
                DifferenceDepthEntry* nextNode = nextIt->second;
                DifferenceDepthEntry* prevNode = nextNode->prev_;

                node->next_ = nextNode;
                node->prev_ = prevNode;

                if (prevNode) prevNode->next_ = node;
                else head = node;

                nextNode->prev_ = node;
            }
        }
    }
}

void OrderBook::removeNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node) {
    if (node->prev_) node->prev_->next_ = node->next_;
    else head = node->next_;

    if (node->next_) node->next_->prev_ = node->prev_;
    else tail = node->prev_;

    node->prev_ = node->next_ = nullptr;
}

void OrderBook::update(DifferenceDepthEntry* e) {
    // dispatch
    PriceSide key{ e->price, e->isAsk };
    auto it = index_.find(key);

    if (e->quantity == 0.0) {
        // remove level
        if (it != index_.end()) {
            auto *node = it->second;
            // update sum/count
            if (node->isAsk) {
                --askCount_;
                sumAskQty_ -= node->quantity;
                askMap_.erase(node->price);
                removeNode(askHead_, askTail_, node);
            } else {
                --bidCount_;
                sumBidQty_ -= node->quantity;
                bidMap_.erase(node->price);
                removeNode(bidHead_, bidTail_, node);
            }
            index_.erase(it);
            deallocateNode(node);
        }
    }
    else {
        if (it != index_.end()) {
            // existing level - only change quantity
            auto *node = it->second;
            if (node->isAsk) {
                sumAskQty_ += (e->quantity - node->quantity);
            } else {
                sumBidQty_ += (e->quantity - node->quantity);
            }
            node->timestampOfReceive = e->timestampOfReceive;
            node->quantity = e->quantity;
            node->isLast = e->isLast;
        }
        else {
            // a new level
            auto *node = allocateNode(e->price, e->isAsk, e->quantity);

            DifferenceDepthEntry temp = *e;
            *node = std::move(temp);

            node->prev_ = nullptr;
            node->next_ = nullptr;

            // auto *node = allocateNode(e->Price, e->IsAsk, e->Quantity);
            // // Kopiuj tylko niezbędne pola zamiast przenoszenia
            // node->Price = e->Price;
            // node->IsAsk = e->IsAsk;
            // node->Quantity = e->Quantity;
            // node->prev_ = nullptr;
            // node->next_ = nullptr;

            if (node->isAsk) {
                ++askCount_;
                sumAskQty_ += node->quantity;

                // Insert into askMap and then update linked list
                auto [mapIt, inserted] = askMap_.insert({node->price, node});

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
                        node->prev_ = prevNode;
                        node->next_ = prevNode->next_;

                        if (prevNode->next_) prevNode->next_->prev_ = node;
                        else askTail_ = node;

                        prevNode->next_ = node;
                    } else {
                        // Insert at the beginning
                        node->next_ = askHead_;
                        askHead_->prev_ = node;
                        node->prev_ = nullptr;
                        askHead_ = node;
                    }
                }
            }
            else {
                ++bidCount_;
                sumBidQty_ += node->quantity;

                // Insert into bidMap and then update linked list
                auto [mapIt, inserted] = bidMap_.insert({node->price, node});

                // Find the correct position in the linked list
                if (bidMap_.size() == 1) {
                    // First element
                    bidHead_ = bidTail_ = node;
                }
                else {
                    auto it = mapIt;

                    // If not the first element in the map
                    if (it != bidMap_.begin()) {
                        auto prevIt = std::prev(it);
                        DifferenceDepthEntry* prevNode = prevIt->second;

                        // Insert after prevNode
                        node->prev_ = prevNode;
                        node->next_ = prevNode->next_;

                        if (prevNode->next_) prevNode->next_->prev_ = node;
                        else bidTail_ = node;

                        prevNode->next_ = node;
                    }
                    else {
                        // Insert at the beginning
                        node->next_ = bidHead_;
                        bidHead_->prev_ = node;
                        node->prev_ = nullptr;
                        bidHead_ = node;
                    }
                }
            }
            index_[key] = node;
        }
    }
}

void OrderBook::printOrderBook() const {
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "ORDERBOOK:\n";
    std::cout << "  Asks (lowest→highest):\n";
    for (auto *n = askTail_; n; n = n->prev_) {
        std::cout
        << n->timestampOfReceive << " "
        << n->symbol << " "
        << n->isAsk << " "
        << n->price << " "
        << n->quantity << " "
        << n->market << " "
        << "\n";
    }

    std::cout << "  Bids (highest→lowest):\n";
    for (auto *n = bidHead_; n; n = n->next_) {
        std::cout
        << n->timestampOfReceive << " "
        << n->symbol << " "
        << n->isAsk << " "
        << n->price << " "
        << n->quantity << " "
        << n->market << " "
        << "\n";
    }
    std::cout << std::endl;
}

std::vector<DifferenceDepthEntry> OrderBook::getAsks() const {
    std::vector<DifferenceDepthEntry> result;
    result.reserve(askCount_);
    for (auto *node = askHead_; node; node = node->next_) {
        result.push_back(*node);
    }
    return result;
}

std::vector<DifferenceDepthEntry> OrderBook::getBids() const {
    std::vector<DifferenceDepthEntry> result;
    result.reserve(bidCount_);
    for (auto *node = bidHead_; node; node = node->next_) {
        result.push_back(*node);
    }
    return result;
}