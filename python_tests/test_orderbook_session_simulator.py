import sys, os
from pathlib import Path

import cpp_binance_orderbook
import pandas as pd
here = Path(__file__).resolve().parent
project_root = here.parent
build_dir = project_root / "cmake-build-release"
dll_dir   = project_root
os.add_dll_directory(str(build_dir))
os.add_dll_directory(str(dll_dir))
sys.path.insert(0, str(build_dir))
import pytest


"""
MarketState.cpp:
...
    std::optional<OrderBookMetricsEntry> MarketState::countOrderBookMetrics(MetricMask mask) const {

        if (orderBook.bids.size() < 2 || orderBook.asks.size() < 2 || !hasLastTrade)
            return std::nullopt;
...

!hasLastTrade - test_positive_binance_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_24 has 2 depth entries before receiving an trade entry.
Hence, variables_df will differ by 2, we must consider doing sheer_df['IsLast'].sum() - 2
"""


ALL_ORDERBOOK_VARIABLES = [
    "timestampOfReceive",
    "market",
    "symbol",
    "bestAskPrice",
    "bestBidPrice",
    "midPrice",

    "bestVolumeImbalance",
    "bestTwoVolumeImbalance",
    "bestThreeVolumeImbalance",
    "bestFiveVolumeImbalance",
    "queueImbalance",
    "volumeImbalance",
    "gap",
    "isAggressorAsk",
    "vwapDeviation",
    "simplifiedSlopeImbalance",

    "tradeCountImbalance1Seconds",
    "tradeCountImbalance3Seconds",
    "tradeCountImbalance5Seconds",
    "tradeCountImbalance10Seconds",
    "tradeCountImbalance15Seconds",
    "tradeCountImbalance30Seconds",
    "tradeCountImbalance60Seconds",

    "cumulativeDelta1Seconds",
    "cumulativeDelta3Seconds",
    "cumulativeDelta5Seconds",
    "cumulativeDelta10Seconds",
    "cumulativeDelta15Seconds",
    "cumulativeDelta30Seconds",
    "cumulativeDelta60Seconds",

    "priceDifference1Seconds",
    "priceDifference3Seconds",
    "priceDifference5Seconds",
    "priceDifference10Seconds",
    "priceDifference15Seconds",
    "priceDifference30Seconds",
    "priceDifference60Seconds",

    "rateOfReturn1Seconds",
    "rateOfReturn3Seconds",
    "rateOfReturn5Seconds",
    "rateOfReturn10Seconds",
    "rateOfReturn15Seconds",
    "rateOfReturn30Seconds",
    "rateOfReturn60Seconds",

    "differenceDepthVolatilityImbalance1Seconds",
    "differenceDepthVolatilityImbalance3Seconds",
    "differenceDepthVolatilityImbalance5Seconds",
    "differenceDepthVolatilityImbalance10Seconds",
    "differenceDepthVolatilityImbalance15Seconds",
    "differenceDepthVolatilityImbalance30Seconds",
    "differenceDepthVolatilityImbalance60Seconds",

    "rsi5Seconds",
    "stochRsi5Seconds",

    "macd2Seconds"
]


class TestOrderBookSessionSimulator:

    def test_init(self):
        ...

    class TestOrderBookSessionSimulatorComputeVariables:

        def test_given_single_pair_merged_csv_when_passing_bad_variable_name_then_exception_is_raised(self):
            import cpp_binance_orderbook

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

            bad_variables_list = ['bestBidPrice', 'bestAskPrice', 'midPrice', 'gap', 'crap']

            with pytest.raises(ValueError) as excinfo:
                metrics = oss.compute_variables(
                    csv_path=csv_path,
                    variables=bad_variables_list
                )
            assert str(excinfo.value) == "Unknown variable name: crap"

        def test_given_single_pair_merged_csv_when_returning_variables_list_then_len_is_equal_to_is_last_column_sum(self):

            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            metrics = oss.compute_variables(
                csv_path=csv_path,
                variables=ALL_ORDERBOOK_VARIABLES
            )
            metrics_df = pd.DataFrame([
                {var: getattr(entry, var) for var in ALL_ORDERBOOK_VARIABLES}
                for entry in metrics
            ])

            sheer_df = pd.read_csv(csv_path, comment='#')

            assert len(metrics_df) == sheer_df['IsLast'].sum() - 2
            assert metrics_df.shape[0] == sheer_df['IsLast'].sum() - 2
            assert (metrics_df['timestampOfReceive'] == sheer_df[sheer_df['IsLast'] == 1]['TimestampOfReceiveUS'].iloc[2:].reset_index(drop=True)).all()

        def test_given_single_pair_merged_csv_is_returned_variables_list_of_selected_variables(self):
            import cpp_binance_orderbook

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            metrics = oss.compute_variables(
                "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv",
                variables=ALL_ORDERBOOK_VARIABLES
            )
            df = pd.DataFrame([
                {var: getattr(entry, var) for var in ALL_ORDERBOOK_VARIABLES}
                for entry in metrics
            ])

            assert df.columns.tolist() == ALL_ORDERBOOK_VARIABLES
            for col in df.columns:
                assert not df[col].isnull().all(), f"Column `{col}` contains only NaN values"

    class TestOrderBookSessionSimulatorComputeBacktest:

        def test_given_single_pair_merged_csv_when_passing_bad_variable_name_then_exception_is_raised(self):
            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            bad_variables_list = ['bestBidPrice', 'bestAskPrice', 'midPrice', 'gap', 'crap']

            with pytest.raises(ValueError) as excinfo:
                metrics = oss.compute_backtest(
                    csv_path=csv_path,
                    variables=bad_variables_list
                )
            assert str(excinfo.value) == "Unknown variable name: crap"

        def test_given_single_pair_merged_csv_when_python_callback_then_count_of_callbacks_is_equal_to_is_last_column_sum(self):

            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"
            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            callback_entries_list = []

            def python_callback(orderbook_entry_metrics: cpp_binance_orderbook.OrderBookMetricsEntry):
                record = {
                    var: getattr(orderbook_entry_metrics, var)
                    for var in ALL_ORDERBOOK_VARIABLES
                }
                callback_entries_list.append(record)

            oss.compute_backtest(
                csv_path=csv_path,
                variables=ALL_ORDERBOOK_VARIABLES,
                python_callback=python_callback
            )
            metrics_df = pd.DataFrame(callback_entries_list)

            sheer_df = pd.read_csv(csv_path, comment='#')

            assert len(metrics_df) == sheer_df['IsLast'].sum() - 2
            assert metrics_df.shape[0] == sheer_df['IsLast'].sum() - 2
            assert (metrics_df['timestampOfReceive'] == sheer_df[sheer_df['IsLast'] == 1]['TimestampOfReceiveUS'].iloc[2:].reset_index(drop=True)).all()

        def test_given_single_pair_merged_csv_when_python_callback_then_callback_variables_sum_is_equal_to_selected_variables(self):
            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"
            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            callback_entries_list = []

            def python_callback(orderbook_entry_metrics: cpp_binance_orderbook.OrderBookMetricsEntry):
                record = {
                    var: getattr(orderbook_entry_metrics, var)
                    for var in ALL_ORDERBOOK_VARIABLES
                }
                callback_entries_list.append(record)

            oss.compute_backtest(
                csv_path=csv_path,
                variables=ALL_ORDERBOOK_VARIABLES,
                python_callback=python_callback
            )

            df = pd.DataFrame(callback_entries_list)

            assert df.columns.tolist() == ALL_ORDERBOOK_VARIABLES
            for col in df.columns:
                assert not df[col].isnull().all(), f"Column `{col}` contains only NaN values"

    class TestOrderBookSessionSimulatorComputeFinalDepthSnapshot:
        def test_given_root_difference_depth_csv_when_computing_final_snapshot_then_snapshot_is_equal_to_next_day_original_binance_depth_snapshot(self):
            from cpp_binance_orderbook import OrderBookSessionSimulator
            difference_depth_csv = 'csv/test_positive_binance_difference_depth_stream_usd_m_futures_trxusdt_09-04-2025_3_cols_till_next_day_firsts.csv'
            oss = OrderBookSessionSimulator()

            final_depth_snapshot_from_orderbook_after_09_04: cpp_binance_orderbook.OrderBook = oss.compute_final_depth_snapshot(csv_path=difference_depth_csv)
            final_depth_snapshot_from_cpp_orderbook_after_09_04_df = pd.DataFrame([
                {var: getattr(entry, var) for var in ['is_ask', 'price', 'quantity']}
                for entry in final_depth_snapshot_from_orderbook_after_09_04.bids()[:1000] + final_depth_snapshot_from_orderbook_after_09_04.asks()[:1000]
            ])
            final_depth_snapshot_from_cpp_orderbook_after_09_04_df.rename(columns={'is_ask': 'IsAsk', 'price': 'Price', 'quantity': 'Quantity'}, inplace=True)

            first_depth_snapshot_root_csv_10_04_path = 'csv/test_positive_binance_depth_snapshot_usd_m_futures_trxusdt_10-04-2025_first_snapshot.csv'
            first_depth_snapshot_root_csv_10_04 = pd.read_csv(first_depth_snapshot_root_csv_10_04_path, comment='#', dtype={'IsAsk': bool, 'Price': float, 'Quantity': float})

            pd.testing.assert_frame_equal(
                final_depth_snapshot_from_cpp_orderbook_after_09_04_df[['IsAsk','Price','Quantity']].reset_index(drop=True),
                first_depth_snapshot_root_csv_10_04[['IsAsk','Price','Quantity']].reset_index(drop=True)
            )

            records1 = final_depth_snapshot_from_cpp_orderbook_after_09_04_df[['IsAsk','Price','Quantity']].reset_index(drop=True).to_dict('records')
            records2 = first_depth_snapshot_root_csv_10_04[['IsAsk','Price','Quantity']].reset_index(drop=True).to_dict('records')
            assert records1 == records2

        def test_given_root_difference_depth_csv_when_computing_final_snapshot_then_price_levels_are_sorted_correctly(self):
            from cpp_binance_orderbook import OrderBookSessionSimulator
            difference_depth_csv = 'csv/test_positive_binance_difference_depth_stream_usd_m_futures_trxusdt_09-04-2025_3_cols_till_next_day_firsts.csv'
            oss = OrderBookSessionSimulator()

            final_depth_snapshot_after_09_04: cpp_binance_orderbook.OrderBook = oss.compute_final_depth_snapshot(csv_path=difference_depth_csv)

            bid_prices = [entry.price for entry in final_depth_snapshot_after_09_04.bids()]
            ask_prices = [entry.price for entry in final_depth_snapshot_after_09_04.asks()]

            assert bid_prices == sorted(bid_prices, reverse=True), "Bids powinny być posortowane malejąco"
            assert ask_prices == sorted(ask_prices), "Asks powinny być posortowane rosnąco"

            final_depth_snapshot_after_09_04_df = pd.DataFrame([
                {var: getattr(entry, var) for var in ['is_ask', 'price', 'quantity']}
                for entry in final_depth_snapshot_after_09_04.bids() + final_depth_snapshot_after_09_04.asks()
            ])

            pd.testing.assert_series_equal(
                final_depth_snapshot_after_09_04_df[final_depth_snapshot_after_09_04_df['is_ask'] == False]['price'].reset_index(drop=True),
                final_depth_snapshot_after_09_04_df[final_depth_snapshot_after_09_04_df['is_ask'] == False]['price'].sort_values(ascending=False).reset_index(drop=True),
                check_dtype=False,
                obj="Bids"
            )
            pd.testing.assert_series_equal(
                final_depth_snapshot_after_09_04_df[final_depth_snapshot_after_09_04_df['is_ask'] == True]['price'].reset_index(drop=True),
                final_depth_snapshot_after_09_04_df[final_depth_snapshot_after_09_04_df['is_ask'] == True]['price'].sort_values(ascending=True).reset_index(drop=True),
                check_dtype=False,
                obj="Asks"
            )

        def test_given_order_book_with_orders_when_compute_final_depth_snapshot_then_snapshot_contains_all_levels(self):
            from cpp_binance_orderbook import OrderBookSessionSimulator
            first_depth_snapshot_root_csv_10_04_path = 'csv/test_positive_binance_depth_snapshot_usd_m_futures_trxusdt_10-04-2025_first_snapshot.csv'
            oss = OrderBookSessionSimulator()
            order_book = oss.compute_final_depth_snapshot(first_depth_snapshot_root_csv_10_04_path)

            order_book_df = pd.DataFrame([
                {var: getattr(entry, var) for var in ['is_ask', 'price', 'quantity']}
                for entry in order_book.bids() + order_book.asks()
            ])

            assert order_book_df.shape[0] == 2000

    class TestOrderBookSessionSimulatorComputeVariablesNumPy:

        def test_given_single_pair_merged_csv_when_passing_bad_variable_name_then_exception_is_raised(self):
            import cpp_binance_orderbook

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

            bad_variables_list = ['bestBidPrice', 'bestAskPrice', 'midPrice', 'gap', 'crap']

            with pytest.raises(ValueError) as excinfo:
                metrics_dict = oss.compute_variables_numpy(
                    csv_path=csv_path,
                    variables=bad_variables_list
                )
            assert str(excinfo.value) == "Unknown variable name: crap"

        def test_given_single_pair_merged_csv_when_returning_variables_list_then_len_is_equal_to_is_last_column_sum(self):

            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            metrics_dict = oss.compute_variables_numpy(
                csv_path=csv_path,
                variables=ALL_ORDERBOOK_VARIABLES
            )
            metrics_df = pd.DataFrame(metrics_dict)

            sheer_df = pd.read_csv(csv_path, comment='#')

            assert len(metrics_df) == sheer_df['IsLast'].sum() - 2
            assert metrics_df.shape[0] == sheer_df['IsLast'].sum() - 2
            assert (metrics_df['timestampOfReceive'] == sheer_df[sheer_df['IsLast'] == 1]['TimestampOfReceiveUS'].iloc[2:].reset_index(drop=True)).all()

        def test_given_single_pair_merged_csv_is_returned_variables_list_of_selected_variables(self):
            import cpp_binance_orderbook

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            metrics_dict = oss.compute_variables_numpy(
                "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv",
                variables=ALL_ORDERBOOK_VARIABLES
            )
            df = pd.DataFrame(metrics_dict)

            assert df.columns.tolist() == ALL_ORDERBOOK_VARIABLES
            for col in df.columns:
                assert not df[col].isnull().all(), f"Column `{col}` contains only NaN values"

    class TestOrderBookSessionSimulatorComputeBacktestNumPy:

        def test_given_single_pair_merged_csv_when_passing_bad_variable_name_then_exception_is_raised(self):
            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            bad_variables_list = ['bestBidPrice', 'bestAskPrice', 'midPrice', 'gap', 'crap']

            with pytest.raises(ValueError) as excinfo:
                metrics = oss.compute_backtest_numpy(
                    csv_path=csv_path,
                    variables=bad_variables_list
                )
            assert str(excinfo.value) == "Unknown variable name: crap"

        def test_given_single_pair_merged_csv_when_python_callback_then_count_of_callbacks_is_equal_to_is_last_column_sum(self):

            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"
            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            callback_entries_list = []

            def python_callback(orderbook_entry_metrics: cpp_binance_orderbook.OrderBookMetricsEntry):
                record = {
                    var: getattr(orderbook_entry_metrics, var)
                    for var in ALL_ORDERBOOK_VARIABLES
                }
                callback_entries_list.append(record)

            oss.compute_backtest_numpy(
                csv_path=csv_path,
                variables=ALL_ORDERBOOK_VARIABLES,
                python_callback=python_callback
            )
            metrics_df = pd.DataFrame(callback_entries_list)

            sheer_df = pd.read_csv(csv_path, comment='#')

            assert len(metrics_df) == sheer_df['IsLast'].sum() - 2
            assert metrics_df.shape[0] == sheer_df['IsLast'].sum() - 2
            assert (metrics_df['timestampOfReceive'] == sheer_df[sheer_df['IsLast'] == 1]['TimestampOfReceiveUS'].iloc[2:].reset_index(drop=True)).all()

        def test_given_single_pair_merged_csv_when_python_callback_then_callback_variables_sum_is_equal_to_selected_variables(self):
            import cpp_binance_orderbook

            csv_path = "csv/test_positive_binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"
            oss = cpp_binance_orderbook.OrderBookSessionSimulator()

            callback_entries_list = []

            def python_callback(orderbook_entry_metrics: cpp_binance_orderbook.OrderBookMetricsEntry):
                record = {
                    var: getattr(orderbook_entry_metrics, var)
                    for var in ALL_ORDERBOOK_VARIABLES
                }
                callback_entries_list.append(record)

            oss.compute_backtest_numpy(
                csv_path=csv_path,
                variables=ALL_ORDERBOOK_VARIABLES,
                python_callback=python_callback
            )

            df = pd.DataFrame(callback_entries_list)

            assert df.columns.tolist() == ALL_ORDERBOOK_VARIABLES
            for col in df.columns:
                assert not df[col].isnull().all(), f"Column `{col}` contains only NaN values"

    class TestParseMask:

        def test_given_variables_list_when_parse_mask_then_accurate_bytes_are_returned(self):
            from cpp_binance_orderbook import parse_mask

            variables_list = [
                'timestampOfReceive',
                'bestAskPrice',
                'bestBidPrice',
                'midPrice',
                'bestVolumeImbalance',
                'queueImbalance',
                'volumeImbalance',
                'gap',
                'isAggressorAsk'
            ]

            mask = parse_mask(variables_list)

            # timestampOfReceive   # 1<<0   =       1
            # bestAskPrice         # 1<<3   =       8
            # bestBidPrice         # 1<<4   =      16
            # midPrice             # 1<<5   =      32
            # bestVolumeImbalance  # 1<<6   =      64
            # queueImbalance       # 1<<12  =    4096
            # volumeImbalance      # 1<<11  =    2048
            # gap                  # 1<<13  =    8192
            # isAggressorAsk       # 1<<14  =   16384
            expected_mask = 1 + 8 + 16 + 32 + 64 + 65536 + 131072 + 262144 + 524288

            assert mask == expected_mask

        def test_given_partial_variables_list_when_parse_mask_then_correct_mask_is_returned(self):
            from cpp_binance_orderbook import parse_mask

            variables_list = [
                'timestampOfReceive',
                'midPrice',
                'volumeImbalance'
            ]

            mask = parse_mask(variables_list)

            # timestampOfReceive = 1 << 0  =    1
            # midPrice           = 1 << 5  =   32
            # volumeImbalance    = 1 << 11 = 2048
            expected_mask = 1 + 32 + 65536

            assert mask == expected_mask

        def test_given_invalid_variable_name_then_exception_is_raised(self):
            import pytest
            from cpp_binance_orderbook import parse_mask

            with pytest.raises(ValueError) as excinfo:
                parse_mask(['crap'])
            assert "Unknown variable name: crap" in str(excinfo.value)
