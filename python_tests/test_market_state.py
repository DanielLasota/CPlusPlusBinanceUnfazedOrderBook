import pytest
from cpp_binance_orderbook import MarketState, DifferenceDepthEntry, TradeEntry

class TestMarketState:

    def test_given_series_of_difference_depth_entries_is_orderbook_updated_and_sorted_correctly(self):
        ms = MarketState()

        for price in [5.0, 1.0, 3.0]:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = 1.0
            e.is_ask = False
            ms.update(e)
        for price in [2.0, 4.0, 6.0]:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = 2.0
            e.is_ask = True
            ms.update(e)
        bid_prices = [b.price for b in ms.orderBook.bids]
        ask_prices = [a.price for a in ms.orderBook.asks]

        assert bid_prices == sorted(bid_prices, reverse=True), "Bids should be sorted descending"
        assert ask_prices == sorted(ask_prices), "Asks should be sorted ascending"

    def test_given_series_of_depth_snapshot_entries_is_orderbook_updated_and_sorted_correctly(self):
        ms = MarketState()

        for price in [10.0, 8.0]:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = 5.0
            e.is_ask = False
            ms.update(e)
        for price in [11.0, 12.0]:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = 6.0
            e.is_ask = True
            ms.update(e)
        bid_prices = [b.price for b in ms.orderBook.bids]
        ask_prices = [a.price for a in ms.orderBook.asks]

        assert bid_prices == [10.0, 8.0]
        assert ask_prices == [11.0, 12.0]

    def test_given_series_of_final_snapshot_entries_is_orderbook_updated_and_sorted_correctly(self):
        ms = MarketState()

        bids = [1.1, 1.3, 1.2]
        asks = [2.1, 2.3, 2.2]
        for price in bids:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = 1.0
            e.is_ask = False
            ms.update(e)
        for price in asks:
            e = DifferenceDepthEntry()
            e.price = price
            e.quantity = 1.0
            e.is_ask = True
            ms.update(e)

        assert [b.price for b in ms.orderBook.bids] == sorted(bids, reverse=True)
        assert [a.price for a in ms.orderBook.asks] == sorted(asks)

    def test_given_no_last_trade_state_when_updating_first_trade_is_last_trade_updated_correctly(self):
        ms = MarketState()
        for price in [1.0, 2.0]:
            e = DifferenceDepthEntry(); e.price = price; e.quantity = 1.0; e.is_ask = False; ms.update(e)
        for price in [3.0, 4.0]:
            e = DifferenceDepthEntry(); e.price = price; e.quantity = 1.0; e.is_ask = True; ms.update(e)

        assert ms.count_order_book_metrics(1) is None
        t = TradeEntry()
        t.timestamp_of_receive = 12345
        t.price = 0.0
        t.quantity = 0.0
        t.is_last = False
        t.is_buyer_market_maker = False
        ms.update(t)
        assert ms.count_order_book_metrics(1) is not None

    def test_given_last_trade_exists_when_updating_trade_is_last_trade_updated_correctly(self):
        ms = MarketState()

        for price in [10.0, 9.0]:
            e = DifferenceDepthEntry(); e.price = price; e.quantity = 1.0; e.is_ask = False; ms.update(e)
        for price in [11.0, 12.0]:
            e = DifferenceDepthEntry(); e.price = price; e.quantity = 1.0; e.is_ask = True; ms.update(e)

        t1 = TradeEntry()
        t1.timestamp_of_receive = 100
        t1.price = 0.0
        t1.quantity = 0.0
        t1.is_last = False
        t1.is_buyer_market_maker = True
        ms.update(t1)
        m1 = ms.count_order_book_metrics(1)
        assert m1 is not None
        assert m1.timestampOfReceive == 100
        t2 = TradeEntry()
        t2.timestamp_of_receive = 200
        t2.price = 0.0
        t2.quantity = 0.0
        t2.is_last = False
        t2.is_buyer_market_maker = False
        ms.update(t2)
        m2 = ms.count_order_book_metrics(1)
        assert m2.timestampOfReceive == 200

    def test_given_empty_MarketState_when_count_order_book_metrics_then_returns_None(self):
        ms = MarketState()
        assert ms.count_order_book_metrics(1) is None

    def testtest(self):
        import time
        ms = MarketState()
        start = time.perf_counter()

        ms.update_depth(
            timestamp_of_receive=0,
            price=10.0,
            quantity=5.0,
            is_ask=False
        )

        ms.update_depth(
            timestamp_of_receive=0,
            price=5.0,
            quantity=5.0,
            is_ask=False
        )

        # e1 = DifferenceDepthEntry()
        # e1.timestamp_of_receive = 0
        # e1.price = 10.0
        # e1.quantity = 5.0
        # e1.is_ask = True
        # ms.update(e1)
        #
        # e2 = DifferenceDepthEntry()
        # e2.timestamp_of_receive = 0
        # e2.price = 11.0
        # e2.quantity = 5.0
        # e2.is_ask = True
        # ms.update(e2)

        end_time = time.perf_counter()
        elapsed = (end_time - start) * 1e6
        print(f"\n🏷️  Czas wstawienia 4 zleceń: {elapsed:.2f} µs")
        ms.print_order_book()
