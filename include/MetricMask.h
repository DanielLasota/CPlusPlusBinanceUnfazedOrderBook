#pragma once

#include <bitset>
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

static_assert(METRICS_COUNT > 0, "No metrics defined");

enum Metric : size_t {
    #define METRIC(name, ctype) name = METRIC_INDEX_##name,
    #include "detail/metrics_list.def"
    #undef METRIC
};

using MetricMask = std::bitset<METRICS_COUNT>;

inline const std::array<std::string_view, METRICS_COUNT>& allMetricNames() {
    static constexpr std::array<std::string_view, METRICS_COUNT> kNames = {
        #define METRIC(name, ctype) #name,
        #include "detail/metrics_list.def"
        #undef METRIC
    };
    return kNames;
}

[[nodiscard]] inline constexpr bool has(MetricMask const& mask, Metric m) noexcept {
    return mask.test(static_cast<size_t>(m));
}
inline constexpr void set(MetricMask& mask, Metric m) noexcept {
    mask.set(static_cast<size_t>(m));
}
inline constexpr void clear(MetricMask& mask, Metric m) noexcept {
    mask.reset(static_cast<size_t>(m));
}

[[nodiscard]] inline constexpr bool operator&(MetricMask const& mask, Metric m) noexcept {
    return has(mask, m);
}

[[nodiscard]] inline std::vector<std::string_view> expandMetricNames(const MetricMask& mask) {
    std::vector<std::string_view> out;
    out.reserve(mask.count());
    const auto& names = allMetricNames();
    for (size_t bit = 0; bit < names.size(); ++bit) {
        if (mask.test(bit)) out.push_back(names[bit]);
    }
    return out;
}

[[nodiscard]] inline constexpr MetricMask makeMask(std::initializer_list<Metric> ms) noexcept {
    MetricMask m;
    m.reset();
    for (Metric x : ms) m.set(static_cast<size_t>(x));
    return m;
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

[[nodiscard]] inline MetricMask parseMask(const std::vector<std::string>& vars) {
    const auto& lut = detail::lutNameToMetric();
    MetricMask mask;
    mask.reset();
    std::vector<std::string> unknowns;
    unknowns.reserve(4);

    for (const auto& s : vars) {
        if (auto it = lut.find(std::string_view{s}); it != lut.end()) {
            mask.set(static_cast<size_t>(it->second));
        } else {
            unknowns.push_back(s);
        }
    }

    if (!unknowns.empty()) {
        std::ostringstream oss;
        oss << "Unknown variable name" << (unknowns.size() > 1 ? "s" : "") << ": ";
        for (size_t i = 0; i < unknowns.size(); ++i) {
            if (i) oss << ", ";
            oss << unknowns[i];
        }
        throw std::invalid_argument(oss.str());
    }
    return mask;
}
