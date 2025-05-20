from cpp_binance_orderbook import OrderBook, DifferenceDepthEntry


class TestOrderBook:

    def test_init(self):
        ...

    class TestUpdateOrderBookWithUpdate:
        def test_given_order_book_when_add_new_orders_then_orders_are_injected_and_sorted_correctly(self):
            ob = OrderBook()

            # inject some asks out of price order
            for price, qty in [(11.0, 1.0), (12.0, 2.0), (10.0, 2.0), (11.5, 2.0), (11.1, 2.0)]:
                e = DifferenceDepthEntry()
                e.price = price
                e.quantity = qty
                e.is_ask = True
                ob.update(e)

            # inject some bids out of price order
            for price, qty in [(9.0, 1.0), (5.0, 2.0), (7.0, 2.0), (6.5, 2.0), (8.1, 2.0)]:
                e = DifferenceDepthEntry()
                e.price = price
                e.quantity = qty
                e.is_ask = False
                ob.update(e)

            # Asks should be ascending
            asks = [(lvl.price, lvl.quantity) for lvl in ob.asks()]
            assert asks == [
                (10.0, 2.0),
                (11.0, 1.0),
                (11.1, 2.0),
                (11.5, 2.0),
                (12.0, 2.0),
            ]

            # Bids should be descending
            bids = [(lvl.price, lvl.quantity) for lvl in ob.bids()]
            assert bids == [
                (9.0, 1.0),
                (8.1, 2.0),
                (7.0, 2.0),
                (6.5, 2.0),
                (5.0, 2.0),
            ]

            # Counts and sums
            assert ob.ask_count() == 5
            assert ob.bid_count() == 5
            assert ob.sum_ask_quantity() == 1+2+2+2+2
            assert ob.sum_bid_quantity() == 1+2+2+2+2

            # Best / second prices & quantities
            assert ob.best_ask_price()    == 10.0
            assert ob.best_ask_quantity() == 2.0
            assert ob.second_ask_price()  == 11.0

            assert ob.best_bid_price()    == 9.0
            assert ob.best_bid_quantity() == 1.0
            assert ob.second_bid_price()  == 8.1

        def test_given_order_book_when_add_existing_price_level_order_then_order_is_being_updated_and_sorted_correctly(self):
            ob = OrderBook()

            # initial injection of one ask at 10 and one bid at 9
            init = [(10.0, 2.0, True), (9.0, 3.0, False)]
            for price, qty, is_ask in init:
                e = DifferenceDepthEntry()
                e.price = price
                e.quantity = qty
                e.is_ask = is_ask
                ob.update(e)

            # update ask@10 to qty=5
            e = DifferenceDepthEntry()
            e.price = 10.0
            e.quantity = 5.0
            e.is_ask = True
            ob.update(e)

            # update bid@9 to qty=1
            e = DifferenceDepthEntry()
            e.price = 9.0
            e.quantity = 1.0
            e.is_ask = False
            ob.update(e)

            # only one level each side, so order remains
            asks = [(lvl.price, lvl.quantity) for lvl in ob.asks()]
            bids = [(lvl.price, lvl.quantity) for lvl in ob.bids()]
            assert asks == [(10.0, 5.0)]
            assert bids == [(9.0, 1.0)]

            # counts and sums updated
            assert ob.ask_count() == 1
            assert ob.sum_ask_quantity() == 5.0
            assert ob.bid_count() == 1
            assert ob.sum_bid_quantity() == 1.0

        def test_given_order_book_when_get_asks_then_list_is_correct(self):
            ob = OrderBook()

            for timestamp_of_receive, (price, qty) in [
                (1, (11.0, 1)),
                (2, (12.0, 2)),
                (3, (10.0, 2)),
                (4, (11.5, 2)),
                (5, (11.1, 2)),
            ]:
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = timestamp_of_receive
                e.price               = price
                e.quantity            = qty
                e.is_ask              = True
                ob.update(e)

            for timestamp_of_receive, (price, qty) in [
                (6, (9.0, 1)),
                (7, (5.0, 2)),
                (8, (7.0, 2)),
                (9, (6.5, 2)),
                (10, (8.1, 2)),
            ]:
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = timestamp_of_receive
                e.price               = price
                e.quantity            = qty
                e.is_ask              = False
                ob.update(e)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ob.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ob.bids()]

            expected_asks = [
                (10.0, 2),
                (11.0, 1),
                (11.1, 2),
                (11.5, 2),
                (12.0, 2)
            ]
            expected_bids = [
                (9.0, 1),
                (8.1, 2),
                (7.0, 2),
                (6.5, 2),
                (5.0, 2)
            ]
            assert actual_asks == expected_asks, f"Asks mismatch: got {actual_asks}"
            assert actual_bids == expected_bids, f"Bids mismatch: got {actual_bids}"

            asks = ob.asks()
            assert isinstance(asks, list)
            assert all(isinstance(l, DifferenceDepthEntry) for l in asks)
            assert [(l.price, l.quantity) for l in asks] == [
                (10.0, 2),
                (11.0, 1),
                (11.1, 2),
                (11.5, 2),
                (12.0, 2)
            ]

            # bids() → descending
            bids = ob.bids()
            assert isinstance(bids, list)
            assert all(isinstance(l, DifferenceDepthEntry) for l in bids)
            assert [(l.price, l.quantity) for l in bids] == [
                (9.0, 1),
                (8.1, 2),
                (7.0, 2),
                (6.5, 2),
                (5.0, 2)
            ]

    class TestBaseOrderBookVariables:

        @staticmethod
        def get_sample_order_book():
            # Create an orderbook with known levels:
            # Asks: 10@2, 11@3, 12@4
            # Bids: 9@1, 8@2, 7@3
            ob = OrderBook()
            for price, qty in [
                (10.0, 2.0), # best ask
                (12.0, 4.0),
                (11.0, 3.0), # 2nd best ask
            ]:
                e = DifferenceDepthEntry()
                e.price = price
                e.quantity = qty
                e.is_ask = True
                ob.update(e)
            for price, qty in [
                (9.0, 1.0), # best bid
                (7.0, 3.0),
                (8.0, 2.0), # 2nd best ask
            ]:
                e = DifferenceDepthEntry()
                e.price = price
                e.quantity = qty
                e.is_ask = False
                ob.update(e)
            return ob

        def test_ask_count(self):
            ob = self.get_sample_order_book()
            assert ob.ask_count() == 3

        def test_bid_count(self):
            ob = self.get_sample_order_book()
            assert ob.bid_count() == 3

        def test_sum_ask_quantity(self):
            ob = self.get_sample_order_book()
            assert ob.sum_ask_quantity() == 9.0

        def test_sum_bid_quantity(self):
            ob = self.get_sample_order_book()
            assert ob.sum_bid_quantity() == 6.0

        def test_best_ask_price(self):
            ob = self.get_sample_order_book()
            assert ob.best_ask_price() == 10.0

        def test_best_bid_price(self):
            ob = self.get_sample_order_book()
            assert ob.best_bid_price() == 9.0

        def test_best_ask_quantity(self):
            ob = self.get_sample_order_book()
            assert ob.best_ask_quantity() == 2.0

        def test_best_bid_quantity(self):
            ob = self.get_sample_order_book()
            assert ob.best_bid_quantity() == 1.0

        def test_second_ask_price(self):
            ob = self.get_sample_order_book()
            assert ob.second_ask_price() == 11.0

        def test_second_bid_price(self):
            ob = self.get_sample_order_book()
            assert ob.second_bid_price() == 8.0
