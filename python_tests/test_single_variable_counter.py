from cpp_binance_orderbook import (
    OrderBook, DifferenceDepthEntry, TradeEntry,
    single_variable_counter as svc
)

class TestSingleVariableCounter:
    @staticmethod
    def get_sample_order_book():
        # Create an orderbook with known levels:
        # Asks: 10@2, 11@3, 12@4
        # Bids: 9@1, 8@2, 7@3
        ob = OrderBook()
        for timestamp_of_receive, (price, qty) in [
            (1, (11.0, 1)), # 2nd best ask
            (2, (12.0, 1)),
            (3, (10.0, 2)), # best ask
            (4, (11.5, 1)),
            (5, (11.1, 1)),
        ]:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = qty
            e.is_ask = True
            e.timestamp_of_receive = timestamp_of_receive
            ob.update(e)
        for timestamp_of_receive, (price, qty) in [
            (6, (9.0, 1)), # best bid
            (7, (5.0, 2)),
            (8, (7.0, 2)),
            (9, (6.5, 2)),
            (10, (8.1, 2)), # 2nd best bid
            (10, (8.0, 1))
        ]:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = qty
            e.is_ask = False
            e.timestamp_of_receive = timestamp_of_receive
            ob.update(e)
        return ob

    def test_calculate_best_ask_price(self):
        ob = self.get_sample_order_book()
        assert svc.calculate_best_ask_price(ob) == 10.0

    def test_calculate_best_bid_price(self):
        ob = self.get_sample_order_book()
        assert svc.calculate_best_bid_price(ob) == 9.0

    def test_calculate_mid_price(self):
        ob = self.get_sample_order_book()
        expected = (10.0 + 9.0) / 2 # 9.5
        assert svc.calculate_mid_price(ob) == expected

    def test_calculate_best_volume_imbalance(self):
        ob = self.get_sample_order_book()
        # (bid_qty - ask_qty)/(bid_qty + ask_qty) = (1-2)/(1+2) = -1/3
        result = svc.calculate_best_volume_imbalance(ob)
        assert result == -1/3

    def test_calculate_queue_imbalance(self):
        ob = self.get_sample_order_book()
        # (bid_count - ask_count)/(bid_count + ask_count) = (6-5)/11 = 1/11 = 0.09
        result = svc.calculate_queue_imbalance(ob)
        assert round(result, 2) == 0.09

    def test_calculate_volume_imbalance(self):
        ob = self.get_sample_order_book()
        # best ask volumes:  1 1 1 1 2 = 6
        # best bid volumes:  1 2 2 2 2 1 = 10
        # (10-5)/15 = 5/15 = 0.2(5
        result = svc.calculate_volume_imbalance(ob)
        print(f'result {result}')
        assert result == 0.25

    def test_calculate_gap(self):
        ob = self.get_sample_order_book()
        # (second_bid + second_ask) - (best_bid + best_ask)
        # = (8.1 + 11.0) - (9.0 + 10.0) = 19.1 - 19 = 0.1
        result = svc.calculate_gap(ob)
        assert round(result,2) == 0.1

    def test_calculate_is_aggressor_ask(self):
        t = TradeEntry()
        t.is_buyer_market_maker = True
        assert svc.calculate_is_aggressor_ask(t) is True
        t.is_buyer_market_maker = False
        assert svc.calculate_is_aggressor_ask(t) is False
