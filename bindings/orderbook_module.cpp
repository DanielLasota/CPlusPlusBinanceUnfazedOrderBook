#include <iomanip>
#include <iostream>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "FeatureCalculator.h"
#include "GlobalMarketState.h"
#include "MarketState.h"
#include "MetricsCalculator.h"
#include "MetricsEntry.h"
#include "OrderBook.h"
#include "Simulator.h"
#include "enums/DifferenceDepthEntry.h"
#include "enums/Market.h"
#include "enums/TradeEntry.h"

namespace py = pybind11;
using MS = MarketState;

static py::int_ parse_mask_py(const std::vector<std::string>& vars)
{
    const MetricMask m = parseMask(vars);
    std::string bits = m.to_string();
    const auto pos = bits.find('1');
    const std::string trimmed = (pos == std::string::npos) ? "0" : bits.substr(pos);

    const py::object pyint = py::module::import("builtins").attr("int")(trimmed, 2);
    return pyint.cast<py::int_>();
}

PYBIND11_MODULE(cpp_binance_orderbook, m) {

        m.def(
            "compute",
            &Simulator::compute,
            py::arg("csv_path"),
            py::arg("features"));
        m.def(
            "backtest",
            &Simulator::backtest,
            py::arg("csv_path"),
            py::arg("features"),
            py::arg("callback") = py::none()
        );
        m.def(
            "compute_final_ob",
            &Simulator::computeFinalOB,
            py::arg("csv_path")
        );

    py::class_<GlobalMarketState>(m, "GlobalMarketState")
        .def(
            py::init<MetricMask>(),
            py::arg("mask")
        )
        .def(
            py::init<const std::vector<std::string>&>(),
            py::arg("features")
        )
        .def(
            "update",
            &GlobalMarketState::update,
            py::arg("entry")
        )
        .def(
            "calc_metrics_entry",
            &GlobalMarketState::calcMetricsEntry,
            py::arg("entry")
        )
        .def(
            "calc_metrics_entry_from_ap",
            &GlobalMarketState::calcMetricsEntryFromAp,
            py::arg("symbol"),
            py::arg("market")
        )
        .def(
            "get_market_state",
            &GlobalMarketState::getMarketState,
            py::arg("symbol"),
            py::arg("market"),
            py::return_value_policy::reference_internal
        )
        .def(
            "get_market_state_count",
            &GlobalMarketState::getMarketStateCount
        )
        .def(
            "__len__",
            &GlobalMarketState::getMarketStateCount
        )
        .def(
            "get_market_state_list",
            &GlobalMarketState::getMarketStateList
        );

    py::class_<MS>(m, "MarketState")
        .def(py::init<>())
        .def_readonly(
            "order_book",
            &MS::orderBook,
            py::return_value_policy::reference_internal
        )
        .def_readonly(
            "trade_roll",
            &MS::tradeRoll,
            py::return_value_policy::reference_internal
        )
        .def_readonly(
            "depth_roll",
            &MS::depthRoll,
            py::return_value_policy::reference_internal
        )
        .def("update",
             &MS::update,
             py::arg("entry")
        )
        .def(
            "update_orderbook",
            &MS::updateOrderBook,
            py::call_guard<py::gil_scoped_release>(),
            py::arg("timestamp_of_receive"),
            py::arg("price"),
            py::arg("quantity"),
            py::arg("is_ask")
        )
        .def("update_trade_registry",
             &MS::updateTradeRegistry,
             py::call_guard<py::gil_scoped_release>(),
             py::arg("timestamp_of_receive"),
             py::arg("price"),
             py::arg("quantity"),
             py::arg("is_buyer_market_maker")
        )
        .def_property_readonly(
            "has_last_trade",
            &MS::getHasLastTrade)
        .def_property_readonly(
            "last_trade",
            &MS::getLastTrade,
            py::return_value_policy::reference_internal
        )
        .def_property_readonly(
            "last_timestamp_of_receive",
            &MS::getLastTimestampOfReceive
        )
        .def("print_order_book",
             [](MS& self) { self.orderBook.printOrderBook(); }
        )
        .def_property_readonly(
            "symbol",
            &MS::getSymbol
        )
        .def_property_readonly(
            "market",
            &MS::getMarket
        );

    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("print_order_book", &OrderBook::printOrderBook)
        .def("asks", &OrderBook::getAsks)
        .def("bids", &OrderBook::getBids)
        .def("ask_count", &OrderBook::askCount)
        .def("bid_count", &OrderBook::bidCount)
        .def("sum_ask_quantity", &OrderBook::sumAskQuantity)
        .def("sum_bid_quantity", &OrderBook::sumBidQuantity)
        .def("best_ask_price", &OrderBook::bestAskPrice)
        .def("best_bid_price", &OrderBook::bestBidPrice)
        .def("best_ask_quantity", &OrderBook::bestAskQuantity)
        .def("best_bid_quantity", &OrderBook::bestBidQuantity)
        .def("second_ask_price", &OrderBook::secondAskPrice)
        .def("second_bid_price", &OrderBook::secondBidPrice)

        .def("cumulative_quantity_of_top_n_asks", &OrderBook::cumulativeQuantityOfTopNAsks)
        .def("cumulative_quantity_of_top_n_bids", &OrderBook::cumulativeQuantityOfTopNBids)
        .def("sum_of_price_times_quantity", &OrderBook::sumOfPriceTimesQuantity)
        .def("best_nth_ask_price", &OrderBook::bestNthAskPrice)
        .def("best_nth_bid_price", &OrderBook::bestNthBidPrice)

        .def("sum_total_ask_bid_quantity", &OrderBook::sumTotalAskBidQuantity)

        .def("delta_ask_count", &OrderBook::deltaAskCount)
        .def("delta_bid_count", &OrderBook::deltaBidCount)
        .def("delta_best_ask_quantity", &OrderBook::deltaBestAskQuantity)
        .def("delta_best_bid_quantity", &OrderBook::deltaBestBidQuantity)
        .def("delta_best_ask_price", &OrderBook::deltaBestAskPrice)
        .def("delta_best_bid_price", &OrderBook::deltaBestBidPrice)
        .def("delta_sum_ask_quantity", &OrderBook::deltaSumAskQuantity)
        .def("delta_sum_bid_quantity", &OrderBook::deltaSumBidQuantity)

        .def("update", &OrderBook::update, py::arg("entry")
    );


    auto fc = m.def_submodule("feature_calculator");
    fc.def(
        "calculate_best_ask_price",
        &FeatureCalculator::calculateBestAskPrice,
        py::arg("order_book")
    );
    fc.def(
        "calculate_best_bid_price",
        &FeatureCalculator::calculateBestBidPrice,
        py::arg("order_book")
    );
    fc.def(
        "calculate_mid_price",
        &FeatureCalculator::calculateMidPrice,
        py::arg("order_book"));
    fc.def(
        "calculate_best_volume_imbalance",
        &FeatureCalculator::calculateBestVolumeImbalance,
        py::arg("order_book")
    );
    fc.def(
        "calculate_best_n_price_levels_volume_imbalance",
        &FeatureCalculator::calculateBestNPriceLevelsVolumeImbalance,
        py::arg("order_book"),
        py::arg("nPriceLevels")
    );
    fc.def(
        "calculate_volume_imbalance",
        &FeatureCalculator::calculateVolumeImbalance,
        py::arg("order_book")
    );
    fc.def(
        "calculate_queue_imbalance",
        &FeatureCalculator::calculateQueueImbalance,
        py::arg("order_book")
    );
    fc.def(
        "calculate_gap",
        &FeatureCalculator::calculateGap,
        py::arg("order_book")
    );
    fc.def(
        "calculate_vwap_deviation",
        &FeatureCalculator::calculateVwapDeviation,
        py::arg("order_book")
    );
    fc.def(
        "calculate_is_aggressor_ask",
        [](const TradeEntry& t) { return FeatureCalculator::calculateIsAggressorAsk(&t); },
        py::arg("trade_entry"));
    fc.def(
        "calculate_simplified_slope_imbalance",
        &FeatureCalculator::calculateSimplifiedSlopeImbalance,
        py::arg("order_book")
    );
    fc.def(
        "calculate_trade_count_imbalance",
        &FeatureCalculator::calculateTradeCountImbalance,
        py::arg("trade_roll"),
        py::arg("windowTimeSeconds")
    );
    fc.def(
        "calculate_trade_volume_diff",
        &FeatureCalculator::calculateTradeVolumeDiff,
        py::arg("trade_roll"),
        py::arg("windowTimeSeconds"));
    fc.def(
        "calculate_price_difference",
        &FeatureCalculator::calculatePriceDifference,
        py::arg("trade_roll"),
        py::arg("windowTimeSeconds")
    );
    fc.def(
        "calculate_rate_of_return",
        &FeatureCalculator::calculateRateOfReturn,
        py::arg("trade_roll"),
        py::arg("windowTimeSeconds")
    );
    fc.def(
        "calculate_difference_depth_count_imbalance",
        &FeatureCalculator::calculateDepthCountImbalance,
        py::arg("trade_roll"),
        py::arg("windowTimeSeconds")
    );

    py::class_<DifferenceDepthEntry>(m, "DifferenceDepthEntry")
        .def(py::init<>())
        .def(
            py::init<uint64_t, Symbol, bool, double, double, bool, Market>(),
            py::arg("timestamp_of_receive"),
            py::arg("symbol"),
            py::arg("is_ask"),
            py::arg("price"),
            py::arg("quantity"),
            py::arg("is_last"),
            py::arg("market")
        )
        .def_readwrite("timestamp_of_receive", &DifferenceDepthEntry::timestampOfReceive)
        .def_readwrite("symbol", &DifferenceDepthEntry::symbol)
        .def_readwrite("is_ask", &DifferenceDepthEntry::isAsk)
        .def_readwrite("price", &DifferenceDepthEntry::price)
        .def_readwrite("quantity", &DifferenceDepthEntry::quantity)
        .def_readwrite("is_last", &DifferenceDepthEntry::isLast)
        .def_readwrite("market", &DifferenceDepthEntry::market)
        .def("__str__", [](const DifferenceDepthEntry& entry)
        {
            std::ostringstream oss;
            oss
                << "TimestampOfReceive: " << entry.timestampOfReceive << " "
                << "Symbol: " << entry.symbol << " "
                << "IsAsk: " << entry.isAsk << " "
                << "Price: " << entry.price << " "
                << "Quantity: " << entry.quantity << " "
                << "IsLast: " << entry.isLast << " "
                << "Market_: " << entry.market << " ";
            return oss.str();
        })
        .def("to_list", [](const DifferenceDepthEntry& e)
        {
            py::list v;
            v.append(e.timestampOfReceive);
            v.append(e.symbol);
            v.append(e.isAsk ? 1 : 0);
            v.append(e.price);
            v.append(e.quantity);
            v.append(e.isLast ? 1 : 0);
            v.append(e.market);
            return v;
        })
        .def_static("field_names", []()
        {
            return std::vector<std::string>{
                "timestamp_of_receive",
                "symbol",
                "is_ask",
                "price",
                "quantity",
                "is_last",
                "market"
            };
        });

    py::class_<TradeEntry>(m, "TradeEntry")
        .def(py::init<>())
        .def(
            py::init<uint64_t, Symbol, double, double, bool, bool, Market>(),
            py::arg("timestamp_of_receive"),
            py::arg("symbol"),
            py::arg("price"),
            py::arg("quantity"),
            py::arg("is_buyer_market_maker"),
            py::arg("is_last"),
            py::arg("market")
        )
        .def_readwrite("timestamp_of_receive", &TradeEntry::timestampOfReceive)
        .def_readwrite("symbol", &TradeEntry::symbol)
        .def_readwrite("price", &TradeEntry::price)
        .def_readwrite("quantity", &TradeEntry::quantity)
        .def_readwrite("is_buyer_market_maker", &TradeEntry::isBuyerMarketMaker)
        .def_readwrite("is_last", &TradeEntry::isLast)
        .def_readwrite("market", &TradeEntry::market)
        .def("__str__", [](const TradeEntry& entry)
        {
            std::ostringstream oss;
            oss
                << "TimestampOfReceive: " << entry.timestampOfReceive << " "
                << "Symbol: " << entry.symbol << " "
                << "Price: " << entry.price << " "
                << "Quantity: " << entry.quantity << " "
                << "IsBuyerMarketMaker: " << entry.isBuyerMarketMaker << " "
                << "IsLast: " << entry.isLast << " "
                << "Market_: " << entry.market << " ";
            return oss.str();
        })
        .def("to_list", [](const TradeEntry& e)
        {
            py::list v;
            v.append(e.timestampOfReceive);
            v.append(e.symbol);
            v.append(e.price);
            v.append(e.quantity);
            v.append(e.isBuyerMarketMaker ? 1 : 0);
            v.append(e.isLast ? 1 : 0);
            v.append(e.market);
            return v;
        })
        .def_static("field_names", []()
        {
            return std::vector<std::string>{
                "timestamp_of_receive",
                "symbol",
                "price",
                "quantity",
                "is_buyer_market_maker",
                "is_last",
                "market"
            };
        });

    py::class_<TradeRoll>(m, "TradeRoll")
        .def(py::init<>());

    py::class_<DepthRoll>(m, "DepthRoll")
        .def(py::init<>());

    py::class_<MetricsEntry>(m, "MetricsEntry")

#define METRIC(name, ctype) .def_readonly(#name, &MetricsEntry::name)
#include "detail/metrics_list.def"


#undef METRIC
        .def("__str__", [](const MetricsEntry& entry)
             {
                 std::ostringstream oss;
                 auto append_field = [&](auto const& field, const char* nm)
                 {
                     using T = std::decay_t<decltype(field)>;
                     oss << nm << ": ";
                     if constexpr (std::is_same_v<T, bool>) oss << (field ? 1 : 0);
                     else if constexpr (std::is_same_v<T, uint8_t>) oss << static_cast<int>(field);
                     else oss << field;
                     oss << " ";
                 };
                #define METRIC(name, ctype) append_field(entry.name, #name);
                #include "detail/metrics_list.def"
                #undef METRIC
                return oss.str();
             }
        );

    m.def("parse_mask", &parse_mask_py, py::arg("variables"));

    py::class_<MetricsCalculator>(m, "MetricsCalculator")
        .def(py::init<MetricMask>(), py::arg("mask"))
        .def(py::init<const std::vector<std::string>&>(), py::arg("variables"))
        .def(
            "calc",
            &MetricsCalculator::calc,
            py::arg("market_state")
        );

    py::enum_<Market>(m, "Market")
        .value("UNKNOWN", Market::UNKNOWN)
        .value("SPOT", Market::SPOT)
        .value("USD_M_FUTURES", Market::USD_M_FUTURES)
        .value("COIN_M_FUTURES", Market::COIN_M_FUTURES)
        .export_values();

    py::enum_<Symbol>(m, "Symbol")
        .value("UNKNOWN", Symbol::UNKNOWN)
        .value("BTCUSDT", Symbol::BTCUSDT)
        .value("ETHUSDT", Symbol::ETHUSDT)
        .value("BNBUSDT", Symbol::BNBUSDT)
        .value("SOLUSDT", Symbol::SOLUSDT)
        .value("XRPUSDT", Symbol::XRPUSDT)
        .value("DOGEUSDT", Symbol::DOGEUSDT)
        .value("ADAUSDT", Symbol::ADAUSDT)
        .value("SHIBUSDT", Symbol::SHIBUSDT)
        .value("LTCUSDT", Symbol::LTCUSDT)
        .value("AVAXUSDT", Symbol::AVAXUSDT)
        .value("TRXUSDT", Symbol::TRXUSDT)
        .value("DOTUSDT", Symbol::DOTUSDT)
        .value("BCHUSDT", Symbol::BCHUSDT)
        .value("SUIUSDT", Symbol::SUIUSDT)
        .export_values();

    py::enum_<StreamType>(m, "StreamType")
        .value("DIFFERENCE_DEPTH_STREAM", StreamType::DIFFERENCE_DEPTH_STREAM)
        .value("TRADE_STREAM", StreamType::TRADE_STREAM)
        .value("DEPTH_SNAPSHOT", StreamType::DEPTH_SNAPSHOT)
        .value("FINAL_DEPTH_SNAPSHOT", StreamType::FINAL_DEPTH_SNAPSHOT)
        .export_values();
}
