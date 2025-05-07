from cpp_binance_orderbook import DifferenceDepthEntry


class TestDifferenceDepthEntry:

    def test_given_default_order_book_entry_when_to_list_then_list_matches_all_fields(self):
        # default case
        e = DifferenceDepthEntry()
        expected = [
            0,      # timestamp_of_receive
            "",     # stream
            "",     # event_type
            0,      # event_time
            0,      # transaction_time
            "",     # symbol
            0,      # first_update_id
            0,      # final_update_id
            0,      # final_update_id_in_last_stream
            0,      # is_ask (False -> 0)
            0.0,    # price
            0.0,    # quantity
            "",     # ps_unknown_field
            0       # is_last (False -> 0)
        ]
        assert e.to_list() == expected

        # populated case
        e2 = DifferenceDepthEntry()
        e2.timestamp_of_receive           = 123456789
        e2.stream                         = "depth_stream"
        e2.event_type                     = "update"
        e2.event_time                     = 42
        e2.transaction_time               = 84
        e2.symbol                         = "BTCUSD"
        e2.first_update_id                = 1001
        e2.final_update_id                = 1002
        e2.final_update_id_in_last_stream = 1003
        e2.is_ask                         = True
        e2.price                          = 5678.90
        e2.quantity                       = 1.234
        e2.ps_unknown_field               = "foo"
        e2.is_last                        = True

        expected2 = [
            123456789,
            "depth_stream",
            "update",
            42,
            84,
            "BTCUSD",
            1001,
            1002,
            1003,
            1,          # True -> 1
            5678.90,
            1.234,
            "foo",
            1           # True -> 1
        ]
        assert e2.to_list() == expected2

    def test_given_default_order_book_entry_when_field_names_then_returned_names_are_correct(self):
        e = DifferenceDepthEntry()
        expected_names = [
            "timestamp_of_receive",
            "stream",
            "event_type",
            "event_time",
            "transaction_time",
            "symbol",
            "first_update_id",
            "final_update_id",
            "final_update_id_in_last_stream",
            "is_ask",
            "price",
            "quantity",
            "ps_unknown_field",
            "is_last"
        ]
        # field_names property returns correct list
        assert e.field_names == expected_names

        # and each of those names corresponds to an actual attribute on the object
        for attr in expected_names:
            assert hasattr(e, attr), f"DifferenceDepthEntry is missing attribute '{attr}'"
