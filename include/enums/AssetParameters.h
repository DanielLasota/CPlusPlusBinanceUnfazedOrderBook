#ifndef ASSETPARAMETERS_H
#define ASSETPARAMETERS_H

#include <string>
#include "Market.h"
#include "StreamType.h"

struct AssetParameters {
    Market market;
    StreamType stream_type;
    std::string pair;
    std::string date;
};

std::ostream& operator<<(std::ostream& os, const Market& market);
std::ostream& operator<<(std::ostream& os, const StreamType& streamType);
std::ostream& operator<<(std::ostream& os, const AssetParameters& params);

#endif // ASSETPARAMETERS_H

