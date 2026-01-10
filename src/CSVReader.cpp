#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "CSVReader.h"
#include "CSVHeader.h"
#include "EntryDecoder.h"
#include "mmap_win.h"
#include "enums/AssetParameters.h"

inline std::vector<std::string_view> splitSVByNewLine(std::string_view sv) {
    std::vector<std::string_view> out;
    size_t start = 0;

    while (start < sv.size()) {
        size_t end = sv.find('\n', start);
        if (end == std::string_view::npos) {
            out.emplace_back(sv.substr(start));
            break;
        }
        out.emplace_back(sv.substr(start, end - start));
        start = end + 1;
    }
    return out;
}

std::vector<std::string> CSVReader::splitLine(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<XEvent> CSVReader::readSingleAssetCSV(const std::string &csvPath) {
    AssetParameters assetParameters = AssetParameters::decodeAssetParametersFromSingleCSVName(csvPath);

    MMapData mm = mmap_file(csvPath);
    std::string_view file_view(mm.data, mm.size);

    auto lines = splitSVByNewLine(file_view);

    std::vector<XEvent> entries;
    entries.reserve(lines.size());

    bool headerSkipped = false;
    for (auto&& line_sv : lines) {
        if (line_sv.empty() || line_sv[0] == '#') continue;
        if (!headerSkipped) {
            headerSkipped = true;
            continue; // skip header
        }
        try {
            XEvent entry = EntryDecoder::decodeSingleAssetParameterEntry(assetParameters, line_sv);
            entries.push_back(std::move(entry));
        } catch (const std::exception &e) {
            std::cerr << "Error processing the line: " << std::string(line_sv) << " - " << e.what() << std::endl;
        }
    }

    munmap_file(mm);
    return entries;
}

std::vector<XEvent> CSVReader::readMultiAssetCSV(const std::string &csvPath) {
    MMapData mm = mmap_file(csvPath);
    const std::string_view file_view(mm.data, mm.size);

    const auto lines = splitSVByNewLine(file_view);

    std::string_view headerLine;
    size_t headerIdx = 0;
    for (; headerIdx < lines.size(); ++headerIdx) {
        if (!lines[headerIdx].empty() && lines[headerIdx][0] != '#') {
            headerLine = lines[headerIdx];
            break;
        }
    }

    if (headerLine.empty()) throw std::runtime_error("Header not found in file: " + csvPath);

    const std::vector<std::string_view> headerTokens = splitLineSV(headerLine, ',');
    const ColMap colMap = buildColMap(headerTokens);

    std::vector<XEvent> entries;
    entries.reserve(lines.size());

    for (size_t i = headerIdx + 1; i < lines.size(); ++i) {
        auto& line_sv = lines[i];
        if (line_sv.empty() || line_sv[0] == '#') continue;
        try {
            XEvent entry = EntryDecoder::decodeMultiAssetParameterEntry(line_sv, colMap);
            entries.push_back(std::move(entry));
        }
        catch (const std::exception &e) {
            std::cerr << "Error processing line: " << std::string(line_sv) << " - " << e.what() << std::endl;
        }
    }

    munmap_file(mm);
    return entries;
}
