#pragma once


struct MetricsEntry {
    #define METRIC(name, ctype) ctype name;
    #include "detail/metrics_list.def"
    #undef METRIC
};
