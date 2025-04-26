#include "CSVHeader.h"
#include <stdexcept>

CSVHeader::CSVHeader(const std::string &headerLine) {
    size_t start = 0;
    while (true) {
        auto pos = headerLine.find(',', start);
        auto len = (pos == std::string::npos
                    ? headerLine.size() - start
                    : pos - start);

        names_.emplace_back(headerLine.substr(start, len));

        if (pos == std::string::npos)
            break;
        start = pos + 1;
    }

    idx_.reserve(names_.size());
    for (size_t i = 0; i < names_.size(); ++i) {
        idx_.emplace(std::string_view{names_[i]}, i);
    }
}

size_t CSVHeader::operator[](std::string_view col) const {
    auto it = idx_.find(col);
    if (it == idx_.end()) {
        throw std::out_of_range("CSVHeader: brak kolumny `" + std::string(col) + "`");
    }
    return it->second;
}

bool CSVHeader::contains(std::string_view col) const {
    return idx_.find(col) != idx_.end();
}
