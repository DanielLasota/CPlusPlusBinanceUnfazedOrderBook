from cpp_binance_orderbook import (
    MarketState,
    DifferenceDepthEntry,
    TradeEntry,
    OrderBookMetricsCalculator
)


class TestMarketState:

    def test_init(self):
        ...

    class TestUpdateMarketStateWithUpdate:

        def test_given_market_state_when_add_new_orders_then_orders_are_injected_and_sorted_correctly(self):
            ms = MarketState()

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
                ms.update(e)

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
                ms.update(e)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

        def test_given_market_state_when_add_existing_price_level_order_then_order_is_being_updated_and_sorted_correctly(self):
            ms = MarketState()

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
                ms.update(e)

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
                ms.update(e)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

            ### end of ms preparation

            original_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            original_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            for i, (price, qty) in enumerate(original_asks, start=1):
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = 100 + i
                e.price                = price
                e.quantity             = qty + 1
                e.is_ask               = True
                ms.update(e)
            for i, (price, qty) in enumerate(original_bids, start=1):
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = 200 + i
                e.price                = price
                e.quantity             = max(qty - 1, 0)
                e.is_ask               = False
                ms.update(e)

            post_update_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            post_update_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            # 4) ręcznie zdefiniuj oczekiwane wartości:
            #    - wszystkie asks: qty wzrosło o 1
            #    - bids przy qty-1=0 są usunięte, pozostałe mają qty-1
            expected_asks = [
                (10.0, 3),  # 2 → 3
                (11.0, 2),  # 1 → 2
                (11.1, 3),  # 2 → 3
                (11.5, 3),  # 2 → 3
                (12.0, 3),  # 2 → 3
            ]
            expected_bids = [
                # (9.0, 1 → 0) usunięte
                (8.1, 1),   # 2 → 1
                (7.0, 1),   # 2 → 1
                (6.5, 1),   # 2 → 1
                (5.0, 1),   # 2 → 1
            ]

            # 5) asercje
            assert post_update_asks == expected_asks, f"Asks mismatch: got {actual_asks}"
            assert post_update_bids == expected_bids, f"Bids mismatch: got {actual_bids}"

        def test_given_existing_order_when_add_order_with_zero_quantity_then_order_is_removed(self):
            ms = MarketState()

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
                ms.update(e)

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
                ms.update(e)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

            ### end of ms preparation

            original_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            original_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            best_ask, best_bid = ms.order_book.asks()[0], ms.order_book.bids()[0]
            third_best_ask, third_best_bid = ms.order_book.asks()[2], ms.order_book.bids()[2]

            for old_order in [best_ask, best_bid, third_best_ask, third_best_bid]:
                new_order = DifferenceDepthEntry()
                new_order.timestamp_of_receive = 11
                new_order.price = old_order.price
                new_order.quantity = 0
                new_order.is_ask = old_order.is_ask
                ms.update(new_order)

            post_update_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            post_update_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            # ręcznie wypisujemy, co powinno zostać
            expected_asks = [
                (11.0, 1),   # initial.ask[1]
                (11.5, 2),   # initial.ask[3]
                (12.0, 2),   # initial.ask[4]
            ]

            expected_bids = [
                (8.1, 2),    # initial.bid[1]
                (6.5, 2),    # initial.bid[3]
                (5.0, 2),    # initial.bid[4]
            ]

            assert post_update_asks == expected_asks, f"Asks mismatch: got {post_update_asks}"
            assert post_update_bids == expected_bids, f"Bids mismatch: got {post_update_bids}"

        def test_given_last_trade_storage_when_updating_first_trade_is_last_trade_updated_correctly(self):
            ms = MarketState()

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
                ms.update(e)

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
                ms.update(e)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

            ### end of ms preparation

            some_trade = TradeEntry()
            some_trade.timestamp_of_receive = 1
            some_trade.price = 10
            some_trade.quantity = 1

            ms.update(some_trade)

            last_trade = ms.last_trade
            assert ms.has_last_trade is True

            assert last_trade.timestamp_of_receive == some_trade.timestamp_of_receive
            assert last_trade.price == some_trade.price
            assert last_trade.quantity == some_trade.quantity
            assert last_trade.is_buyer_market_maker == some_trade.is_buyer_market_maker

        def test_given_last_trade_storage_when_updating_existing_trade_is_last_trade_updated_correctly(self):
            ms = MarketState()

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
                ms.update(e)
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
                ms.update(e)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

            ### end of ms preparation

            pre_defined_trade = TradeEntry()
            pre_defined_trade.timestamp_of_receive = 1
            pre_defined_trade.price = 5
            pre_defined_trade.quantity = 0.5
            ms.update(pre_defined_trade)

            last_trade = ms.last_trade
            assert ms.has_last_trade is True
            assert last_trade.timestamp_of_receive == pre_defined_trade.timestamp_of_receive
            assert last_trade.price == pre_defined_trade.price
            assert last_trade.quantity == pre_defined_trade.quantity
            assert last_trade.is_buyer_market_maker == pre_defined_trade.is_buyer_market_maker


            trade_update = TradeEntry()
            trade_update.timestamp_of_receive = 2
            trade_update.price = 10
            trade_update.quantity = 1
            ms.update(trade_update)

            last_trade = ms.last_trade
            assert ms.has_last_trade is True
            assert last_trade.timestamp_of_receive == trade_update.timestamp_of_receive
            assert last_trade.price == trade_update.price
            assert last_trade.quantity == trade_update.quantity
            assert last_trade.is_buyer_market_maker == trade_update.is_buyer_market_maker

        def test_given_updating_order_or_trade_when_update_then_last_timestamp_of_receive_equals_last_timestamp(self):
            ms = MarketState()

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
                ms.update(e)
                assert ms.last_timestamp_of_receive == e.timestamp_of_receive
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
                ms.update(e)
                assert ms.last_timestamp_of_receive == e.timestamp_of_receive

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

            ### end of ms preparation

            pre_defined_trade = TradeEntry()
            pre_defined_trade.timestamp_of_receive = 1
            pre_defined_trade.price = 5
            pre_defined_trade.quantity = 0.5
            ms.update(pre_defined_trade)

            last_trade = ms.last_trade
            assert ms.has_last_trade is True
            assert last_trade.timestamp_of_receive == pre_defined_trade.timestamp_of_receive
            assert last_trade.price == pre_defined_trade.price
            assert last_trade.quantity == pre_defined_trade.quantity
            assert last_trade.is_buyer_market_maker == pre_defined_trade.is_buyer_market_maker
            assert ms.last_timestamp_of_receive == pre_defined_trade.timestamp_of_receive

            trade_update = TradeEntry()
            trade_update.timestamp_of_receive = 2
            trade_update.price = 10
            trade_update.quantity = 1
            ms.update(trade_update)

            last_trade = ms.last_trade
            assert ms.has_last_trade is True
            assert last_trade.timestamp_of_receive == trade_update.timestamp_of_receive
            assert last_trade.price == trade_update.price
            assert last_trade.quantity == trade_update.quantity
            assert last_trade.is_buyer_market_maker == trade_update.is_buyer_market_maker

            assert ms.last_timestamp_of_receive == trade_update.timestamp_of_receive

    class TestUpdateMarketStateWithFastUpdate:

        def test_given_market_state_when_add_new_orders_then_orders_are_injected_and_sorted_correctly(self):
            ms = MarketState()

            for timestamp_of_receive, (price, qty) in [
                (1, (11.0, 1)),
                (2, (12.0, 2)),
                (3, (10.0, 2)),
                (4, (11.5, 2)),
                (5, (11.1, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, True)

            for timestamp_of_receive, (price, qty) in [
                (6, (9.0, 1)),
                (7, (5.0, 2)),
                (8, (7.0, 2)),
                (9, (6.5, 2)),
                (10, (8.1, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, False)

            actual_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            actual_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

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

        def test_given_market_state_when_add_existing_price_level_order_then_order_is_being_updated_and_sorted_correctly(self):
            ms = MarketState()

            # initial injection
            for timestamp_of_receive, (price, qty) in [
                (1, (11.0, 1)),
                (2, (12.0, 2)),
                (3, (10.0, 2)),
                (4, (11.5, 2)),
                (5, (11.1, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, True)
            for timestamp_of_receive, (price, qty) in [
                (6, (9.0, 1)),
                (7, (5.0, 2)),
                (8, (7.0, 2)),
                (9, (6.5, 2)),
                (10, (8.1, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, False)

            original_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            original_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            # update existing levels
            for i, (price, qty) in enumerate(original_asks, start=1):
                ms.update_orderbook(100 + i, price, qty + 1, True)
            for i, (price, qty) in enumerate(original_bids, start=1):
                ms.update_orderbook(200 + i, price, max(qty - 1, 0), False)

            post_update_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            post_update_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            expected_asks = [
                (10.0, 3),
                (11.0, 2),
                (11.1, 3),
                (11.5, 3),
                (12.0, 3),
            ]
            expected_bids = [
                (8.1, 1),
                (7.0, 1),
                (6.5, 1),
                (5.0, 1),
            ]

            assert post_update_asks == expected_asks, f"Asks mismatch: got {post_update_asks}"
            assert post_update_bids == expected_bids, f"Bids mismatch: got {post_update_bids}"

        def test_given_existing_order_when_add_order_with_zero_quantity_then_order_is_removed(self):
            ms = MarketState()

            # initial injection
            for timestamp_of_receive, (price, qty) in [
                (1, (11.0, 1)),
                (2, (12.0, 2)),
                (3, (10.0, 2)),
                (4, (11.5, 2)),
                (5, (11.1, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, True)
            for timestamp_of_receive, (price, qty) in [
                (6, (9.0, 1)),
                (7, (5.0, 2)),
                (8, (7.0, 2)),
                (9, (6.5, 2)),
                (10, (8.1, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, False)

            best_ask, best_bid = ms.order_book.asks()[0], ms.order_book.bids()[0]
            third_best_ask, third_best_bid = ms.order_book.asks()[2], ms.order_book.bids()[2]

            # remove some levels by zero quantity
            for old in [best_ask, best_bid, third_best_ask, third_best_bid]:
                ms.update_orderbook(11, old.price, 0, old.is_ask)

            post_update_asks = [(lvl.price, lvl.quantity) for lvl in ms.order_book.asks()]
            post_update_bids = [(lvl.price, lvl.quantity) for lvl in ms.order_book.bids()]

            expected_asks = [
                (11.0, 1),
                (11.5, 2),
                (12.0, 2),
            ]
            expected_bids = [
                (8.1, 2),
                (6.5, 2),
                (5.0, 2),
            ]

            assert post_update_asks == expected_asks, f"Asks mismatch: got {post_update_asks}"
            assert post_update_bids == expected_bids, f"Bids mismatch: got {post_update_bids}"

        def test_given_last_trade_storage_when_updating_first_trade_is_last_trade_updated_correctly(self):
            ms = MarketState()

            # inject some orders (so trade registry can proceed)
            for timestamp_of_receive, (price, qty) in [
                (1, (11.0, 1)),
                (2, (12.0, 2)),
            ]:
                ms.update_orderbook(timestamp_of_receive, price, qty, True)

            # first trade
            ts, price, qty, is_bmm = 1, 10.0, 1.0, False
            ms.update_trade_registry(ts, price, qty, is_bmm)

            last = ms.last_trade
            assert ms.has_last_trade is True
            assert last.timestamp_of_receive == ts
            assert last.price == price
            assert last.quantity == qty
            assert last.is_buyer_market_maker == is_bmm

        def test_given_last_trade_storage_when_updating_existing_trade_is_last_trade_updated_correctly(self):
            ms = MarketState()
            # inject one order
            ms.update_orderbook(1, 11.0, 1, True)

            # initial trade
            ms.update_trade_registry(1, 5.0, 0.5, True)
            last = ms.last_trade
            assert last.price == 5.0

            # update trade
            ms.update_trade_registry(2, 10.0, 1.0, False)
            last = ms.last_trade
            assert ms.has_last_trade is True
            assert last.timestamp_of_receive == 2
            assert last.price == 10.0
            assert last.quantity == 1.0
            assert last.is_buyer_market_maker is False

        def test_given_updating_order_or_trade_when_update_then_last_timestamp_of_receive_equals_last_timestamp(self):
            ms = MarketState()

            # orders
            for ts, (price, qty) in [
                (1, (11.0, 1)),
                (2, (12.0, 2)),
            ]:
                ms.update_orderbook(ts, price, qty, True)
                assert ms.last_timestamp_of_receive == ts

            # trade
            ms.update_trade_registry(3, 5.0, 0.5, True)
            assert ms.last_timestamp_of_receive == 3

    class TestMarketStateCountVariablesWithUpdate:

        def test_given_empty_market_state_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]

            ms = MarketState()
            obmc = OrderBookMetricsCalculator(variables)
            variables = obmc.count_market_state_metrics(ms)

            assert variables is None

        def test_given_non_empty_orderbook_empty_trade_registry_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()
            # wstawiamy dwa poziomy ask i dwa poziomy bid
            for price, qty in [(10.0, 1), (11.0, 2)]:
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = 1
                e.price = price
                e.quantity = qty
                e.is_ask = True
                ms.update(e)
            for price, qty in [(9.0, 1), (8.0, 2)]:
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = 2
                e.price = price
                e.quantity = qty
                e.is_ask = False
                ms.update(e)

            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_empty_orderbook_non_empty_trade_registry_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()

            t = TradeEntry()
            t.timestamp_of_receive = 100
            t.price = 5.0
            t.quantity = 1.5
            t.is_buyer_market_maker = True
            ms.update(t)

            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_orderbook_with_one_order_non_empty_trade_registry_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()
            # dokładnie po jednym poziomie na każdej stronie
            a = DifferenceDepthEntry()
            a.timestamp_of_receive = 1
            a.price = 10.0
            a.quantity = 2.0
            a.is_ask = True
            ms.update(a)

            b = DifferenceDepthEntry()
            b.timestamp_of_receive = 2
            b.price = 9.0
            b.quantity = 3.0
            b.is_ask = False
            ms.update(b)

            # jeden trade
            t = TradeEntry()
            t.timestamp_of_receive = 200
            t.price = 9.5
            t.quantity = 1.0
            t.is_buyer_market_maker = False
            ms.update(t)

            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_non_empty_market_state_when_count_order_book_metrics_then_returns_correct_order_book_metrics_entry(self):
            # przygotuj maskę na wszystkie metryki
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]

            ms = MarketState()
            # dwa poziomy ask (10@1, 11@1)
            for i, (price, qty) in enumerate([(10.0, 1.0), (11.0, 1.0)], start=1):
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = i
                e.price = price
                e.quantity = qty
                e.is_ask = True
                ms.update(e)
            # dwa poziomy bid (9@1, 8@2)
            for i, (price, qty) in enumerate([(9.0, 1.0), (8.0, 2.0)], start=3):
                e = DifferenceDepthEntry()
                e.timestamp_of_receive = i
                e.price = price
                e.quantity = qty
                e.is_ask = False
                ms.update(e)
            # ostatni trade (ustawi timestamp_of_receive)
            trade_ts = 100
            t = TradeEntry()
            t.timestamp_of_receive = trade_ts
            t.price = 7.7
            t.quantity = 0.5
            t.is_buyer_market_maker = True
            ms.update(t)

            obmc = OrderBookMetricsCalculator(variables)
            entry = obmc.count_market_state_metrics(ms)
            assert entry is not None

            # sprawdź wszystkie pola
            # bestAsk = 10, bestBid = 9
            assert entry.bestAskPrice == 10.0
            assert entry.bestBidPrice == 9.0
            # midPrice = (10+9)/2 = 9.5
            assert entry.midPrice == 9.5
            # bestVolumeImbalance = (1 - 1)/(1+1) = 0
            assert entry.bestVolumeImbalance == 0.0
            # queueImbalance = (2 - 2)/(2+2) = 0
            assert entry.queueImbalance == 0.0
            # volumeImbalance = ((1+2) - (1+1)) / ((1+2)+(1+1)) = (3-2)/(5) = 0.2
            assert entry.volumeImbalance == (3.0 - 2.0) / 5.0
            # gap = (secondBid + secondAsk) - (bestBid + bestAsk) = (8+11)-(9+10)=19-19=0
            assert entry.gap == 0.0
            # isAggressorAsk == True
            assert entry.isAggressorAsk is True
            # timestampOfReceive == trade_ts
            assert entry.timestampOfReceive == trade_ts

    class TestMarketStateCountVariablesWithFastUpdate:

        def test_given_empty_market_state_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "gap"
            ]
            ms = MarketState()
            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_non_empty_orderbook_empty_trade_registry_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()
            # wstawiamy dwa poziomy ask i dwa poziomy bid
            for price, qty in [(10.0, 1), (11.0, 2)]:
                ms.update_orderbook(1, price, qty, True)
            for price, qty in [(9.0, 1), (8.0, 2)]:
                ms.update_orderbook(2, price, qty, False)
            # nie było jeszcze żadnego trade
            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_empty_orderbook_non_empty_trade_registry_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()
            # tylko trade, zero poziomów w orderbooku
            ms.update_trade_registry(100, 5.0, 1.5, True)
            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_orderbook_with_one_order_non_empty_trade_registry_when_count_order_book_metrics_then_returns_none(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()
            # dokładnie po jednym poziomie na każdej stronie
            ms.update_orderbook(1, 10.0, 2.0, True)
            ms.update_orderbook(2,  9.0, 3.0, False)
            # jeden trade
            ms.update_trade_registry(200, 9.5, 1.0, False)
            obmc = OrderBookMetricsCalculator(variables)
            result = obmc.count_market_state_metrics(ms)
            assert result is None

        def test_given_non_empty_market_state_when_count_order_book_metrics_then_returns_correct_order_book_metrics_entry(self):
            variables = [
                "timestampOfReceive",
                "bestAskPrice",
                "bestBidPrice",
                "midPrice",
                "bestVolumeImbalance",
                "queueImbalance",
                "volumeImbalance",
                "gap",
                "isAggressorAsk"
            ]
            ms = MarketState()
            # dwa poziomy ask (10@1, 11@1)
            ms.update_orderbook(1, 10.0, 1.0, True)
            ms.update_orderbook(2, 11.0, 1.0, True)
            # dwa poziomy bid (9@1, 8@2)
            ms.update_orderbook(3, 9.0, 1.0, False)
            ms.update_orderbook(4, 8.0, 2.0, False)
            # ostatni trade
            trade_ts = 100
            ms.update_trade_registry(trade_ts, 7.7, 0.5, True)

            obmc = OrderBookMetricsCalculator(variables)
            entry = obmc.count_market_state_metrics(ms)
            assert entry is not None

            assert entry.bestAskPrice == 10.0
            assert entry.bestBidPrice == 9.0
            assert entry.midPrice == 9.5
            assert entry.bestVolumeImbalance == 0.0
            assert entry.queueImbalance == 0.0
            assert entry.volumeImbalance == (3.0 - 2.0) / 5.0
            assert entry.gap == 0.0
            assert entry.isAggressorAsk is True
            assert entry.timestampOfReceive == trade_ts
