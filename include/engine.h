// ===== include/engine.h =====
#pragma once

#include <cstdint>

// Podstawowe aliasy
using t_price   = uint32_t;
using t_size    = uint32_t;
using t_orderid = uint32_t;

// Struktura raportu wykonania
struct t_execution {
    char   symbol[6];  // do 5 znaków + '\0'
    t_price price;
    t_size  size;
    int     side;      // 0 = buy, 1 = sell
    char   trader[6];  // do 5 znaków + '\0'
};

// Struktura przychodzącego zlecenia
struct t_order {
    char   symbol[6];
    char   trader[6];
    int     side;     // 0 = buy, 1 = sell
    t_price price;
    t_size  size;
};

// Funkcja, którą silnik wywołuje za każdym razem, gdy dochodzi do transakcji.
void execution(const t_execution& exec);

// Interfejs silnika matchingowego (nie musisz z niego korzystać, jeśli używasz EXECUTE_TRADE)
t_orderid limit(const t_order& order);
void cancel(t_orderid orderid);

// Makro/func do wywoływania dwóch wywołań `execution()` (buyer + seller)
void EXECUTE_TRADE(const char* symbol,
                   const char* buyTrader,
                   const char* sellTrader,
                   t_price tradePrice,
                   t_size tradeSize);
