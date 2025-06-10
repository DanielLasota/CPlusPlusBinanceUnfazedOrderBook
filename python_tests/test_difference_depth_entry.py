from cpp_binance_orderbook import DifferenceDepthEntry, Market, Symbol


class TestDifferenceDepthEntry:

    def test_given_default_order_book_entry_when_to_list_then_list_matches_all_fields(self):
        # default case
        e = DifferenceDepthEntry()

        print(e)

        expected = [
            0,
            Symbol.UNKNOWN,
            0,
            0.0,
            0.0,
            0,
            Market.UNKNOWN
        ]
        assert e.to_list() == expected

        # populated case
        e2 = DifferenceDepthEntry()
        e2.timestamp_of_receive           = 123456789
        e2.symbol                         = Symbol.BTCUSDT
        e2.is_ask                         = True
        e2.price                          = 5678.90
        e2.quantity                       = 1.234
        e2.is_last                        = True
        e2.market                         = Market.USD_M_FUTURES

        expected2 = [
            123456789,
            Symbol.BTCUSDT,
            1,
            5678.90,
            1.234,
            1,
            Market.USD_M_FUTURES
        ]
        assert e2.to_list() == expected2

    def test_given_default_order_book_entry_when_field_names_then_returned_names_are_correct(self):
        e = DifferenceDepthEntry()
        expected_names = [
            "timestamp_of_receive",
            "symbol",
            "is_ask",
            "price",
            "quantity",
            "is_last",
            "market"
        ]
        assert e.field_names() == expected_names

        for attr in expected_names:
            assert hasattr(e, attr), f"DifferenceDepthEntry is missing attribute '{attr}'"

    def test_given_constructor_order_book_entry_when_field_names_then_returned_names_are_correct(self):
        expected_names = [
            "timestamp_of_receive",
            "symbol",
            "is_ask",
            "price",
            "quantity",
            "is_last",
            "market"
        ]
        e = DifferenceDepthEntry(
            0,
            Symbol.UNKNOWN,
            0,
            0.0,
            0.0,
            0,
            Market.SPOT
        )
        assert e.field_names() == expected_names

        for attr in expected_names:
            assert hasattr(e, attr), f"DifferenceDepthEntry is missing attribute '{attr}'"
