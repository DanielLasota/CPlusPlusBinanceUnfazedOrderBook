// ===== src/engine.cpp =====
#include "engine.h"
#include <cstring>

// ten stub wypisuje wykonane transakcje na stdout
void execution(const t_execution& exec) {
    // np. "TRADE AAPL BUY size=10 price=123 trader=ALICE"
//    printf("TRADE %s %s size=%u price=%u trader=%s\n",
//           exec.symbol,
//           exec.side == 0 ? "BUY" : "SELL",
//           exec.size,
//           exec.price,
//           exec.trader);
}

// puste stuby, żeby zaspokoić deklaracje z engine.h
t_orderid limit(const t_order& /*order*/) {
    return 0;
}
void cancel(t_orderid /*orderid*/) {}

// makro-funkcja wywołująca execution() dwukrotnie
void EXECUTE_TRADE(const char* symbol,
                   const char* buyTrader,
                   const char* sellTrader,
                   t_price tradePrice,
                   t_size tradeSize)
{
    t_execution e{};
    // pierwsze wywołanie dla kupującego
    strncpy(e.symbol, symbol, 5);    e.symbol[5]    = '\0';
    e.price       = tradePrice;
    e.size        = tradeSize;
    e.side        = 0;
    strncpy(e.trader, buyTrader, 5); e.trader[5]    = '\0';
    execution(e);

    // drugie dla sprzedającego
    e.side        = 1;
    strncpy(e.trader, sellTrader, 5); e.trader[5]   = '\0';
    execution(e);
}
