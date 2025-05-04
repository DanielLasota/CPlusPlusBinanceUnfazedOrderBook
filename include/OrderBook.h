// ===== OrderBook.h =====
#pragma once

#include "enums/OrderBookEntry.h"
#include <cstddef>
#include <cstdint>
#include <vector>

static constexpr size_t MAX_PRICE      = 500'000;    // maksymalna cena (dostosuj)
static constexpr size_t MAX_NUM_ORDERS = 10'000'000;   // maks. liczba zleceń

// Pojedynczy węzeł w liście zleceń
struct OrderNode {
    DifferenceDepthEntry entry;
    OrderNode*          next;
};

// Lista zleceń na jednym poziomie cenowym
struct PricePoint {
    OrderNode* head;
    OrderNode* tail;
};

class OrderBook {
public:
    OrderBook();

    // Przyjmuje nową aktualizację (DifferenceDepthEntry z Binance),
    // jeżeli Quantity>0 => to limit, jeśli 0 => cancel
    void addOrder(const DifferenceDepthEntry& upd);

    // Dla debugu: wypisuje „snapshot” booka
    void printOrderBook() const;

    std::vector<DifferenceDepthEntry*> getAsks() const;
    std::vector<DifferenceDepthEntry*> getBids() const;

private:
    PricePoint  pricePoints[MAX_PRICE + 1];
    size_t      askMin;        // najniższy poziom zaskowany (ask)
    size_t      bidMax;        // najwyższy poziom zbiddowany (bid)

    // arena na OrderNode’y
    OrderNode   arena[MAX_NUM_ORDERS];
    size_t      curOrderID;    // alokator – kolejne ID węzła

    // Wstawia węzeł na tail listy pp
    static void ppInsertOrder(PricePoint& pp, OrderNode* node);
};
