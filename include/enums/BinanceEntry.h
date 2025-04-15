//
// Created by daniel on 14.04.2025.
//

#ifndef BINANCEENTRY_H
#define BINANCEENTRY_H
#include <variant>

#include <enums/TradeEntry.h>
#include <enums/DifferenceDepthEntry.h>


using BinanceEntry = std::variant<
    DifferenceDepthEntry,
    TradeEntry
>;

#endif //BINANCEENTRY_H
