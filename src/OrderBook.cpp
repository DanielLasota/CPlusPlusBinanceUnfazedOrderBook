#include <algorithm>
#include <iostream>
#include <iomanip>

#include "OrderBook.h"

OrderBook::OrderBook(const size_t maxLevels) {
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
    node->price = price;
    node->isAsk = isAsk;
    node->quantity = quantity;
    node->prev_ = node->next_ = nullptr;
    return node;
}

void OrderBook::deallocateNode(DifferenceDepthEntry* node) {
    node->next_ = freeListHead_;
    freeListHead_ = node;
}

void OrderBook::addNode(DifferenceDepthEntry*& head, DifferenceDepthEntry*& tail, DifferenceDepthEntry* node, bool isAsk) {
    if (!head) {
        head = tail = node;
        node->prev_ = node->next_ = nullptr;
        return;
    }

    if (isAsk) {
        auto it = askMap_.find(node->price);
        if (it == askMap_.end()) {
            node->next_ = head;
            head->prev_ = node;
            node->prev_ = nullptr;
            head = node;
        }
        else {
            auto nextIt = std::next(it);
            if (nextIt == askMap_.end()) {
                tail->next_ = node;
                node->prev_ = tail;
                node->next_ = nullptr;
                tail = node;
            } else {
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
        auto it = bidMap_.find(node->price);
        if (it == bidMap_.end()) {
            node->next_ = head;
            head->prev_ = node;
            node->prev_ = nullptr;
            head = node;
        } else {
            auto nextIt = std::next(it);
            if (nextIt == bidMap_.end()) {
                tail->next_ = node;
                node->prev_ = tail;
                node->next_ = nullptr;
                tail = node;
            } else {
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
    PriceSide key{ e->price, e->isAsk };
    auto it = index_.find(key);

    if (e->quantity == 0.0) {
        if (it != index_.end()) {
            auto *node = it->second;
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
            sumOfPriceTimesQuantity_  -= node->price * node->quantity;
            index_.erase(it);
            deallocateNode(node);
        }
    }
    else {
        if (it != index_.end()) {
            auto *node = it->second;

            double oldQ = node->quantity;
            double newQ = e->quantity;

            if (node->isAsk) sumAskQty_ += newQ - oldQ;
            else sumBidQty_ += newQ - oldQ;

            sumOfPriceTimesQuantity_ += node->price * (newQ - oldQ);

            node->timestampOfReceive = e->timestampOfReceive;
            node->quantity = e->quantity;
            node->isLast = e->isLast;
        }
        else {
            auto *node = allocateNode(e->price, e->isAsk, e->quantity);

            DifferenceDepthEntry temp = *e;
            *node = std::move(temp);

            node->prev_ = nullptr;
            node->next_ = nullptr;

            if (node->isAsk) {
                ++askCount_;
                sumAskQty_ += node->quantity;

                auto [mapIt, inserted] = askMap_.insert({node->price, node});

                if (askMap_.size() == 1) {
                    askHead_ = askTail_ = node;
                } else {
                    auto it = mapIt;

                    if (it != askMap_.begin()) {
                        auto prevIt = std::prev(it);
                        DifferenceDepthEntry* prevNode = prevIt->second;

                        node->prev_ = prevNode;
                        node->next_ = prevNode->next_;

                        if (prevNode->next_) prevNode->next_->prev_ = node;
                        else askTail_ = node;

                        prevNode->next_ = node;
                    } else {
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

                auto [mapIt, inserted] = bidMap_.insert({node->price, node});

                if (bidMap_.size() == 1) {
                    bidHead_ = bidTail_ = node;
                }
                else {
                    auto it = mapIt;

                    if (it != bidMap_.begin()) {
                        auto prevIt = std::prev(it);
                        DifferenceDepthEntry* prevNode = prevIt->second;

                        node->prev_ = prevNode;
                        node->next_ = prevNode->next_;

                        if (prevNode->next_) prevNode->next_->prev_ = node;
                        else bidTail_ = node;

                        prevNode->next_ = node;
                    }
                    else {
                        node->next_ = bidHead_;
                        bidHead_->prev_ = node;
                        node->prev_ = nullptr;
                        bidHead_ = node;
                    }
                }
            }

            sumOfPriceTimesQuantity_ += node->price * node->quantity;
            index_[key] = node;
        }
    }
}

void OrderBook::printOrderBook() const {
    // std::cout << std::fixed << std::setprecision(5);
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

double OrderBook::cumulativeQuantityOfTopNAsks(size_t n) const {
    double sum = 0;
    auto *node = askHead_;
    for (size_t i = 0; i < n && node; ++i, node = node->next_) {
        sum += node->quantity;
    }
    return sum;
}

double OrderBook::cumulativeQuantityOfTopNBids(size_t n) const {
    double sum = 0;
    auto *node = bidHead_;
    for (size_t i = 0; i < n && node; ++i, node = node->next_) {
        sum += node->quantity;
    }
    return sum;
}

double OrderBook::bestNthAskPrice(const size_t K) const {
    const DifferenceDepthEntry* node = askHead_;
    for (size_t i = 1; i < K && node; ++i)
        node = node->next_;
    return node ? node->price : std::numeric_limits<double>::quiet_NaN();
}

double OrderBook::bestNthBidPrice(const size_t K) const {
    const DifferenceDepthEntry* node = bidHead_;
    for (size_t i = 1; i < K && node; ++i)
        node = node->next_;
    return node ? node->price : std::numeric_limits<double>::quiet_NaN();
}
