#include <algorithm>
#include <iostream>
#include <type_traits>

#include "Resampler.h"

std::optional<MetricsEntry> Resampler::update(const MetricsEntry &me) {
    const uint64_t currTsId = me.timestampOfReceive / cfg.resolutionUs;

    switch (cfg.method) {
        case ResampleMethod::LAST: {
            if (!hasPrev) {
                hasPrev = true;
                prevTsId = currTsId;
                prevMe = me;
                return std::nullopt;
            }

            if (prevTsId != currTsId) {
                const MetricsEntry out = prevMe;
                prevTsId = currTsId;
                prevMe = me;
                return out;
            }

            prevMe = me;
            return std::nullopt;
        }

        case ResampleMethod::MEAN: {
            if (!hasPrev) {
                hasPrev = true;
                prevTsId = currTsId;
                resetAvg(me);
                return std::nullopt;
            }

            if (prevTsId != currTsId) {
                prevTsId = currTsId;

                const MetricsEntry out = calcAvg();
                resetAvg(me);

                return out;
            }

            updateAvg(me);
            return std::nullopt;
        }

        case ResampleMethod::HLC3: {
            if (!hasPrev) {
                hasPrev = true;
                prevTsId = currTsId;
                resetHLC3(me);
                return std::nullopt;
            }
            if (prevTsId != currTsId) {
                prevTsId = currTsId;
                const MetricsEntry out = calcHLC3();
                resetHLC3(me);
                return out;
            }

            updateHLC3(me);
            return std::nullopt;
        }
        default: ;
    }

    return std::nullopt;


    return std::nullopt;
}

void Resampler::resetAvg(const MetricsEntry &first) {
    meanMe = first;
    count = 1;
}

void Resampler::updateAvg(const MetricsEntry& e) {
    ++count;

    #define METRIC(name, ctype) \
        if constexpr (std::is_same_v<ctype, double>) { meanMe.name += e.name; } \
        else { meanMe.name = e.name; }
    #include "detail/metrics_list.def"
    #undef METRIC
}

MetricsEntry Resampler::calcAvg() const {
    MetricsEntry out = meanMe;
    const double invCount = 1.0 / static_cast<double>(count);

    #define METRIC(name, ctype) \
        if constexpr (std::is_same_v<ctype, double>) { out.name *= invCount; }\
        else if constexpr (std::is_same_v<ctype, bool>) { out.name *= static_cast<double>(invCount); }
    #include "detail/metrics_list.def"
    #undef METRIC

    return out;
}

void Resampler::resetHLC3(const MetricsEntry& firstMe) {
    hiMe = firstMe;
    loMe = firstMe;
    closeMe = firstMe;
}

void Resampler::updateHLC3(const MetricsEntry& me) {
    #define METRIC(name, ctype) \
        do { \
            if constexpr (std::is_same_v<ctype, double>) { \
                hiMe.name = std::max(hiMe.name, me.name); \
                loMe.name = std::min(loMe.name, me.name); \
            } \
            closeMe.name = me.name; \
        } while (0);
    #include "detail/metrics_list.def"
    #undef METRIC
}

MetricsEntry Resampler::calcHLC3() const {
    MetricsEntry out{};

    #define METRIC(name, ctype) \
        do { \
            if constexpr (std::is_same_v<ctype, double>) { \
                out.name = (hiMe.name + loMe.name + closeMe.name) / 3.0; \
            } else { \
                out.name = closeMe.name; \
            } \
        } while (0);
    #include "detail/metrics_list.def"
    #undef METRIC

    return out;
}
