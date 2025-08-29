#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <array>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <sstream>


enum : size_t {
    #define METRIC(name, ctype) METRIC_INDEX_##name,
    #include "detail/metrics_list.def"
    #undef METRIC
        METRICS_COUNT
};

static_assert(METRICS_COUNT <= 64, "MetricMask (uint64_t) pomieści max 64 metryki");


enum Metric : uint64_t {
    #define METRIC(name, ctype) name = (1ULL << METRIC_INDEX_##name),
    #include "detail/metrics_list.def"
    #undef METRIC
};

using MetricMask = uint64_t;

inline const std::array<std::string_view, METRICS_COUNT>& allMetricNames() {
    static constexpr std::array<std::string_view, METRICS_COUNT> kNames = {
        #define METRIC(name, ctype) #name,
        #include "detail/metrics_list.def"
        #undef METRIC
    };
    return kNames;
}



[[nodiscard]] inline constexpr bool has(MetricMask mask, Metric m) noexcept {
    return (mask & static_cast<MetricMask>(m)) != 0;
}
inline constexpr void set(MetricMask& mask, Metric m) noexcept {
    mask |= static_cast<MetricMask>(m);
}
inline constexpr void clear(MetricMask& mask, Metric m) noexcept {
    mask &= ~static_cast<MetricMask>(m);
}

inline std::vector<std::string_view> expandMetricNames(const MetricMask mask) {
    std::vector<std::string_view> out;
    out.reserve(32);
    const auto& names = allMetricNames();
    for (size_t bit = 0; bit < names.size(); ++bit) {
        if (mask & (1ULL << bit)) out.push_back(names[bit]);
    }
    return out;
}


namespace detail {
    inline const std::unordered_map<std::string_view, Metric>& lutNameToMetric() {
        static const std::unordered_map<std::string_view, Metric> map = []{
            std::unordered_map<std::string_view, Metric> m;
            m.reserve(METRICS_COUNT * 2);
            #define METRIC(name, ctype) m.emplace(std::string_view{#name}, Metric(name));
            #include "detail/metrics_list.def"
            #undef METRIC
            return m;
        }();
        return map;
    }
}

inline MetricMask parseMask(const std::vector<std::string>& vars) {
    const auto& lut = detail::lutNameToMetric();
    MetricMask mask = 0;
    std::vector<std::string> unknowns;
    unknowns.reserve(4);

    for (const auto& s : vars) {
        if (auto it = lut.find(std::string_view{s}); it != lut.end()) {
            mask |= static_cast<MetricMask>(it->second);
        } else {
            unknowns.push_back(s);
        }
    }

    if (!unknowns.empty()) {
        std::ostringstream oss;
        oss << "Unknown variable name";
        if (unknowns.size() > 1) oss << "s";
        oss << ": ";
        for (size_t i = 0; i < unknowns.size(); ++i) {
            oss << unknowns[i];
            if (i + 1 < unknowns.size()) oss << ", ";
        }
        throw std::invalid_argument(oss.str());
    }
    return mask;
}
