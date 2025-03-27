// src/main.cpp
#include <iostream>
#include <chrono>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVParser.h"

void mainOrderbookEntryLoop();

int main() {
    mainOrderbookEntryLoop();
    return 0;
}

void mainOrderbookEntryLoop() {
    try {
        std::string directory = "C:/Users/daniel/Documents/binance_archival_data/"
                                "binance_difference_depth_stream_usd_m_futures_trxusdt_25-03-2025.csv";

        auto entries = getOrderbookEntriesFromCSV(directory);

        std::vector<OrderBookEntry*> ptr_entries;
        ptr_entries.reserve(entries.size());
        for (auto &entry : entries) {
            ptr_entries.push_back(&entry);
        }

        OrderBookEntry** data = ptr_entries.data();
        size_t count = ptr_entries.size();

        auto start = std::chrono::steady_clock::now();

        for (size_t i = 0; i < count; ++i) {
            OrderBookEntry* entry = data[i];
            // Przykładowe przetwarzanie (tu odkomentować, jeśli chcesz wypisywać dane)
            // std::cout << entry->TimestampOfReceive << std::endl;
            // std::cout << "Timestamp: " << entry->TimestampOfReceive
            //           << ", Symbol: " << entry->Symbol
            //           << ", Price: " << entry->Price
            //           << ", Quantity: " << entry->Quantity << std::endl;
        }

        auto finish = std::chrono::steady_clock::now();

        auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        auto finish_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish.time_since_epoch()).count();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

        std::cout << "Start timestamp (ms): " << start_ms << std::endl;
        std::cout << "Finish timestamp (ms): " << finish_ms << std::endl;
        std::cout << "Czas iteracji: " << elapsed_ms << " ms" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Wyjątek: " << e.what() << std::endl;
    }
}
