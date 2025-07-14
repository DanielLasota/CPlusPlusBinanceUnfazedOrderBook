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
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 1) == -1
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 3) == 0
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 5) == 0
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 10) == 0.2
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 15) == 0.0
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 30) == 0.33
        assert svc.calculate_trade_count_imbalance(market_state.rolling_trade_statistics, 60) == 0.45

    def test_calculate_cumulative_delta_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 1) == -4.1799
        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 3) == 2.0
        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 5) == 1.0
        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 10) == 5.1799
        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 15) == -1.0
        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 30) == 16.5397
        assert svc.calculate_cumulative_delta(market_state.rolling_trade_statistics, 60) == 26.8995

    def test_calculate_price_difference_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 1) == 0.9
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 3) == 1.0
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 5) == 1.1
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 10) == 1.3
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 15) == 1.4
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 30) == 1.2
        assert svc.calculate_price_difference(market_state.rolling_trade_statistics, 60) == -3


    def test_calculate_rate_of_return_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 1) == round(0.9/6.0 * 100, 2)
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 3) == round(1.0/5.9 * 100, 2)
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 5) == round(1.1/5.8 * 100, 2)
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 10) == round(1.3/5.6 * 100, 2)
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 15) == round(1.4/5.5 * 100, 2)
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 30) == round(1.2/5.7 * 100, 2)
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 60) == round(-3.0/9.9 * 100, 2)

        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 1)  == 15.00
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 3)  == 16.95
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 5)  == 18.97
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 10) == 23.21
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 15) == 25.45
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 30) == 21.05
        assert svc.calculate_rate_of_return(market_state.rolling_trade_statistics, 60) == -30.30

    def test_calculate_difference_depth_volatility_imbalance_n_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 1) == 1.0
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 3) == 0.0
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 5) == 0.0
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 10) == -0.2
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 15) == -0.33
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 30) == 0.11
        assert svc.calculate_difference_depth_volatility_imbalance(market_state.rolling_difference_depth_statistics, 60) == 0.27

    def test_calculate_rsi_5_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )

        '''
            rsi 5s, 14 periods:
            
            diff_i: (gainSum += diff_i): 5 1.1
            diff_i: (gainSum += diff_i): 10 0.2
            diff_i: (gainSum += diff_i): 15 0.1
            diff_i: (lossSum += -diff_i): 20 -0.2
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 -4.2
            diff_i: (lossSum += -diff_i): 40 0
            diff_i: (lossSum += -diff_i): 45 0
            diff_i: (lossSum += -diff_i): 50 0
            diff_i: (lossSum += -diff_i): 55 0
            diff_i: (lossSum += -diff_i): 60 0
            diff_i: (lossSum += -diff_i): 65 -3.1
            diff_i: (gainSum += diff_i): 70 9
            
            avgGain = 0.742857
            avgLoss = 0.535714
            periods = 14
            
            rs = 1.38667
            rsi = 100-(100/1+rs) = 58.10061...
        '''
        assert svc.calculate_rsi(market_state.rolling_trade_statistics, 0, 5) == 58.1
        '''
            diff_i: (gainSum += diff_i): 4 0.1
            diff_i: (gainSum += diff_i): 6 0.2
            diff_i: (gainSum += diff_i): 8 0.1
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (gainSum += diff_i): 12 0.1
            diff_i: (lossSum += -diff_i): 14 0
            diff_i: (lossSum += -diff_i): 16 0
            diff_i: (lossSum += -diff_i): 18 -0.2
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 22 0
            diff_i: (lossSum += -diff_i): 24 0
            diff_i: (lossSum += -diff_i): 26 0
            diff_i: (lossSum += -diff_i): 28 0
            avgGain = 0.1
            avgLoss = 0.0142857
            periods = 14
            rs = 7
            rsi = 100-(100/1+rs) = 87.5
        '''
        assert svc.calculate_rsi(market_state.rolling_trade_statistics, 0, 2) == 87.5

    def test_calculate_stoch_rsi_5_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        '''
            offset0
            windowTimeSeconds5
            diff_i: (gainSum += diff_i): 5 1.1
            diff_i: (gainSum += diff_i): 10 0.2
            diff_i: (gainSum += diff_i): 15 0.1
            diff_i: (lossSum += -diff_i): 20 -0.2
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 -4.2
            diff_i: (lossSum += -diff_i): 40 0
            diff_i: (lossSum += -diff_i): 45 0
            diff_i: (lossSum += -diff_i): 50 0
            diff_i: (lossSum += -diff_i): 55 0
            diff_i: (lossSum += -diff_i): 60 0
            diff_i: (lossSum += -diff_i): 65 -3.1
            diff_i: (gainSum += diff_i): 70 9
            avgGain = 0.742857
            avgLoss = 0.535714
            periods = 14
            rs = 1.38667
            rsi = 58.1
            XXXXXX
            
            offset5
            windowTimeSeconds5
            diff_i: (gainSum += diff_i): 5 0.2
            diff_i: (gainSum += diff_i): 10 0.1
            diff_i: (lossSum += -diff_i): 15 -0.2
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 -4.2
            diff_i: (lossSum += -diff_i): 35 0
            diff_i: (lossSum += -diff_i): 40 0
            diff_i: (lossSum += -diff_i): 45 0
            diff_i: (lossSum += -diff_i): 50 0
            diff_i: (lossSum += -diff_i): 55 0
            diff_i: (lossSum += -diff_i): 60 -3.1
            diff_i: (gainSum += diff_i): 65 9
            diff_i: (lossSum += -diff_i): 70 -2
            avgGain = 0.664286
            avgLoss = 0.678571
            periods = 14
            rs = 0.978947
            rsi = 49.47
            XXXXXX
            
            offset10
            windowTimeSeconds5
            diff_i: (gainSum += diff_i): 5 0.1
            diff_i: (lossSum += -diff_i): 10 -0.2
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 -4.2
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 0
            diff_i: (lossSum += -diff_i): 40 0
            diff_i: (lossSum += -diff_i): 45 0
            diff_i: (lossSum += -diff_i): 50 0
            diff_i: (lossSum += -diff_i): 55 -3.1
            diff_i: (gainSum += diff_i): 60 9
            diff_i: (lossSum += -diff_i): 65 -2
            diff_i: (lossSum += -diff_i): 70 -0.6
            avgGain = 0.65
            avgLoss = 0.721429
            periods = 14
            rs = 0.90099
            rsi = 47.4
            XXXXXX
            
            offset15
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 -0.2
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 -4.2
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 0
            diff_i: (lossSum += -diff_i): 40 0
            diff_i: (lossSum += -diff_i): 45 0
            diff_i: (lossSum += -diff_i): 50 -3.1
            diff_i: (gainSum += diff_i): 55 9
            diff_i: (lossSum += -diff_i): 60 -2
            diff_i: (lossSum += -diff_i): 65 -0.6
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.65
            avgLoss = 0.721429
            periods = 14
            rs = 0.90099
            rsi = 47.4
            XXXXXX
            
            offset20
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 -4.2
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 0
            diff_i: (lossSum += -diff_i): 40 0
            diff_i: (lossSum += -diff_i): 45 -3.1
            diff_i: (gainSum += diff_i): 50 9
            diff_i: (lossSum += -diff_i): 55 -2
            diff_i: (lossSum += -diff_i): 60 -0.6
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.657143
            avgLoss = 0.707143
            periods = 14
            rs = 0.929293
            rsi = 48.17
            XXXXXX
            
            offset25
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 -4.2
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 0
            diff_i: (lossSum += -diff_i): 40 -3.1
            diff_i: (gainSum += diff_i): 45 9
            diff_i: (lossSum += -diff_i): 50 -2
            diff_i: (lossSum += -diff_i): 55 -0.6
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.664286
            avgLoss = 0.707143
            periods = 14
            rs = 0.939394
            rsi = 48.44
            XXXXXX
            
            offset30
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 -4.2
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 0
            diff_i: (lossSum += -diff_i): 35 -3.1
            diff_i: (gainSum += diff_i): 40 9
            diff_i: (lossSum += -diff_i): 45 -2
            diff_i: (lossSum += -diff_i): 50 -0.6
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.671429
            avgLoss = 0.707143
            periods = 14
            rs = 0.949495
            rsi = 48.7
            XXXXXX
            
            offset35
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 0
            diff_i: (lossSum += -diff_i): 30 -3.1
            diff_i: (gainSum += diff_i): 35 9
            diff_i: (lossSum += -diff_i): 40 -2
            diff_i: (lossSum += -diff_i): 45 -0.6
            diff_i: (gainSum += diff_i): 50 0.1
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.678571
            avgLoss = 0.407143
            periods = 14
            rs = 1.66667
            rsi = 62.5
            XXXXXX
            
            offset40
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 0
            diff_i: (lossSum += -diff_i): 25 -3.1
            diff_i: (gainSum += diff_i): 30 9
            diff_i: (lossSum += -diff_i): 35 -2
            diff_i: (lossSum += -diff_i): 40 -0.6
            diff_i: (gainSum += diff_i): 45 0.1
            diff_i: (gainSum += diff_i): 50 0.1
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.6
            avgGain = 0.721429
            avgLoss = 0.407143
            periods = 14
            rs = 1.77193
            rsi = 63.92
            XXXXXX
            
            offset45
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 0
            diff_i: (lossSum += -diff_i): 20 -3.1
            diff_i: (gainSum += diff_i): 25 9
            diff_i: (lossSum += -diff_i): 30 -2
            diff_i: (lossSum += -diff_i): 35 -0.6
            diff_i: (gainSum += diff_i): 40 0.1
            diff_i: (gainSum += diff_i): 45 0.1
            diff_i: (gainSum += diff_i): 50 0.1
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.6
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.728571
            avgLoss = 0.407143
            periods = 14
            rs = 1.78947
            rsi = 64.15
            XXXXXX
            
            offset50
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 0
            diff_i: (lossSum += -diff_i): 15 -3.1
            diff_i: (gainSum += diff_i): 20 9
            diff_i: (lossSum += -diff_i): 25 -2
            diff_i: (lossSum += -diff_i): 30 -0.6
            diff_i: (gainSum += diff_i): 35 0.1
            diff_i: (gainSum += diff_i): 40 0.1
            diff_i: (gainSum += diff_i): 45 0.1
            diff_i: (gainSum += diff_i): 50 0.1
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.6
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.735714
            avgLoss = 0.407143
            periods = 14
            rs = 1.80702
            rsi = 64.38
            XXXXXX
            
            offset55
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 0
            diff_i: (lossSum += -diff_i): 10 -3.1
            diff_i: (gainSum += diff_i): 15 9
            diff_i: (lossSum += -diff_i): 20 -2
            diff_i: (lossSum += -diff_i): 25 -0.6
            diff_i: (gainSum += diff_i): 30 0.1
            diff_i: (gainSum += diff_i): 35 0.1
            diff_i: (gainSum += diff_i): 40 0.1
            diff_i: (gainSum += diff_i): 45 0.1
            diff_i: (gainSum += diff_i): 50 0.1
            diff_i: (gainSum += diff_i): 55 0.6
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.742857
            avgLoss = 0.407143
            periods = 14
            rs = 1.82456
            rsi = 64.6
            XXXXXX
            
            offset60
            windowTimeSeconds5
            diff_i: (lossSum += -diff_i): 5 -3.1
            diff_i: (gainSum += diff_i): 10 9
            diff_i: (lossSum += -diff_i): 15 -2
            diff_i: (lossSum += -diff_i): 20 -0.6
            diff_i: (gainSum += diff_i): 25 0.1
            diff_i: (gainSum += diff_i): 30 0.1
            diff_i: (gainSum += diff_i): 35 0.1
            diff_i: (gainSum += diff_i): 40 0.1
            diff_i: (gainSum += diff_i): 45 0.1
            diff_i: (gainSum += diff_i): 50 0.6
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.75
            avgLoss = 0.407143
            periods = 14
            rs = 1.84211
            rsi = 64.81
            XXXXXX
            
            offset65
            windowTimeSeconds5
            diff_i: (gainSum += diff_i): 5 9
            diff_i: (lossSum += -diff_i): 10 -2
            diff_i: (lossSum += -diff_i): 15 -0.6
            diff_i: (gainSum += diff_i): 20 0.1
            diff_i: (gainSum += diff_i): 25 0.1
            diff_i: (gainSum += diff_i): 30 0.1
            diff_i: (gainSum += diff_i): 35 0.1
            diff_i: (gainSum += diff_i): 40 0.1
            diff_i: (gainSum += diff_i): 45 0.6
            diff_i: (gainSum += diff_i): 50 0.1
            diff_i: (gainSum += diff_i): 55 0.1
            diff_i: (gainSum += diff_i): 60 0.1
            diff_i: (gainSum += diff_i): 65 0.1
            diff_i: (gainSum += diff_i): 70 0.1
            avgGain = 0.757143
            avgLoss = 0.185714
            periods = 14
            rs = 4.07692
            rsi = 80.3
            XXXXXX
        '''

        '''
            price_hash = 3
            quantity_hash = 5
            
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
                (1750489210_000_000, round((price_hash + 6.9), 5), quantity_hash + 4, False),  # price 9.9 @quantity: + 6.1799
                (1750489215_000_000, round((price_hash + 6.9), 5), quantity_hash + 4, False),  # price 9.9 @quantity: + 6.1799
                (1750489220_000_000, round((price_hash + 6.9), 5), quantity_hash + 4, False),  # price 9.9 @quantity: + 6.1799
                (1750489225_000_000, round((price_hash + 6.9), 5), quantity_hash + 4, False),  # price 9.9 @quantity: + 6.1799
                (1750489230_000_000, round((price_hash + 2.7), 5), quantity_hash + 4, False),  # price 5.7 @quantity: + 6.1799
                (1750489235_000_000, round((price_hash + 2.7), 5), quantity_hash + 4, False),  # price 5.7 @quantity: + 6.1799
                (1750489240_000_000, round((price_hash + 2.7), 5), quantity_hash + 4, False),  # price 5.7 @quantity: + 6.1799
                (1750489244_999_999, round((price_hash + 2.5), 5), quantity_hash + 3, False),  # price 5.5 @quantity: + 5.1799
                (1750489249_999_999, round((price_hash + 2.6), 5), quantity_hash + 4, True),   # price 5.6 @quantity: - 6.1799
                (1750489255_000_000, round((price_hash + 2.8), 5), quantity_hash + 2, False),  # price 5.8 @quantity: + 4.1799
                (1750489260_000_000, round((price_hash + 3.9), 5), quantity_hash + 2, True),   # price 6.9 @quantity: - 4.1799
            ]
            
            import pandas as pd
            ONE_SEC_US = 1_000_000
            
            # rozwinąć każde oryginalne wejście 5 razy
            expanded_trade_entries = []
            for ts, price, qty, is_buyer_maker in expected_trade_entries_data_list:
                for i in range(5):
                    new_ts = ts + i * ONE_SEC_US
                    expanded_trade_entries.append((new_ts, price, qty, is_buyer_maker))
            
            # teraz tworzysz DataFrame z expanded_trade_entries
            import pandas as pd
            
            df = pd.DataFrame(expected_trade_entries_data_list, columns=['timestamp', 'price', 'quantity', 'is_ask'])
            
            from ta.momentum import RSIIndicator, StochRSIIndicator
            df['rsi'] = RSIIndicator(df['price'], window=14).rsi()
            stoch = StochRSIIndicator(df['price'], window=14, smooth1=1, smooth2=1)
            df['stoch_rsi_raw'] = stoch.stochrsi()
            df
        '''
        assert svc.calculate_stoch_rsi(market_state.rolling_trade_statistics, 5) == 0.33

    def test_calculate_macd_2_seconds(self):
        market_state = self.get_sample_order_book(
            symbol=Symbol.ADAUSDT,
            market=Market.USD_M_FUTURES,
            price_hash=3,
            quantity_hash=2.1799
        )
        # SMA(24s) = (5.5+5.6+5.7+5.8+5.9+6.0+6.9)/7 ≈ 5.91428571
        # SMA(52s) = (5.5+5.7+5.5+5.6+5.7+5.8+5.9+6.0+6.9)/9 ≈ 5.84444444
        # MACD = 5.91428571 - 5.84444444 ≈ 0.06984127
        assert svc.calculate_macd(market_state.rolling_trade_statistics, 2) == 0.06984127

