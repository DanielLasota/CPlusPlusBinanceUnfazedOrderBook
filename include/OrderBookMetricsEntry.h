#pragma once


struct OrderBookMetricsEntry {
    #define METRIC(name, ctype) ctype name;
    #include "detail/metrics_list.def"
    #undef METRIC
};
