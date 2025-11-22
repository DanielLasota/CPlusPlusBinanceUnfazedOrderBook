import cpp_binance_orderbook

from cpp_binance_orderbook import (
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
            (1750489199_999_999, 1, round((price_hash + 2.1), 5), quantity_hash + 1),    # ask price 5.1 @quantity: 3.1799      # best ask

            (1750489200_000_000, 0, round((price_hash + 1.0), 5), quantity_hash + 3),    # bid price 4.0 @quantity: 5.1799
            (1750489229_999_999, 0, round((price_hash + 2.0), 5), quantity_hash + 8),    # bid price 5.0 @quantity: 10.1799     # best bid
            (1750489230_000_000, 0, round((price_hash + 1.5), 5), quantity_hash + 2),    # bid price 3.0 @quantity: 4.1799
            (1750489230_000_000, 0, round((price_hash + 0.0), 5), quantity_hash + 2),    # bid price 3.0 @quantity: 4.1799
            (1750489244_999_999, 0, round((price_hash + 1.5), 5), quantity_hash + 7),    # bid price 4.5 @quantity: 9.1799      # 2nd best bid
            (1750489249_999_999, 1, round((price_hash + 2.3), 5), quantity_hash + 2),    # ask price 5.3 @quantity: 4.1799      # 2nd best ask
            (1750489254_999_999, 1, round((price_hash + 2.5), 5), quantity_hash + 2),    # ask price 5.5 @quantity: 4.1799
            (1750489255_000_000, 1, round((price_hash + 3.1), 5), quantity_hash + 2),    # ask price 6.1 @quantity: 4.1799
            (1750489256_999_999, 0, round((price_hash + 1.1), 5), quantity_hash + 9),    # bid price 4.1 @quantity: 11.1799
            (1750489258_999_999, 1, round((price_hash + 4.1), 5), quantity_hash + 2),    # ask price 7.1 @quantity: 4.1799
            (1750489260_000_000, 0, round((price_hash + 1.2), 5), quantity_hash + 8),    # bid price 4.2 @quantity: 10.1799
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
            (1750489120_000_000, round((price_hash + 1.0), 5),  quantity_hash + 3, True),  # price 4.0 @quantity: - 5.1799 # < t-60s
            (1750489125_000_000, round((price_hash + 2.0), 5),  quantity_hash + 3, True),  # price 5.0 @quantity: - 5.1799 # < t-60s
            (1750489130_000_000, round((price_hash + 2.1), 5),  quantity_hash + 3, True),  # price 5.1 @quantity: - 5.1799 # < t-60s
            (1750489135_000_000, round((price_hash + 2.2), 5),  quantity_hash + 3, True),  # price 5.2 @quantity: - 5.1799 # < t-60s
            (1750489140_000_000, round((price_hash + 2.3), 5),  quantity_hash + 3, True),  # price 5.3 @quantity: - 5.1799 # < t-60s
            (1750489145_000_000, round((price_hash + 2.4), 5),  quantity_hash + 3, True),  # price 5.4 @quantity: - 5.1799 # < t-60s
            (1750489150_000_000, round((price_hash + 2.5), 5),  quantity_hash + 3, True),  # price 5.5 @quantity: - 5.1799 # < t-60s
            (1750489155_000_000, round((price_hash + 3.1), 5),  quantity_hash + 3, True),  # price 6.1 @quantity: - 5.1799 # < t-60s
            (1750489160_000_000, round((price_hash + 3.2), 5),  quantity_hash + 3, True),  # price 6.2 @quantity: - 5.1799 # < t-60s
            (1750489165_000_000, round((price_hash + 3.3), 5),  quantity_hash + 3, True),  # price 6.3 @quantity: - 5.1799 # < t-60s
            (1750489170_000_000, round((price_hash + 3.4), 5),  quantity_hash + 3, True),  # price 6.4 @quantity: - 5.1799 # < t-60s
            (1750489175_000_000, round((price_hash + 3.5), 5),  quantity_hash + 3, True),  # price 6.5 @quantity: - 5.1799 # < t-60s
            (1750489180_000_000, round((price_hash + 3.6), 5),  quantity_hash + 3, True),  # price 6.6 @quantity: - 5.1799 # < t-60s
            (1750489185_000_000, round((price_hash + 3.0), 5),  quantity_hash + 3, True),  # price 6.0 @quantity: - 5.1799 # < t-60s
            (1750489190_000_000, round((price_hash + 1.0), 5),  quantity_hash + 3, True),  # price 4.0 @quantity: - 5.1799 # < t-60s
            (1750489195_000_000, round((price_hash +10.0), 5),  quantity_hash + 1, False), # price13.0 @quantity: - 5.1799 # < t-60s

            (1750489200_000_000, round((price_hash + 6.9), 5), quantity_hash + 4, False),  # price 9.9 @quantity: + 6.1799
            (1750489229_999_999, round((price_hash + 2.5), 5), quantity_hash + 2, False),  # price 5.5 @quantity: + 4.1799
            (1750489230_000_000, round((price_hash + 2.4), 5), quantity_hash + 4, False),  # price 5.4 @quantity: + 6.1799
            (1750489230_000_000, round((price_hash + 2.7), 5), quantity_hash + 4, False),  # price 5.7 @quantity: + 6.1799
            (1750489244_999_999, round((price_hash + 2.5), 5), quantity_hash + 3, False),  # price 5.5 @quantity: + 5.1799
            (1750489249_999_999, round((price_hash + 2.6), 5), quantity_hash + 4, True),   # price 5.6 @quantity: - 6.1799
            (1750489254_999_999, round((price_hash + 2.7), 5), quantity_hash + 2, False),  # price 5.7 @quantity: + 4.1799
            (1750489255_000_000, round((price_hash + 2.8), 5), quantity_hash + 2, False),  # price 5.8 @quantity: + 4.1799
            (1750489256_999_999, round((price_hash + 2.9), 5), quantity_hash + 3, True),   # price 5.9 @quantity: - 5.1799
            (1750489258_999_999, round((price_hash + 3.0), 5), quantity_hash + 4, False),  # price 6.0 @quantity: + 6.1799
            (1750489260_000_000, round((price_hash + 3.9), 5), quantity_hash + 2, True),   # price 6.9 @quantity: - 4.1799
        ]



        for (timestamp_of_receive, price, quantity, is_buyer_market_maker) in expected_trade_entries_data_list:
            market_state.update(
                TradeEntry(
                    timestamp_of_receive=timestamp_of_receive,
                    symbol=symbol,
                    price=price,
                    quantity=quantity,
                    is_buyer_market_maker=is_buyer_market_maker,
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
        assert svc.calculate_best_volume_imbalance(market_state.order_book) == 0.5239599395200527

    def test_best_n_price_levels_volume_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        # 19.3598 - 7.3598 / 19.3598 + 7.3598 = 12 / 26.7196 ~= 0.45
        assert svc.calculate_best_n_price_levels_volume_imbalance(market_state.order_book, 2) == 0.4491085195886166

        # 29.5397 - 11.5397 / 29.5397 + 11.5397 = 18 / 41.0794 ~= 0.44
        assert svc.calculate_best_n_price_levels_volume_imbalance(market_state.order_book, 3) == 0.43817582535285327

        # 45.8995 - 19.8995 / 45.8995 + 19.8995 = 26 / 65.7990 ~= 0.40
        assert svc.calculate_best_n_price_levels_volume_imbalance(market_state.order_book, 5) == 0.39514278332497454

    def test_calculate_volume_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (50.0794 - 19.8995) / (50.0794 + 19.8995)
        assert svc.calculate_volume_imbalance(market_state.order_book) == 0.43127142610129626

    def test_calculate_queue_imbalance(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (6-5)/(6+5) = 1/11 ~= 0.09
        assert svc.calculate_queue_imbalance(market_state.order_book) == 0.09090909090909091

    def test_calculate_gap(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # (second_bid + second_ask) - (best_bid + best_ask)
        # = (4.5 + 5.3) - (5.0 + 5.1) = 9.8 - 10.1 = -0.3
        assert svc.calculate_gap(market_state.order_book) == -0.29999999999999893

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
        assert svc.calculate_vwap_deviation(market_state.order_book) == -0.06450836970108521

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
        assert svc.calculate_simplified_slope_imbalance(market_state.order_book) == 0.6365813262114586


    def test_calculate_trade_count_imbalance_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # 5-4/5+4 = 1/9 = 0.11
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 1) == -1
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 3) == 0
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 5) == -0.3333333333333333
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 10) == 0.2
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 15) == 0.0
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 30) == 0.14285714285714285
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 60) == 0.4

    def test_calculate_trade_volume_diff(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 1) == -4.1799
        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 3) == 2.0
        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 5) == -3.1799
        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 10) == 5.1799
        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 15) == -1.0
        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 30) == 4.1799
        assert svc.calculate_trade_volume_diff(market_state.rolling_trade_statistics, 60) == 20.719599999999996

    def test_calculate_price_difference_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 1) == 0.0
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 3) == 0.9000000000000004
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 5) == 1.0
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 10) == 1.3000000000000007
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 15) == 1.4000000000000004
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 30) == 1.2000000000000002
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 60) == -3


    def test_calculate_rate_of_return_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 1) == 1.0/6.0 * 100
        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 3) == 0.9/5.9 * 100
        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 5) == 1.0/5.8 * 100
        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 10) == 1.3/5.6 * 100
        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 15) == 1.4/5.5 * 100
        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 30) == 1.2/5.7 * 100
        # assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 60) == -3.0/9.9 * 100

        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 1)  == 0.0
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 3)  == 0.15000000000000005
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 5)  == 0.1694915254237288
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 10) == 0.2321428571428573
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 15) == 0.25454545454545463
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 30) == 0.2105263157894737
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 60) == -0.30303030303030304

    def test_calculate_difference_depth_volatility_imbalance_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 1) == 1.0
        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 3) == 0.0
        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 5) == 0.0
        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 10) == -0.2
        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 15) == -0.3333333333333333
        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 30) == 0.1111111111111111
        assert svc.calculate_difference_depth_count_imbalance(market_state.rolling_difference_depth_statistics, 60) == 0.2727272727272727
