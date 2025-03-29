#ifndef ORDERBOOK_SESSION_SIMULATOR_H
#define ORDERBOOK_SESSION_SIMULATOR_H

#include "OrderBook.h"
#include <string>

class OrderbookSessionSimulator {
public:
    OrderbookSessionSimulator();

    void processOrderbook(const std::string& csvPath);

private:
    OrderBook orderbook;
};

#endif
