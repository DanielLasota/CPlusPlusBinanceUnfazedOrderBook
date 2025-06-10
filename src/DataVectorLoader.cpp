#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "CSVHeader.h"
#include "DataVectorLoader.h"
#include "enums/AssetParameters.h"
#include "EntryDecoder.h"
#include "mmap_win.h"

inline std::vector<std::string_view> split_sv_by_newline(std::string_view sv) {
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

std::vector<std::string> DataVectorLoader::splitLine(const std::string &line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<DecodedEntry> DataVectorLoader::getEntriesFromSingleAssetParametersCSV(const std::string &csvPath) {
    AssetParameters assetParameters = AssetParameters::decodeAssetParametersFromSingleCSVName(csvPath);

    MMapData mm = mmap_file(csvPath);
    std::string_view file_view(mm.data, mm.size);

    auto lines = split_sv_by_newline(file_view);

    std::vector<DecodedEntry> entries;
    entries.reserve(lines.size());

    bool headerSkipped = false;
    for (auto&& line_sv : lines) {
        if (line_sv.empty() || line_sv[0] == '#') continue;
        if (!headerSkipped) {
            headerSkipped = true;
            continue; // skip header
        }
        try {
            DecodedEntry entry = EntryDecoder::decodeSingleAssetParameterEntry(assetParameters, line_sv);
            entries.push_back(std::move(entry));
        } catch (const std::exception &e) {
            std::cerr << "Error processing the line: " << std::string(line_sv) << " - " << e.what() << std::endl;
        }
    }

    munmap_file(mm);
    return entries;
}

std::vector<DecodedEntry> DataVectorLoader::getEntriesFromMultiAssetParametersCSV(const std::string &csvPath) {
    // auto start0 = std::chrono::steady_clock::now();

    MMapData mm = mmap_file(csvPath);
    std::string_view file_view(mm.data, mm.size);
    // auto start1 = std::chrono::steady_clock::now();

    auto lines = split_sv_by_newline(file_view);

    // auto start2 = std::chrono::steady_clock::now();

    std::string_view headerLine;
    size_t headerIdx = 0;
    for (; headerIdx < lines.size(); ++headerIdx) {
        if (!lines[headerIdx].empty() && lines[headerIdx][0] != '#') {
            headerLine = lines[headerIdx];
            break;
        }
    }
    if (headerLine.empty())
        throw std::runtime_error("Header not found in file: " + csvPath);

    std::vector<std::string_view> headerTokens = splitLineSV(headerLine, ',');
    ColMap colMap = buildColMap(headerTokens);

    std::vector<DecodedEntry> entries;
    entries.reserve(lines.size());

    for (size_t i = headerIdx + 1; i < lines.size(); ++i) {
        auto& line_sv = lines[i];
        if (line_sv.empty() || line_sv[0] == '#') continue;
        try {
            DecodedEntry entry = EntryDecoder::decodeMultiAssetParameterEntry(line_sv, colMap);
            entries.push_back(std::move(entry));
        }
        catch (const std::exception &e) {
            std::cerr << "Error processing line: " << std::string(line_sv)
                      << " - " << e.what() << std::endl;
        }
    }

    // auto start3 = std::chrono::steady_clock::now();
    // auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(start1 - start0).count();
    // auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(start2 - start1).count();
    // auto elapsed_ms3 = std::chrono::duration_cast<std::chrono::milliseconds>(start3 - start2).count();
    // std::cout << "mmap: " << elapsed_ms1 << " ms" << std::endl;
    // std::cout << "split_sv_by_newline: " << elapsed_ms2 << " ms" << std::endl;
    // std::cout << "decodeMultiAssetParameterEntry loop: " << elapsed_ms3 << " ms" << std::endl;

    munmap_file(mm);
    return entries;
}
