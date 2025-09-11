import cpp_binance_orderbook
import pytest
from cpp_binance_orderbook import OrderBook, DifferenceDepthEntry, Symbol, Market


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
        def get_sample_order_book(
                symbol: cpp_binance_orderbook.Symbol,
                market: cpp_binance_orderbook.Market,
                price_hash: float,
                quantity_hash: float
        ) -> cpp_binance_orderbook.OrderBook:

            ob = OrderBook()

            expected_data_list = [
                (1, 1, round((price_hash + 2.1), 5), quantity_hash + 1),    # best ask
                (2, 1, round((price_hash + 2.5), 5), quantity_hash + 2),
                (3, 1, round((price_hash + 2.3), 5), quantity_hash + 2),    #           2nd best
                (4, 1, round((price_hash + 3.1), 5), quantity_hash + 2),
                (5, 1, round((price_hash + 4.1), 5), quantity_hash + 2),

                (6,  0, round((price_hash + 1.0), 5), quantity_hash + 3),
                (7,  0, round((price_hash + 2.0), 5), quantity_hash + 3),    # best bid
                (8,  0, round((price_hash + 0.0), 5), quantity_hash + 2),
                (9,  0, round((price_hash + 1.5), 5), quantity_hash + 7),    #           2nd best
                (10, 0, round((price_hash + 1.1), 5), quantity_hash + 9),
                (11, 0, round((price_hash + 1.2), 5), quantity_hash + 8),
            ]

            for (timestamp_of_receive, is_ask, price, quantity) in expected_data_list:
                ob.update(
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

            return ob

        def test_ask_count(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.ask_count() == 5

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=1,
                    price=5.3,
                    quantity=1,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.ask_count() == 5

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=1,
                    price=5.3,
                    quantity=0,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.ask_count() == 4

        def test_bid_count(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.bid_count() == 6

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=0,
                    price=4.1,
                    quantity=1,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.bid_count() == 6

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=0,
                    price=4.1,
                    quantity=0,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.bid_count() == 5

        def test_sum_ask_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.sum_ask_quantity() == 19.0

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=1,
                    price=5.3,
                    quantity=1,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.sum_ask_quantity() == 19.0 - 3.0

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=1,
                    price=5.3,
                    quantity=0,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.sum_ask_quantity() == 19.0 - 4.0

        def test_sum_bid_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.sum_bid_quantity() == 44.0

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=0,
                    price=4.1,
                    quantity=1,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.sum_bid_quantity() == 44.0 - 10.0

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=0,
                    price=4.1,
                    quantity=0,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.sum_bid_quantity() == 44.0 - 11.0

        def test_best_ask_price(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.best_ask_price() == 5.1

        def test_best_bid_price(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.best_bid_price() == 5.0

        def test_best_ask_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.best_ask_quantity() == 3.0

        def test_best_bid_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.best_bid_quantity() == 5.0

        def test_second_ask_price(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.second_ask_price() == 5.3

        def test_second_bid_price(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.second_bid_price() == 4.5


        def test_cumulative_sum_of_top_n_asks_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.cumulative_quantity_of_top_n_asks(4) == 15.0

        def test_cumulative_sum_of_top_n_bids_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.cumulative_quantity_of_top_n_bids(4) == 35.0

        def test_sum_of_price_times_quantity(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.sum_of_price_times_quantity() == 295.9

            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=0,
                    price=4.1,
                    quantity=1,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.sum_of_price_times_quantity() == 295.9 - (10 * 4.1)
            ob.update(
                DifferenceDepthEntry(
                    timestamp_of_receive=12,
                    symbol=Symbol.ADAUSDT,
                    is_ask=0,
                    price=4.1,
                    quantity=0,
                    is_last=1,
                    market=Market.USD_M_FUTURES
                )
            )
            assert ob.sum_of_price_times_quantity() == 295.9 - (11 * 4.1)

        def test_best_nth_bid_price(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )

            assert ob.best_nth_bid_price(1) == 5.
            assert ob.best_nth_bid_price(3) == 4.2
            assert ob.best_nth_bid_price(6) == 3.

        def test_best_nth_ask_price(self):
            ob = self.get_sample_order_book(
                symbol=Symbol.ADAUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=3,
                quantity_hash=2
            )
            assert ob.best_nth_ask_price(1) == 5.1
            assert ob.best_nth_ask_price(3) == 5.5
            assert ob.best_nth_ask_price(5) == 7.1

        def test_sum_total_ask_bid_quantity(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # sum_ask = 19.0, sum_bid = 44.0
            assert ob.sum_total_ask_bid_quantity() == pytest.approx(63.0, abs=1e-12)

        def test_delta_bid_count(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=0,
                price=4.9,
                quantity=2.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_bid_count() == 1
            assert ob.delta_sum_bid_quantity() == pytest.approx(2.0, abs=1e-12)

        def test_delta_ask_count(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=1,
                price=7.2,
                quantity=1.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_ask_count() == 1
            assert ob.delta_sum_ask_quantity() == pytest.approx(1.0, abs=1e-12)

        def test_delta_best_ask_quantity(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # best ask początkowo 5.1 @ 3.0
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=1,
                price=5.1,
                quantity=5.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_best_ask_quantity() == pytest.approx(2.0, abs=1e-12)
            assert ob.delta_best_ask_price() == pytest.approx(0.0, abs=1e-12)

        def test_delta_best_bid_quantity(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # best bid początkowo 5.0 @ 5.0
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=0,
                price=5.0,
                quantity=8.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_best_bid_quantity() == pytest.approx(3.0, abs=1e-12)
            assert ob.delta_best_bid_price() == pytest.approx(0.0, abs=1e-12)

        def test_delta_best_ask_price(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # best ask początkowo 5.1 @ 3.0 → zmiana ceny na 5.05
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=1,
                price=5.05,
                quantity=3.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_best_ask_price() == pytest.approx(-0.05, abs=1e-12)
            assert ob.delta_best_ask_quantity() == pytest.approx(0.0, abs=1e-12)

        def test_delta_best_bid_price(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # best bid początkowo 5.0 @ 5.0 → zmiana ceny na 5.05
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=0,
                price=5.05,
                quantity=5.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_best_bid_price() == pytest.approx(0.05, abs=1e-12)
            assert ob.delta_best_bid_quantity() == pytest.approx(0.0, abs=1e-12)

        def test_delta_sum_ask_quantity(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # nowy poziom ASK (nie-best)
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=1,
                price=7.0,
                quantity=4.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_sum_ask_quantity() == pytest.approx(4.0, abs=1e-12)
            assert ob.delta_sum_bid_quantity() == pytest.approx(0.0, abs=1e-12)

        def test_delta_sum_bid_quantity(self):
            ob = TestOrderBook.TestBaseOrderBookVariables.get_sample_order_book(
                symbol=Symbol.ADAUSDT, market=Market.USD_M_FUTURES,
                price_hash=3, quantity_hash=2
            )
            # nowy poziom BID (nie-best)
            e = DifferenceDepthEntry(
                timestamp_of_receive=100,
                symbol=Symbol.ADAUSDT,
                is_ask=0,
                price=4.3,
                quantity=6.0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            ob.update(e)
            assert ob.delta_sum_bid_quantity() == pytest.approx(6.0, abs=1e-12)
            assert ob.delta_sum_ask_quantity() == pytest.approx(0.0, abs=1e-12)
