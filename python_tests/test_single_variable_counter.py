import cpp_binance_orderbook
import pytest
from cpp_binance_orderbook import (
    OrderBook,
    MarketState,
    DifferenceDepthEntry,
    TradeEntry,
    Symbol,
    Market,
    single_variable_counter as svc
)

class TestSingleVariableCounter:
    @staticmethod
    def get_sample_order_book(
            symbol: cpp_binance_orderbook.Symbol,
            market: cpp_binance_orderbook.Market,
            price_hash: float,
            quantity_hash: float
    ) -> cpp_binance_orderbook.OrderBook:

        market_state = cpp_binance_orderbook.MarketState()

        expected_difference_depth_entries_data_list = [
            (1750489200_000_001, 1, round((price_hash + 2.1), 5), quantity_hash + 1),    # best ask
            (1750489200_000_002, 1, round((price_hash + 2.5), 5), quantity_hash + 2),
            (1750489200_000_003, 1, round((price_hash + 2.3), 5), quantity_hash + 2),    #           2nd best
            (1750489200_000_004, 1, round((price_hash + 3.1), 5), quantity_hash + 2),
            (1750489200_000_005, 1, round((price_hash + 4.1), 5), quantity_hash + 2),

            (1750489200_000_006,  0, round((price_hash + 1.0), 5), quantity_hash + 3),
            (1750489200_000_007,  0, round((price_hash + 2.0), 5), quantity_hash + 8),    # best bid
            (1750489200_000_008,  0, round((price_hash + 0.0), 5), quantity_hash + 2),
            (1750489200_000_009,  0, round((price_hash + 1.5), 5), quantity_hash + 7),    #           2nd best
            (1750489200_000_010,  0, round((price_hash + 1.1), 5), quantity_hash + 9),
            (1750489200_000_011,  0, round((price_hash + 1.2), 5), quantity_hash + 8),
        ]

        for (timestamp_of_receive, is_ask, price, quantity) in expected_difference_depth_entries_data_list:
            market_state.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=timestamp_of_receive,
                    symbol=symbol,
                    is_ask=is_ask,
                    price=price,
                    quantity=quantity,
                    is_last=1,
                    market=market
                )
            )

        expected_trade_entries_data_list = [
            (1750489200_000_011, round((price_hash + 2.1), 5), quantity_hash + 3, True),   # - 5.1799
            (1750489200_000_012, round((price_hash + 2.4), 5), quantity_hash + 4, False),  # + 6.1799
            (1750489200_000_013, round((price_hash + 2.5), 5), quantity_hash + 2, False),  # + 4.1799
            (1750489200_000_014, round((price_hash + 2.6), 5), quantity_hash + 3, False),  # + 5.1799
            (1750489200_000_015, round((price_hash + 2.7), 5), quantity_hash + 4, True),   # - 6.1799
            (1750489200_000_016, round((price_hash + 2.8), 5), quantity_hash + 2, False),  # + 4.1799
            (1750489200_000_017, round((price_hash + 2.9), 5), quantity_hash + 3, True),   # - 5.1799
            (1750489200_000_018, round((price_hash + 3.0), 5), quantity_hash + 4, False),  # + 6.1799
            (1750489200_000_019, round((price_hash + 3.9), 5), quantity_hash + 2, True),   # - 4.1799
        ]

        for (timestamp_of_receive, price, quantity, is_buyer_market_maker) in expected_trade_entries_data_list:
            market_state.update(
                TradeEntry(
                    timestamp_of_receive=timestamp_of_receive,
                    symbol=symbol,
                    price=price,
                    quantity=quantity,
                    is_buyer_market_maker=is_buyer_market_maker,
                    m_unknown_parameter='False',
                    x_unknown_parameter='False',
                    is_last=1,
                    market=market
                )
            )

        return market_state

    def test_calculate_best_ask_price(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        assert svc.calculate_best_ask_price(market_state.order_book) == 5.1

    def test_calculate_best_bid_price(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        assert svc.calculate_best_bid_price(market_state.order_book) == 5.0

    def test_calculate_mid_price(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        assert svc.calculate_mid_price(market_state.order_book) == 5.05


    def test_calculate_best_volume_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (10.1799 - 3.1799)/(10.1799 + 3.1799) = 0.53846153846 ~= 0.54
        assert svc.calculate_best_volume_imbalance(market_state.order_book) == 0.52

    def test_calculate_best_volume_ratio(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # 10.1799 / 3.1799 = 3.2013270857574136
        assert svc.calculate_best_depth_volume_ratio(market_state.order_book) == 3.20

    def test_best_n_price_levels_volume_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        # 19.3598 - 7.3598 / 19.3598 + 7.3598 = 12 / 26.7196 ~= 0.45
        assert svc.calculate_best_n_price_levels_volume_imbalance(market_state.order_book, 2) == 0.45

        # 29.5397 - 11.5397 / 29.5397 + 11.5397 = 18 / 41.0794 ~= 0.44
        assert svc.calculate_best_n_price_levels_volume_imbalance(market_state.order_book, 3) == 0.44

        # 45.8995 - 19.8995 / 45.8995 + 19.8995 = 26 / 65.7990 ~= 0.40
        assert svc.calculate_best_n_price_levels_volume_imbalance(market_state.order_book, 5) == 0.40

    def test_calculate_volume_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (50.0794 - 19.8995) / (50.0794 + 19.8995)
        assert svc.calculate_volume_imbalance(market_state.order_book) == 0.43

    def test_calculate_queue_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (6-5)/(6+5) = 1/11 ~= 0.09
        assert svc.calculate_queue_imbalance(market_state.order_book) == 0.09

    def test_calculate_gap(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (second_bid + second_ask) - (best_bid + best_ask)
        # = (4.5 + 5.3) - (5.0 + 5.1) = 9.8 - 10.1 = -0.3
        assert svc.calculate_gap(market_state.order_book) == -0.3

    def test_calculate_is_aggressor_ask(self):
        t = TradeEntry()
        t.is_buyer_market_maker = True
        assert svc.calculate_is_aggressor_ask(t) is True
        t.is_buyer_market_maker = False
        assert svc.calculate_is_aggressor_ask(t) is False

    def test_calculate_vwap_deviation(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # Aski:
        # 5.1 × 3.1799 + 5.3 × 4.1799 + 5.5 × 4.1799 + 6.1 × 4.1799 + 7.1 × 4.1799
        # = 16.21749 + 22.18347 + 23.98945 + 25.49739 + 29.70829
        # = 117.59609

        # Bidy:
        # 5.0 × 10.1799 + 4.5 × 9.1799 + 4.2 × 10.1799 + 4.1 × 11.1799 + 4.0 × 5.1799 + 3.0 × 4.1799
        # = 50.8995 + 41.30955 + 42.75558 + 45.82759 + 20.7196 + 12.5397
        # = 213.00052

        # Razem: 117.59609 + 213.00052 = 330.59661
        assert svc.calculate_vwap_deviation(market_state.order_book) == -0.06450837

    def test_calculate_simplified_slope_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        # bestFiveBidsQuantityCumulativeSum / (midPrice - bestFifthBidPrice);
        # bestFiveAsksQuantityCumulativeSum / (bestFifthAskPrice - midPrice);
        # (bidSlope - askSlope) / (bidSlope + askSlope)
        # 43.7138 - 9.7076 / 43.7138 + 9.7076 = 34.0062 / 53.4214 ~= 0.64
        assert svc.calculate_simplified_slope_imbalance(market_state.order_book) == 0.64


    def test_calculate_trade_count_imbalance_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # 5-4/5+4 = 1/9 = 0.11
        assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 1) == 0.11
        # assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 3) == 0.11
        # assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 5) == 0.11
        # assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 10) == 0.11
        # assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 15) == 0.11
        # assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 30) == 0.11
        # assert svc.calculate_trade_count_imbalance(market_state.rolling_statistics_data, 60) == 0.11

    def test_calculate_cumulative_delta_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_cumulative_delta(market_state.rolling_statistics_data, 1) == 5.1799

    def test_calculate_price_difference_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_price_difference(market_state.rolling_statistics_data, 1) == 1.8

    def test_calculate_rate_of_return_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # 1.8/5.1
        assert svc.calculate_rate_of_return(market_state.rolling_statistics_data, 1) == 35.29

    def test_calculate_difference_depth_volatility_imbalance_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        # 6-5/11
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_statistics_data, 1) == 0.09


    def test_calculate_rsi_5_seconds(self):
        ...

    def test_calculate_stoch_rsi_2_seconds(self):
        ...

    def test_calculate_macd_2_seconds(self):
        ...

