//
// Created by daniel on 2.04.2025.
//

#ifndef ENTRYDECODER_H
#define ENTRYDECODER_H

#include <string>
#include <vector>

#include <enums/AssetParameters.h>
#include <enums/BinanceEntry.h>


class EntryDecoder {
public:
    static BinanceEntry decodeSingleCSVEntry(const AssetParameters &params, const std::string &line);

    static BinanceEntry decodeMergedCSVEntry(const AssetParameters &params, const std::string &line);

private:
    static std::vector<std::string> splitLine(const std::string &line, char delimiter);
};

#endif // ENTRYDECODER_H

