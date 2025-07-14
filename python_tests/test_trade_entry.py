from cpp_binance_orderbook import TradeEntry, Market, Symbol


class TestTradeEntry:

    def test_given_default_trade_entry_when_to_list_then_list_matches_all_fields(self):
        # default case
        e = TradeEntry()
        expected = [
            0,    # timestamp_of_receive
            Symbol.UNKNOWN,   # symbol
            0.0,  # price
            0.0,  # quantity
            0,    # is_buyer_market_maker (False -> 0)
            0,    # is_last (False -> 0),
            Market.UNKNOWN
        ]
        assert e.to_list() == expected

        # populated case
        e2 = TradeEntry()
        e2.timestamp_of_receive   = 987654321
        e2.symbol                 = Symbol.ETHUSDT
        e2.price                  = 2500.55
        e2.quantity               = 0.005
        e2.is_buyer_market_maker  = True
        e2.is_last                = True
        e2.market                 = Market.COIN_M_FUTURES

        expected2 = [
            987654321,
            Symbol.ETHUSDT,
            2500.55,
            0.005,
            1,        # True -> 1
            1,        # True -> 1
            Market.COIN_M_FUTURES
        ]
        assert e2.to_list() == expected2

    def test_given_default_trade_entry_when_field_names_then_returned_names_are_correct(self):
        e = TradeEntry()
        expected_names = [
            "timestamp_of_receive",
            "symbol",
            "price",
            "quantity",
            "is_buyer_market_maker",
            "is_last",
            "market"
        ]
        # field_names property returns correct list
        assert e.field_names() == expected_names

        # and each name corresponds to an actual attribute on the object
        for attr in expected_names:
            assert hasattr(e, attr), f"TradeEntry is missing attribute '{attr}'"

    def test_given_trade_entry_constructor_when_field_names_then_returned_names_are_correct(self):
        e = TradeEntry(
            1,
            Symbol.ETHUSDT,
            1.1,
            2.2,
            False,
            False,
            Market.SPOT
        )
        expected_names = [
            "timestamp_of_receive",
            "symbol",
            "price",
            "quantity",
            "is_buyer_market_maker",
            "is_last",
            "market"
        ]
        assert e.field_names() == expected_names

        for attr in expected_names:
            assert hasattr(e, attr), f"TradeEntry is missing attribute '{attr}'"
