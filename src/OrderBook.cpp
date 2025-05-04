// ===== OrderBook.cpp =====
#include "OrderBook.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include "engine.h"

// Helper do wstawiania FIFO
void OrderBook::ppInsertOrder(PricePoint& pp, OrderNode* node) {
    if (pp.head) {
        pp.tail->next = node;
    } else {
        pp.head = node;
    }
    pp.tail = node;
    node->next = nullptr;
}

OrderBook::OrderBook()
  : askMin(MAX_PRICE+1)
  , bidMax(0)
  , curOrderID(0)
{
    // inicjalizacja pricePoints
    for (size_t i = 0; i <= MAX_PRICE; ++i) {
        pricePoints[i].head = pricePoints[i].tail = nullptr;
    }
}

// Core: algorytm Price–Time Matching Engine
void OrderBook::addOrder(const DifferenceDepthEntry& upd) {
    size_t price    = static_cast<size_t>(upd.Price);
    size_t qty      = static_cast<size_t>(upd.Quantity);
    bool   isAsk    = upd.IsAsk;

    // CANCEL: jeżeli przyszło upd.Quantity==0 => oznacz w arena[ID] jako 0
    if (qty == 0) {
        // Tu musiałbyś mieć mapę orderID→node, aby znaleźć
        // i ustawić node->entry.Quantity=0.
        // Pomijam implementację cancel, bo w Twoim kodzie
        // nie było trackingu ID w Entry.
        return;
    }

    // MATCHING
    if (!isAsk) {
        // **Buy**: szukamy sell orders od askMin do price
        if (askMin <= price) {
            size_t lvl = askMin;
            while (lvl <= price && qty > 0) {
                auto& pp = pricePoints[lvl];
                OrderNode* node = pp.head;
                OrderNode* prev = nullptr;
                // przejście FIFO po liście
                while (node && qty > 0) {
                    if (node->entry.Quantity == 0) {
                        // anulowane – skip
                        prev = node;
                            node = node->next;
                        continue;
                    }
                    size_t avail = static_cast<size_t>(node->entry.Quantity);
                    size_t traded = std::min(avail, qty);
                    // wygeneruj EXECUTE_TRADE(...) z Twojego silnika
                    EXECUTE_TRADE(
                      upd.Symbol.c_str(),
                      /*buyTrader=*/upd.PSUnknownField.c_str(),
                      /*sellTrader=*/node->entry.PSUnknownField.c_str(),
                      /*price*/ static_cast<t_price>(lvl),
                      /*size*/  traded
                    );
                    // aktualizacja
                    node->entry.Quantity -= traded;
                    qty -= traded;
                    if (node->entry.Quantity == 0) {
                        // usuwamy z listy głowę
                        pp.head = node->next;
                        node = pp.head;
                    } else {
                        // partial fill => koniec matching’u na tej nodzie
                        break;
                    }
                }
                if (pp.head==nullptr) {
                    // posun askMin do góry
                    ++lvl;
                    askMin = lvl;
                } else {
                    // dalej na tym cenowym nie ma co czyścić head
                    break;
                }
            }
        }
        // RESZTA zlecenia => trafia do booka
        if (qty > 0) {
            OrderNode* node = &arena[++curOrderID];
            node->entry = upd;
            node->entry.Quantity = qty;
            ppInsertOrder(pricePoints[price], node);
            bidMax = std::max(bidMax, price);
        }
    }
    else {
        // **Sell**: symetrycznie – od bidMax w dół do price
        if (bidMax >= price) {
            size_t lvl = bidMax;
            while (lvl >= price && qty > 0) {
                auto& pp = pricePoints[lvl];
                OrderNode* node = pp.head;
                while (node && qty > 0) {
                    if (node->entry.Quantity == 0) {
                        node = node->next;
                        continue;
                    }
                    size_t avail = static_cast<size_t>(node->entry.Quantity);
                    size_t traded = std::min(avail, qty);
                    EXECUTE_TRADE(
                      upd.Symbol.c_str(),
                      /*buyTrader=*/node->entry.PSUnknownField.c_str(),
                      /*sellTrader=*/upd.PSUnknownField.c_str(),
                      /*price=*/ static_cast<t_price>(lvl),
                      /*size=*/ traded
                    );
                    node->entry.Quantity -= traded;
                    qty -= traded;
                    if (node->entry.Quantity == 0) {
                        pp.head = node->next;
                        node = pp.head;
                    } else {
                        break;
                    }
                }
                if (pp.head==nullptr) {
                    --lvl;
                    bidMax = lvl;
                } else {
                    break;
                }
            }
        }
        if (qty > 0) {
            OrderNode* node = &arena[++curOrderID];
            node->entry = upd;
            node->entry.Quantity = qty;
            ppInsertOrder(pricePoints[price], node);
            askMin = std::min(askMin, price);
        }
    }
}

std::vector<DifferenceDepthEntry*> OrderBook::getAsks() const {
    std::vector<DifferenceDepthEntry*> out;
    for (size_t p = askMin; p <= MAX_PRICE; ++p) {
        for (auto* node = pricePoints[p].head; node; node = node->next) {
            if (node->entry.Quantity > 0)
                out.push_back(&node->entry);
        }
    }
    return out;
}

std::vector<DifferenceDepthEntry*> OrderBook::getBids() const {
    std::vector<DifferenceDepthEntry*> out;
    for (size_t p = bidMax + 1; p-- > 0; ) {
        for (auto* node = pricePoints[p].head; node; node = node->next) {
            if (node->entry.Quantity > 0)
                out.push_back(&node->entry);
        }
    }
    return out;
}

// Wypisanie booka (stosunkowo rzadko używane)
void OrderBook::printOrderBook() const {
    std::cout << "===== ORDERBOOK SNAPSHOT =====\n";
    std::cout << " Asks (od low do high):\n";
    for (size_t p = askMin; p <= MAX_PRICE; ++p) {
        for (OrderNode* n = pricePoints[p].head; n; n = n->next) {
            if (n->entry.Quantity>0)
                std::cout
                  << "  price="<<p
                  << " qty="<<n->entry.Quantity
                  << "\n";
        }
    }
    std::cout << " Bids (od high do low):\n";
    for (size_t p = bidMax+1; p-->0;) {
        for (OrderNode* n = pricePoints[p].head; n; n = n->next) {
            if (n->entry.Quantity>0)
                std::cout
                  << "  price="<<p
                  << " qty="<<n->entry.Quantity
                  << "\n";
        }
    }
    std::cout << "==============================\n";
}
