// src/CSVParser.cpp
#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

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

std::vector<std::string> split(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
