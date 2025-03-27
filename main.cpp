#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

struct OrderBookEntry {
    int64_t TimestampOfReceive;
    int64_t EventTime;
    std::string Symbol;
    int64_t FirstUpdateId;
    int64_t FinalUpdateId;
    bool IsAsk;
    double Price;
    double Quantity;
};

void mainOrderbookEntryLoop();

std::vector<OrderBookEntry> getOrderbookEntriesFromCSV(const std::string & csvPath);

std::vector<std::string> split(const std::string & line, char delimiter);

// #ifdef MODULE_TEST
int main() {
    mainOrderbookEntryLoop();
    return 0;
}
// #endif



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

std::vector<OrderBookEntry> getOrderbookEntriesFromCSV(const std::string &csvPath) {
    std::vector<OrderBookEntry> entries;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku: " + csvPath);
    }

    std::string line;
    bool headerSkipped = false;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        auto tokens = split(line, ',');
        if (tokens.size() < 12) {
            std::cerr << "Niepoprawny format linii: " << line << std::endl;
            continue;
        }

        try {
            OrderBookEntry entry;
            entry.TimestampOfReceive = std::stoll(tokens[0]);

            entry.EventTime          = std::stoll(tokens[3]);

            entry.Symbol             = tokens[5];
            entry.FirstUpdateId      = std::stoll(tokens[6]);
            entry.FinalUpdateId      = std::stoll(tokens[7]);

            entry.IsAsk              = (std::stoi(tokens[9]) != 0);
            entry.Price              = std::stod(tokens[10]);
            entry.Quantity           = std::stod(tokens[11]);

            entries.push_back(entry);
        } catch (const std::exception &e) {
            std::cerr << "Błąd przetwarzania linii: " << line << " - " << e.what() << std::endl;
        }
    }
    file.close();
    return entries;
}

std::vector<std::string> split(const std::string &line, const char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

PYBIND11_MODULE(orderbook, m) {
    py::class_<OrderBookEntry>(m, "OrderBookEntry")
        .def_readonly("TimestampOfReceive", &OrderBookEntry::TimestampOfReceive)
        .def_readonly("EventTime", &OrderBookEntry::EventTime)
        .def_readonly("Symbol", &OrderBookEntry::Symbol)
        .def_readonly("FirstUpdateId", &OrderBookEntry::FirstUpdateId)
        .def_readonly("FinalUpdateId", &OrderBookEntry::FinalUpdateId)
        .def_readonly("IsAsk", &OrderBookEntry::IsAsk)
        .def_readonly("Price", &OrderBookEntry::Price)
        .def_readonly("Quantity", &OrderBookEntry::Quantity);

    m.def("loadCSV", &getOrderbookEntriesFromCSV, "Wczytuje plik CSV i zwraca listę wpisów OrderBookEntry", py::arg("directory"));
}
