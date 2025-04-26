#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>

class CSVHeader {
public:
    // Parsuje headerLine (rozdzielone przecinkami) i buduje mapę kolumna→indeks
    explicit CSVHeader(const std::string &headerLine);

    // Zwraca indeks kolumny (rzuca std::out_of_range, jeśli nie ma)
    size_t operator[](std::string_view col) const;

    // Sprawdza, czy kolumna istnieje
    bool contains(std::string_view col) const;

private:
    std::vector<std::string>                  names_;  // bufor na nazwy
    std::unordered_map<std::string_view,size_t> idx_;    // mapowanie view→indeks
};