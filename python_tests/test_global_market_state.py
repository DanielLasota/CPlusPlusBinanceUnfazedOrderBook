from types import NoneType

import cpp_binance_orderbook
import pytest

from cpp_binance_orderbook import (
    GlobalMarketState,
    DifferenceDepthEntry,
    parse_mask,
    Market,
    TradeEntry,
    OrderBookMetricsEntry,
    Symbol
)

def sample_order_list(
        symbol: cpp_binance_orderbook.Symbol,
        market: Market,
        price_hash: float,
        quantity_hash: float
) -> list[DifferenceDepthEntry]:

    orders_list = []

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
    ]

    for (timestamp_of_receive, is_ask, price, quantity) in expected_data_list:
        orders_list.append(
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

    return orders_list


class TestGlobalMarketState:

    def test_init(self):
        ...

    class TestUpdateGlobalMarketState:

        def test_given_global_market_state_when_add_new_orders_to_multiple_market_states_then_orders_are_injected_and_sorted_correctly(self):
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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            ms_trxusdt = gms.get_market_state(Symbol.TRXUSDT, Market.SPOT)
            ms_btcusdt = gms.get_market_state(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            actual_trxusdt = [*reversed(ms_trxusdt.order_book.asks()), *ms_trxusdt.order_book.bids()]
            expected_trxusdt = sorted(sample_trxusdt_spot_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_trxusdt, expected_trxusdt):
                assert a.to_list() == e.to_list()
            del sample_trxusdt_spot_order_list
            assert ms_trxusdt.order_book.ask_count() == 5
            assert ms_trxusdt.order_book.bid_count() == 5
            assert ms_trxusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_trxusdt if x.is_ask == 1])
            assert ms_trxusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_trxusdt if x.is_ask == 0])
            assert ms_trxusdt.order_book.best_ask_price() == 12.34589
            assert ms_trxusdt.order_book.best_bid_price() == 12.24589
            assert ms_trxusdt.order_book.best_ask_quantity() == 1.0
            assert ms_trxusdt.order_book.best_bid_quantity() == 3.0
            assert ms_trxusdt.order_book.second_ask_price() == 12.54589
            assert ms_trxusdt.order_book.second_bid_price() == 11.74589
            assert ms_trxusdt.last_timestamp_of_receive == 10.0

            actual_btcusdt = [*reversed(ms_btcusdt.order_book.asks()), *ms_btcusdt.order_book.bids()]
            expected_btcusdt = sorted(sample_btcusdt_usd_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_btcusdt, expected_btcusdt):
                assert a.to_list() == e.to_list()
            del sample_btcusdt_usd_m_futures_order_list
            assert ms_btcusdt.order_book.ask_count() == 5
            assert ms_btcusdt.order_book.bid_count() == 5
            assert ms_btcusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_btcusdt if x.is_ask == 1])
            assert ms_btcusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_btcusdt if x.is_ask == 0])
            assert ms_btcusdt.order_book.best_ask_price() == 22.1
            assert ms_btcusdt.order_book.best_bid_price() == 22.0
            assert ms_btcusdt.order_book.best_ask_quantity() == 11.0
            assert ms_btcusdt.order_book.best_bid_quantity() == 13.0
            assert ms_btcusdt.order_book.second_ask_price() == 22.3
            assert ms_btcusdt.order_book.second_bid_price() == 21.5
            assert ms_btcusdt.last_timestamp_of_receive == 10.0

            actual_adausdt = [*reversed(ms_adausdt.order_book.asks()), *ms_adausdt.order_book.bids()]
            expected_adausdt = sorted(sample_adausdt_usd_coin_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_adausdt, expected_adausdt):
                assert a.to_list() == e.to_list()
            del sample_adausdt_usd_coin_m_futures_order_list
            assert ms_adausdt.order_book.ask_count() == 5
            assert ms_adausdt.order_book.bid_count() == 5
            assert ms_adausdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_adausdt if x.is_ask == 1])
            assert ms_adausdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_adausdt if x.is_ask == 0])
            assert ms_adausdt.order_book.best_ask_price() == 32.1
            assert ms_adausdt.order_book.best_bid_price() == 32.0
            assert ms_adausdt.order_book.best_ask_quantity() == 21.0
            assert ms_adausdt.order_book.best_bid_quantity() == 23.0
            assert ms_adausdt.order_book.second_ask_price() == 32.3
            assert ms_adausdt.order_book.second_bid_price() == 31.5
            assert ms_adausdt.last_timestamp_of_receive == 10.0

            actual_dogeusdt = [*reversed(ms_dogeusdt.order_book.asks()), *ms_dogeusdt.order_book.bids()]
            expected_dogeusdt = sorted(sample_dogeusdt_usd_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_dogeusdt, expected_dogeusdt):
                assert a.to_list() == e.to_list()
            del sample_dogeusdt_usd_m_futures_order_list
            assert ms_dogeusdt.order_book.ask_count() == 5
            assert ms_dogeusdt.order_book.bid_count() == 5
            assert ms_dogeusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_dogeusdt if x.is_ask == 1])
            assert ms_dogeusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_dogeusdt if x.is_ask == 0])
            assert ms_dogeusdt.order_book.best_ask_price() == 42.1
            assert ms_dogeusdt.order_book.best_bid_price() == 42.0
            assert ms_dogeusdt.order_book.best_ask_quantity() == 31.0
            assert ms_dogeusdt.order_book.best_bid_quantity() == 33.0
            assert ms_dogeusdt.order_book.second_ask_price() == 42.3
            assert ms_dogeusdt.order_book.second_bid_price() == 41.5
            assert ms_dogeusdt.last_timestamp_of_receive == 10.0

            assert gms.get_market_state_count() == 4

            assert sorted(gms.get_market_state_list(), key=lambda x: (int(x[0]), int(x[1]))) == sorted([
                (Symbol.TRXUSDT, Market.SPOT),
                (Symbol.DOGEUSDT, Market.USD_M_FUTURES),
                (Symbol.BTCUSDT, Market.USD_M_FUTURES),
                (Symbol.ADAUSDT, Market.COIN_M_FUTURES),
            ], key=lambda x: (int(x[0]), int(x[1])))

        def test_given_global_market_state_when_add_existing_price_level_order_then_order_is_being_updated_and_sorted_correctly(self):
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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            ms_trxusdt = gms.get_market_state(Symbol.TRXUSDT, Market.SPOT)
            ms_btcusdt = gms.get_market_state(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            # -------------------------------------------------------- #
            # -------------------------------------------------------- #
            new_ask_trxusdt_timestamp_of_receive = 21
            new_ask_trxusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_trxusdt_timestamp_of_receive,
                symbol=Symbol.TRXUSDT,
                is_ask=1,
                price=12.54589,
                quantity=3.0,
                is_last=0,
                market=Market.SPOT
                )
            gms.update(new_ask_trxusdt)
            to_remove = None
            for e in sample_trxusdt_spot_order_list:
                if e.price == new_ask_trxusdt.price and e.is_ask == new_ask_trxusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_trxusdt_spot_order_list.remove(to_remove)
            sample_trxusdt_spot_order_list.append(new_ask_trxusdt)

            new_bid_trxusdt_timestamp_of_receive = 22
            new_bid_trxusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_trxusdt_timestamp_of_receive,
                symbol=Symbol.TRXUSDT,
                is_ask=0,
                price=10.24589,
                quantity=1.0,
                is_last=0,
                market=Market.SPOT
            )

            gms.update(new_bid_trxusdt)
            to_remove = None
            for e in sample_trxusdt_spot_order_list:
                if e.price == new_bid_trxusdt.price and e.is_ask == new_bid_trxusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_trxusdt_spot_order_list.remove(to_remove)
            sample_trxusdt_spot_order_list.append(new_bid_trxusdt)

            actual_trxusdt = [*reversed(ms_trxusdt.order_book.asks()), *ms_trxusdt.order_book.bids()]
            expected_trxusdt = sorted(sample_trxusdt_spot_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_trxusdt, expected_trxusdt):
                assert a.to_list() == e.to_list()
            del sample_trxusdt_spot_order_list
            assert ms_trxusdt.order_book.ask_count() == 5
            assert ms_trxusdt.order_book.bid_count() == 5
            assert ms_trxusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_trxusdt if x.is_ask == 1])
            assert ms_trxusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_trxusdt if x.is_ask == 0])
            assert ms_trxusdt.order_book.best_ask_price() == 12.34589
            assert ms_trxusdt.order_book.best_bid_price() == 12.24589
            assert ms_trxusdt.order_book.best_ask_quantity() == 1.0
            assert ms_trxusdt.order_book.best_bid_quantity() == 3.0
            assert ms_trxusdt.order_book.second_ask_price() == 12.54589
            assert ms_trxusdt.order_book.second_bid_price() == 11.74589
            assert ms_trxusdt.last_timestamp_of_receive == 22
            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_btcusdt_timestamp_of_receive = 23
            new_ask_btcusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_btcusdt_timestamp_of_receive,
                symbol=Symbol.BTCUSDT,
                is_ask=1,
                price=22.3,
                quantity=3.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )

            gms.update(new_ask_btcusdt)
            to_remove = None
            for e in sample_btcusdt_usd_m_futures_order_list:
                if e.price == new_ask_btcusdt.price and e.is_ask == new_ask_btcusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_btcusdt_usd_m_futures_order_list.remove(to_remove)
            sample_btcusdt_usd_m_futures_order_list.append(new_ask_btcusdt)

            new_bid_btcusdt_timestamp_of_receive = 24
            new_bid_btcusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_btcusdt_timestamp_of_receive,
                symbol=Symbol.BTCUSDT,
                is_ask=0,
                price=22.0,
                quantity=1.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )
            gms.update(new_bid_btcusdt)

            to_remove = None
            for e in sample_btcusdt_usd_m_futures_order_list:
                if e.price == new_bid_btcusdt.price and e.is_ask == new_bid_btcusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_btcusdt_usd_m_futures_order_list.remove(to_remove)
            sample_btcusdt_usd_m_futures_order_list.append(new_bid_btcusdt)

            actual_btcusdt = [*reversed(ms_btcusdt.order_book.asks()), *ms_btcusdt.order_book.bids()]
            expected_btcusdt = sorted(sample_btcusdt_usd_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_btcusdt, expected_btcusdt):
                assert a.to_list() == e.to_list()
            del sample_btcusdt_usd_m_futures_order_list
            assert ms_btcusdt.order_book.ask_count() == 5
            assert ms_btcusdt.order_book.bid_count() == 5
            assert ms_btcusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_btcusdt if x.is_ask == 1])
            assert ms_btcusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_btcusdt if x.is_ask == 0])
            assert ms_btcusdt.order_book.best_ask_price() == 22.1
            assert ms_btcusdt.order_book.best_bid_price() == 22.0
            assert ms_btcusdt.order_book.best_ask_quantity() == 11.0
            assert ms_btcusdt.order_book.best_bid_quantity() == 1.0
            assert ms_btcusdt.order_book.second_ask_price() == 22.3
            assert ms_btcusdt.order_book.second_bid_price() == 21.5
            assert ms_btcusdt.last_timestamp_of_receive == 24
            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_adausdt_timestamp_of_receive = 25
            new_ask_adausdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_adausdt_timestamp_of_receive,
                symbol=Symbol.ADAUSDT,
                is_ask=1,
                price=33.1,
                quantity=3.0,
                is_last=0,
                market=Market.COIN_M_FUTURES
            )

            gms.update(new_ask_adausdt)
            to_remove = None
            for e in sample_adausdt_usd_coin_m_futures_order_list:
                if e.price == new_ask_adausdt.price and e.is_ask == new_ask_adausdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_adausdt_usd_coin_m_futures_order_list.remove(to_remove)
            sample_adausdt_usd_coin_m_futures_order_list.append(new_ask_adausdt)

            new_bid_adausdt_timestamp_of_receive = 26
            new_bid_adausdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_adausdt_timestamp_of_receive,
                symbol=Symbol.ADAUSDT,
                is_ask=0,
                price=30.0,
                quantity=1.0,
                is_last=0,
                market=Market.COIN_M_FUTURES
            )

            gms.update(new_bid_adausdt)
            to_remove = None
            for e in sample_adausdt_usd_coin_m_futures_order_list:
                if e.price == new_bid_adausdt.price and e.is_ask == new_bid_adausdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_adausdt_usd_coin_m_futures_order_list.remove(to_remove)
            sample_adausdt_usd_coin_m_futures_order_list.append(new_bid_adausdt)

            actual_adausdt = [*reversed(ms_adausdt.order_book.asks()), *ms_adausdt.order_book.bids()]
            expected_adausdt = sorted(sample_adausdt_usd_coin_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_adausdt, expected_adausdt):
                assert a.to_list() == e.to_list()
            del sample_adausdt_usd_coin_m_futures_order_list
            assert ms_adausdt.order_book.ask_count() == 5
            assert ms_adausdt.order_book.bid_count() == 5
            assert ms_adausdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_adausdt if x.is_ask == 1])
            assert ms_adausdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_adausdt if x.is_ask == 0])
            assert ms_adausdt.order_book.best_ask_price() == 32.1
            assert ms_adausdt.order_book.best_bid_price() == 32.0
            assert ms_adausdt.order_book.best_ask_quantity() == 21.0
            assert ms_adausdt.order_book.best_bid_quantity() == 23.0
            assert ms_adausdt.order_book.second_ask_price() == 32.3
            assert ms_adausdt.order_book.second_bid_price() == 31.5
            assert ms_adausdt.last_timestamp_of_receive == 26
            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_dogeusdt_timestamp_of_receive = 27
            new_ask_dogeusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_dogeusdt_timestamp_of_receive,
                symbol=Symbol.DOGEUSDT,
                is_ask=1,
                price=42.1,
                quantity=3.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )

            gms.update(new_ask_dogeusdt)
            to_remove = None
            for e in sample_dogeusdt_usd_m_futures_order_list:
                if e.price == new_ask_dogeusdt.price and e.is_ask == new_ask_dogeusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_dogeusdt_usd_m_futures_order_list.remove(to_remove)
            sample_dogeusdt_usd_m_futures_order_list.append(new_ask_dogeusdt)

            new_bid_dogeusdt_timestamp_of_receive = 28
            new_bid_dogeusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_dogeusdt_timestamp_of_receive,
                symbol=Symbol.DOGEUSDT,
                is_ask=0,
                price=42.0,
                quantity=1.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )

            gms.update(new_bid_dogeusdt)
            to_remove = None
            for e in sample_dogeusdt_usd_m_futures_order_list:
                if e.price == new_bid_dogeusdt.price and e.is_ask == new_bid_dogeusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_dogeusdt_usd_m_futures_order_list.remove(to_remove)
            sample_dogeusdt_usd_m_futures_order_list.append(new_bid_dogeusdt)

            actual_dogeusdt = [*reversed(ms_dogeusdt.order_book.asks()), *ms_dogeusdt.order_book.bids()]
            expected_dogeusdt = sorted(sample_dogeusdt_usd_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_dogeusdt, expected_dogeusdt):
                assert a.to_list() == e.to_list()
            del sample_dogeusdt_usd_m_futures_order_list
            assert ms_dogeusdt.order_book.ask_count() == 5
            assert ms_dogeusdt.order_book.bid_count() == 5
            assert ms_dogeusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_dogeusdt if x.is_ask == 1])
            assert ms_dogeusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_dogeusdt if x.is_ask == 0])
            assert ms_dogeusdt.order_book.best_ask_price() == 42.1
            assert ms_dogeusdt.order_book.best_bid_price() == 42.0
            assert ms_dogeusdt.order_book.best_ask_quantity() == 3.0
            assert ms_dogeusdt.order_book.best_bid_quantity() == 1.0
            assert ms_dogeusdt.order_book.second_ask_price() == 42.3
            assert ms_dogeusdt.order_book.second_bid_price() == 41.5
            assert ms_dogeusdt.last_timestamp_of_receive == 28

            assert gms.get_market_state_count() == 4

            assert sorted(gms.get_market_state_list(), key=lambda x: (int(x[0]), int(x[1]))) == sorted([
                (Symbol.TRXUSDT, Market.SPOT),
                (Symbol.DOGEUSDT, Market.USD_M_FUTURES),
                (Symbol.BTCUSDT, Market.USD_M_FUTURES),
                (Symbol.ADAUSDT, Market.COIN_M_FUTURES),
            ], key=lambda x: (int(x[0]), int(x[1])))

        def test_given_existing_order_when_add_order_with_zero_quantity_then_order_is_removed(self):
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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            ms_trxusdt = gms.get_market_state(Symbol.TRXUSDT, Market.SPOT)
            ms_btcusdt = gms.get_market_state(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_trxusdt_timestamp_of_receive = 21
            new_ask_trxusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_trxusdt_timestamp_of_receive,
                symbol=Symbol.TRXUSDT,
                is_ask=1,
                price=12.54589,
                quantity=0.0,
                is_last=0,
                market=Market.SPOT
            )
            gms.update(new_ask_trxusdt)
            to_remove = None
            for e in sample_trxusdt_spot_order_list:
                if e.price == new_ask_trxusdt.price and e.is_ask == new_ask_trxusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_trxusdt_spot_order_list.remove(to_remove)

            new_bid_trxusdt_timestamp_of_receive = 22
            new_bid_trxusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_trxusdt_timestamp_of_receive,
                symbol=Symbol.TRXUSDT,
                is_ask=0,
                price=10.24589,
                quantity=0.0,
                is_last=0,
                market=Market.SPOT
            )
            gms.update(new_bid_trxusdt)
            to_remove = None
            for e in sample_trxusdt_spot_order_list:
                if e.price == new_bid_trxusdt.price and e.is_ask == new_bid_trxusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_trxusdt_spot_order_list.remove(to_remove)

            actual_trxusdt = [*reversed(ms_trxusdt.order_book.asks()), *ms_trxusdt.order_book.bids()]
            expected_trxusdt = sorted(sample_trxusdt_spot_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_trxusdt, expected_trxusdt):
                assert a.to_list() == e.to_list()
            del sample_trxusdt_spot_order_list
            assert ms_trxusdt.order_book.ask_count() == 4
            assert ms_trxusdt.order_book.bid_count() == 4
            assert ms_trxusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_trxusdt if x.is_ask == 1])
            assert ms_trxusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_trxusdt if x.is_ask == 0])
            assert ms_trxusdt.order_book.best_ask_price() == 12.34589
            assert ms_trxusdt.order_book.best_bid_price() == 12.24589
            assert ms_trxusdt.order_book.best_ask_quantity() == 1.0
            assert ms_trxusdt.order_book.best_bid_quantity() == 3.0
            assert ms_trxusdt.order_book.second_ask_price() == 12.74589
            assert ms_trxusdt.order_book.second_bid_price() == 11.74589
            assert ms_trxusdt.last_timestamp_of_receive == 22

            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_btcusdt_timestamp_of_receive = 23
            new_ask_btcusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_btcusdt_timestamp_of_receive,
                symbol=Symbol.BTCUSDT,
                is_ask=1,
                price=22.3,
                quantity=0.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )
            gms.update(new_ask_btcusdt)
            to_remove = None
            for e in sample_btcusdt_usd_m_futures_order_list:
                if e.price == new_ask_btcusdt.price and e.is_ask == new_ask_btcusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_btcusdt_usd_m_futures_order_list.remove(to_remove)

            new_bid_btcusdt_timestamp_of_receive = 24
            new_bid_btcusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_btcusdt_timestamp_of_receive,
                symbol=Symbol.BTCUSDT,
                is_ask=0,
                price=22.0,
                quantity=0.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )
            gms.update(new_bid_btcusdt)
            to_remove = None
            for e in sample_btcusdt_usd_m_futures_order_list:
                if e.price == new_bid_btcusdt.price and e.is_ask == new_bid_btcusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_btcusdt_usd_m_futures_order_list.remove(to_remove)

            actual_btcusdt = [*reversed(ms_btcusdt.order_book.asks()), *ms_btcusdt.order_book.bids()]
            expected_btcusdt = sorted(sample_btcusdt_usd_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_btcusdt, expected_btcusdt):
                assert a.to_list() == e.to_list()
            del sample_btcusdt_usd_m_futures_order_list
            assert ms_btcusdt.order_book.ask_count() == 4
            assert ms_btcusdt.order_book.bid_count() == 4
            assert ms_btcusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_btcusdt if x.is_ask == 1])
            assert ms_btcusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_btcusdt if x.is_ask == 0])
            assert ms_btcusdt.order_book.best_ask_price() == 22.1
            assert ms_btcusdt.order_book.best_bid_price() == 21.5
            assert ms_btcusdt.order_book.best_ask_quantity() == 11.0
            assert ms_btcusdt.order_book.best_bid_quantity() == 17.0
            assert ms_btcusdt.order_book.second_ask_price() == 22.5
            assert ms_btcusdt.order_book.second_bid_price() == 21.1
            assert ms_btcusdt.last_timestamp_of_receive == 24
            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_adausdt_timestamp_of_receive = 25
            new_ask_adausdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_adausdt_timestamp_of_receive,
                symbol=Symbol.ADAUSDT,
                is_ask=1,
                price=33.1,
                quantity=0.0,
                is_last=0,
                market=Market.COIN_M_FUTURES
            )
            gms.update(new_ask_adausdt)
            to_remove = None
            for e in sample_adausdt_usd_coin_m_futures_order_list:
                if e.price == new_ask_adausdt.price and e.is_ask == new_ask_adausdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_adausdt_usd_coin_m_futures_order_list.remove(to_remove)

            new_bid_adausdt_timestamp_of_receive = 26
            new_bid_adausdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_adausdt_timestamp_of_receive,
                symbol=Symbol.ADAUSDT,
                is_ask=0,
                price=30.0,
                quantity=0.0,
                is_last=0,
                market=Market.COIN_M_FUTURES
            )
            gms.update(new_bid_adausdt)
            to_remove = None
            for e in sample_adausdt_usd_coin_m_futures_order_list:
                if e.price == new_bid_adausdt.price and e.is_ask == new_bid_adausdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_adausdt_usd_coin_m_futures_order_list.remove(to_remove)

            actual_adausdt = [*reversed(ms_adausdt.order_book.asks()), *ms_adausdt.order_book.bids()]
            expected_adausdt = sorted(sample_adausdt_usd_coin_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_adausdt, expected_adausdt):
                assert a.to_list() == e.to_list()
            del sample_adausdt_usd_coin_m_futures_order_list
            assert ms_adausdt.order_book.ask_count() == 4
            assert ms_adausdt.order_book.bid_count() == 4
            assert ms_adausdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_adausdt if x.is_ask == 1])
            assert ms_adausdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_adausdt if x.is_ask == 0])
            assert ms_adausdt.order_book.best_ask_price() == 32.1
            assert ms_adausdt.order_book.best_bid_price() == 32.0
            assert ms_adausdt.order_book.best_ask_quantity() == 21.0
            assert ms_adausdt.order_book.best_bid_quantity() == 23.0
            assert ms_adausdt.order_book.second_ask_price() == 32.3
            assert ms_adausdt.order_book.second_bid_price() == 31.5
            assert ms_adausdt.last_timestamp_of_receive == 26
            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_dogeusdt_timestamp_of_receive = 27
            new_ask_dogeusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_dogeusdt_timestamp_of_receive,
                symbol=Symbol.DOGEUSDT,
                is_ask=1,
                price=42.1,
                quantity=0.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )
            gms.update(new_ask_dogeusdt)
            to_remove = None
            for e in sample_dogeusdt_usd_m_futures_order_list:
                if e.price == new_ask_dogeusdt.price and e.is_ask == new_ask_dogeusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_dogeusdt_usd_m_futures_order_list.remove(to_remove)

            new_bid_dogeusdt_timestamp_of_receive = 28
            new_bid_dogeusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_dogeusdt_timestamp_of_receive,
                symbol=Symbol.DOGEUSDT,
                is_ask=0,
                price=42.0,
                quantity=0.0,
                is_last=0,
                market=Market.USD_M_FUTURES
            )
            gms.update(new_bid_dogeusdt)
            to_remove = None
            for e in sample_dogeusdt_usd_m_futures_order_list:
                if e.price == new_bid_dogeusdt.price and e.is_ask == new_bid_dogeusdt.is_ask:
                    to_remove = e
                    break
            if to_remove is not None:
                sample_dogeusdt_usd_m_futures_order_list.remove(to_remove)

            actual_dogeusdt = [*reversed(ms_dogeusdt.order_book.asks()), *ms_dogeusdt.order_book.bids()]
            expected_dogeusdt = sorted(sample_dogeusdt_usd_m_futures_order_list, key=lambda x: x.price, reverse=True)
            for a, e in zip(actual_dogeusdt, expected_dogeusdt):
                assert a.to_list() == e.to_list()
            del sample_dogeusdt_usd_m_futures_order_list
            assert ms_dogeusdt.order_book.ask_count() == 4
            assert ms_dogeusdt.order_book.bid_count() == 4
            assert ms_dogeusdt.order_book.sum_ask_quantity() == sum([x.quantity for x in expected_dogeusdt if x.is_ask == 1])
            assert ms_dogeusdt.order_book.sum_bid_quantity() == sum([x.quantity for x in expected_dogeusdt if x.is_ask == 0])
            assert ms_dogeusdt.order_book.best_ask_price() == 42.3
            assert ms_dogeusdt.order_book.best_bid_price() == 41.5
            assert ms_dogeusdt.order_book.best_ask_quantity() == 32.0
            assert ms_dogeusdt.order_book.best_bid_quantity() == 37.0
            assert ms_dogeusdt.order_book.second_ask_price() == 42.5
            assert ms_dogeusdt.order_book.second_bid_price() == 41.1
            assert ms_dogeusdt.last_timestamp_of_receive ==28

            assert gms.get_market_state_count() == 4
            assert sorted(gms.get_market_state_list(), key=lambda x: (int(x[0]), int(x[1]))) == sorted([
                (Symbol.TRXUSDT, Market.SPOT),
                (Symbol.DOGEUSDT, Market.USD_M_FUTURES),
                (Symbol.BTCUSDT, Market.USD_M_FUTURES),
                (Symbol.ADAUSDT, Market.COIN_M_FUTURES),
            ], key=lambda x: (int(x[0]), int(x[1])))

        def test_given_last_trade_storage_when_updating_first_trade_is_last_trade_updated_correctly(self):
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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            trxusdt_trade = TradeEntry(
                timestamp_of_receive=20,
                symbol=Symbol.TRXUSDT,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                symbol=Symbol.BTCUSDT,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                symbol=Symbol.ADAUSDT,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                symbol=Symbol.DOGEUSDT,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            ms_trxusdt = gms.get_market_state(Symbol.TRXUSDT, Market.SPOT)
            ms_btcusdt = gms.get_market_state(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            # TRXUSDT
            assert ms_trxusdt.last_trade.timestamp_of_receive     == 20
            assert ms_trxusdt.last_trade.symbol                   == Symbol.TRXUSDT
            assert ms_trxusdt.last_trade.price                    == 10.0
            assert ms_trxusdt.last_trade.quantity                 == 1.0
            assert ms_trxusdt.last_trade.is_buyer_market_maker    == 1
            assert ms_trxusdt.last_trade.is_last                  == 1
            assert ms_trxusdt.last_trade.market                   == Market.SPOT

            # BTCUSDT
            assert ms_btcusdt.last_trade.timestamp_of_receive     == 21
            assert ms_btcusdt.last_trade.symbol                   == Symbol.BTCUSDT
            assert ms_btcusdt.last_trade.price                    == 11.0
            assert ms_btcusdt.last_trade.quantity                 == 2.0
            assert ms_btcusdt.last_trade.is_buyer_market_maker    == 0
            assert ms_btcusdt.last_trade.is_last                  == 1
            assert ms_btcusdt.last_trade.market                   == Market.USD_M_FUTURES

            # ADAUSDT
            assert ms_adausdt.last_trade.timestamp_of_receive     == 22
            assert ms_adausdt.last_trade.symbol                   == Symbol.ADAUSDT
            assert ms_adausdt.last_trade.price                    == 12.0
            assert ms_adausdt.last_trade.quantity                 == 3.0
            assert ms_adausdt.last_trade.is_buyer_market_maker    == 1
            assert ms_adausdt.last_trade.is_last                  == 1
            assert ms_adausdt.last_trade.market                   == Market.COIN_M_FUTURES

            # DOGEUSDT
            assert ms_dogeusdt.last_trade.timestamp_of_receive    == 23
            assert ms_dogeusdt.last_trade.symbol                  == Symbol.DOGEUSDT
            assert ms_dogeusdt.last_trade.price                   == 13.0
            assert ms_dogeusdt.last_trade.quantity                == 4.0
            assert ms_dogeusdt.last_trade.is_buyer_market_maker   == 0
            assert ms_dogeusdt.last_trade.is_last                 == 1
            assert ms_dogeusdt.last_trade.market                  == Market.USD_M_FUTURES

            assert ms_trxusdt.last_timestamp_of_receive == 20
            assert ms_btcusdt.last_timestamp_of_receive == 21
            assert ms_adausdt.last_timestamp_of_receive == 22
            assert ms_dogeusdt.last_timestamp_of_receive == 23

            assert gms.get_market_state_count() == 4
            assert sorted(gms.get_market_state_list(), key=lambda x: (int(x[0]), int(x[1]))) == sorted([
                (Symbol.TRXUSDT, Market.SPOT),
                (Symbol.DOGEUSDT, Market.USD_M_FUTURES),
                (Symbol.BTCUSDT, Market.USD_M_FUTURES),
                (Symbol.ADAUSDT, Market.COIN_M_FUTURES),
            ], key=lambda x: (int(x[0]), int(x[1])))

        def test_given_last_trade_storage_when_updating_existing_trade_is_last_trade_updated_correctly(self):
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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            trxusdt_trade = TradeEntry(
                timestamp_of_receive=20,
                symbol=Symbol.TRXUSDT,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                symbol=Symbol.BTCUSDT,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                symbol=Symbol.ADAUSDT,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                symbol=Symbol.DOGEUSDT,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            ms_trxusdt = gms.get_market_state(Symbol.TRXUSDT, Market.SPOT)
            ms_btcusdt = gms.get_market_state(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            # TRXUSDT
            assert ms_trxusdt.last_trade.timestamp_of_receive     == 20
            assert ms_trxusdt.last_trade.symbol                   == Symbol.TRXUSDT
            assert ms_trxusdt.last_trade.price                    == 10.0
            assert ms_trxusdt.last_trade.quantity                 == 1.0
            assert ms_trxusdt.last_trade.is_buyer_market_maker    == 1
            assert ms_trxusdt.last_trade.is_last                  == 1
            assert ms_trxusdt.last_trade.market                   == Market.SPOT

            # BTCUSDT
            assert ms_btcusdt.last_trade.timestamp_of_receive     == 21
            assert ms_btcusdt.last_trade.symbol                   == Symbol.BTCUSDT
            assert ms_btcusdt.last_trade.price                    == 11.0
            assert ms_btcusdt.last_trade.quantity                 == 2.0
            assert ms_btcusdt.last_trade.is_buyer_market_maker    == 0
            assert ms_btcusdt.last_trade.is_last                  == 1
            assert ms_btcusdt.last_trade.market                   == Market.USD_M_FUTURES

            # ADAUSDT
            assert ms_adausdt.last_trade.timestamp_of_receive     == 22
            assert ms_adausdt.last_trade.symbol                   == Symbol.ADAUSDT
            assert ms_adausdt.last_trade.price                    == 12.0
            assert ms_adausdt.last_trade.quantity                 == 3.0
            assert ms_adausdt.last_trade.is_buyer_market_maker    == 1
            assert ms_adausdt.last_trade.is_last                  == 1
            assert ms_adausdt.last_trade.market                   == Market.COIN_M_FUTURES

            # DOGEUSDT
            assert ms_dogeusdt.last_trade.timestamp_of_receive    == 23
            assert ms_dogeusdt.last_trade.symbol                  == Symbol.DOGEUSDT
            assert ms_dogeusdt.last_trade.price                   == 13.0
            assert ms_dogeusdt.last_trade.quantity                == 4.0
            assert ms_dogeusdt.last_trade.is_buyer_market_maker   == 0
            assert ms_dogeusdt.last_trade.is_last                 == 1
            assert ms_dogeusdt.last_trade.market                  == Market.USD_M_FUTURES

            assert ms_trxusdt.last_timestamp_of_receive == 20
            assert ms_btcusdt.last_timestamp_of_receive == 21
            assert ms_adausdt.last_timestamp_of_receive == 22
            assert ms_dogeusdt.last_timestamp_of_receive == 23

            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            trxusdt_next_trade = TradeEntry(
                timestamp_of_receive=30,
                symbol=Symbol.TRXUSDT,
                price=20.0,
                quantity=10.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_next_trade)

            btcusdt_next_trade = TradeEntry(
                timestamp_of_receive=31,
                symbol=Symbol.BTCUSDT,
                price=21.0,
                quantity=11.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_next_trade)

            adausdt_next_trade = TradeEntry(
                timestamp_of_receive=32,
                symbol=Symbol.ADAUSDT,
                price=22.0,
                quantity=12.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_next_trade)

            dogeusdt_next_trade = TradeEntry(
                timestamp_of_receive=33,
                symbol=Symbol.DOGEUSDT,
                price=13.0,
                quantity=33.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_next_trade)

            # TRXUSDT
            assert ms_trxusdt.last_trade.timestamp_of_receive     == 30
            assert ms_trxusdt.last_trade.symbol                   == Symbol.TRXUSDT
            assert ms_trxusdt.last_trade.price                    == 20.0
            assert ms_trxusdt.last_trade.quantity                 == 10.0
            assert ms_trxusdt.last_trade.is_buyer_market_maker    == 0
            assert ms_trxusdt.last_trade.is_last                  == 1
            assert ms_trxusdt.last_trade.market                   == Market.SPOT

            # BTCUSDT
            assert ms_btcusdt.last_trade.timestamp_of_receive     == 31
            assert ms_btcusdt.last_trade.symbol                   == Symbol.BTCUSDT
            assert ms_btcusdt.last_trade.price                    == 21.0
            assert ms_btcusdt.last_trade.quantity                 == 11.0
            assert ms_btcusdt.last_trade.is_buyer_market_maker    == 1
            assert ms_btcusdt.last_trade.is_last                  == 1
            assert ms_btcusdt.last_trade.market                   == Market.USD_M_FUTURES

            # ADAUSDT
            assert ms_adausdt.last_trade.timestamp_of_receive     == 32
            assert ms_adausdt.last_trade.symbol                   == Symbol.ADAUSDT
            assert ms_adausdt.last_trade.price                    == 22.0
            assert ms_adausdt.last_trade.quantity                 == 12.0
            assert ms_adausdt.last_trade.is_buyer_market_maker    == 0
            assert ms_adausdt.last_trade.is_last                  == 1
            assert ms_adausdt.last_trade.market                   == Market.COIN_M_FUTURES

            # DOGEUSDT
            assert ms_dogeusdt.last_trade.timestamp_of_receive    == 33
            assert ms_dogeusdt.last_trade.symbol                  == Symbol.DOGEUSDT
            assert ms_dogeusdt.last_trade.price                   == 13.0
            assert ms_dogeusdt.last_trade.quantity                == 33.0
            assert ms_dogeusdt.last_trade.is_buyer_market_maker   == 1
            assert ms_dogeusdt.last_trade.is_last                 == 1
            assert ms_dogeusdt.last_trade.market                  == Market.USD_M_FUTURES

            assert ms_trxusdt.last_timestamp_of_receive == 30
            assert ms_btcusdt.last_timestamp_of_receive == 31
            assert ms_adausdt.last_timestamp_of_receive == 32
            assert ms_dogeusdt.last_timestamp_of_receive == 33

            assert gms.get_market_state_count() == 4
            assert sorted(gms.get_market_state_list(), key=lambda x: (int(x[0]), int(x[1]))) == sorted([
                (Symbol.TRXUSDT, Market.SPOT),
                (Symbol.DOGEUSDT, Market.USD_M_FUTURES),
                (Symbol.BTCUSDT, Market.USD_M_FUTURES),
                (Symbol.ADAUSDT, Market.COIN_M_FUTURES),
            ], key=lambda x: (int(x[0]), int(x[1])))

    class TestGlobalMarketStateCountVariables:

        def test_given_no_market_state_when_count_order_book_metrics_then_returns_none(self):
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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            with pytest.raises(RuntimeError, match="no specified market"):
                x = gms.count_market_state_metrics(Symbol.TRXUSDT, Market.SPOT)

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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            x1 = gms.count_market_state_metrics(Symbol.TRXUSDT, Market.SPOT)
            x2 = gms.count_market_state_metrics(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            x3 = gms.count_market_state_metrics(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            x4 = gms.count_market_state_metrics(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            assert x1 is None
            assert x2 is None
            assert x3 is None
            assert x4 is None
            assert isinstance(x1, NoneType)
            assert isinstance(x2, NoneType)
            assert isinstance(x3, NoneType)
            assert isinstance(x4, NoneType)

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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            trxusdt_trade = TradeEntry(
                timestamp_of_receive=20,
                symbol=Symbol.TRXUSDT,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                symbol=Symbol.BTCUSDT,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                symbol=Symbol.ADAUSDT,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                symbol=Symbol.DOGEUSDT,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            x1 = gms.count_market_state_metrics(Symbol.TRXUSDT, Market.SPOT)
            x2 = gms.count_market_state_metrics(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            x3 = gms.count_market_state_metrics(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            x4 = gms.count_market_state_metrics(Symbol.DOGEUSDT, Market.USD_M_FUTURES)
            assert x1 is None
            assert x2 is None
            assert x3 is None
            assert x4 is None
            assert isinstance(x1, NoneType)
            assert isinstance(x2, NoneType)
            assert isinstance(x3, NoneType)
            assert isinstance(x4, NoneType)

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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                entry = order_list[0]
                gms.update(entry)

            trxusdt_trade = TradeEntry(
                timestamp_of_receive=20,
                symbol=Symbol.TRXUSDT,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                symbol=Symbol.BTCUSDT,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                symbol=Symbol.ADAUSDT,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                symbol=Symbol.DOGEUSDT,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            x1 = gms.count_market_state_metrics(Symbol.TRXUSDT, Market.SPOT)
            x2 = gms.count_market_state_metrics(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            x3 = gms.count_market_state_metrics(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            x4 = gms.count_market_state_metrics(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            assert x1 is None
            assert x2 is None
            assert x3 is None
            assert x4 is None
            assert isinstance(x1, NoneType)
            assert isinstance(x2, NoneType)
            assert isinstance(x3, NoneType)
            assert isinstance(x4, NoneType)

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
            mask = parse_mask(variables)
            gms = GlobalMarketState(mask)

            sample_trxusdt_spot_order_list = sample_order_list(
                symbol=Symbol.TRXUSDT,
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.BTCUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol=Symbol.ADAUSDT,
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol=Symbol.DOGEUSDT,
                market=Market.USD_M_FUTURES,
                price_hash=40.0,
                quantity_hash=30.0
            )
            for order_list in [
                sample_trxusdt_spot_order_list,
                sample_btcusdt_usd_m_futures_order_list,
                sample_adausdt_usd_coin_m_futures_order_list,
                sample_dogeusdt_usd_m_futures_order_list
            ]:
                for entry in order_list:
                    gms.update(entry)

            trxusdt_trade = TradeEntry(
                timestamp_of_receive=20,
                symbol=Symbol.TRXUSDT,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                symbol=Symbol.BTCUSDT,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                symbol=Symbol.ADAUSDT,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                symbol=Symbol.DOGEUSDT,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            ms_trxusdt = gms.get_market_state(Symbol.TRXUSDT, Market.SPOT)
            ms_btcusdt = gms.get_market_state(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            # ms_trxusdt.print_order_book()
            # print(ms_trxusdt.last_trade)
            # print()
            #
            # ms_btcusdt.print_order_book()
            # print(ms_btcusdt.last_trade)
            # print()
            #
            # ms_adausdt.print_order_book()
            # print(ms_adausdt.last_trade)
            # print()
            #
            # ms_dogeusdt.print_order_book()
            # print(ms_dogeusdt.last_trade)
            # print()

            x1: OrderBookMetricsEntry = gms.count_market_state_metrics(Symbol.TRXUSDT, Market.SPOT)
            x2: OrderBookMetricsEntry = gms.count_market_state_metrics(Symbol.BTCUSDT, Market.USD_M_FUTURES)
            x3: OrderBookMetricsEntry = gms.count_market_state_metrics(Symbol.ADAUSDT, Market.COIN_M_FUTURES)
            x4: OrderBookMetricsEntry = gms.count_market_state_metrics(Symbol.DOGEUSDT, Market.USD_M_FUTURES)

            # === x1 (TRXUSDT) ===
            assert x1.timestampOfReceive == 20
            assert x1.bestAskPrice == 12.34589
            assert x1.bestBidPrice == 12.24589
            assert x1.midPrice == 12.29589
            assert x1.bestVolumeImbalance == 0.5
            assert x1.queueImbalance == 0.0
            assert x1.volumeImbalance == 0.45
            assert x1.gap == -0.3
            assert x1.isAggressorAsk == 1

            # === x2 (BTCUSDT) ===
            assert x2.timestampOfReceive == 21
            assert x2.bestAskPrice == 22.1
            assert x2.bestBidPrice == 22.0
            assert x2.midPrice == 22.05
            assert x2.bestVolumeImbalance == 0.08
            assert x2.queueImbalance == 0.0
            assert x2.volumeImbalance == 0.11
            assert x2.gap == -0.3
            assert x2.isAggressorAsk == 0

            # === x3 (ADAUSDT) ===
            assert x3.timestampOfReceive == 22
            assert x3.bestAskPrice == 32.1
            assert x3.bestBidPrice == 32.0
            assert x3.midPrice == 32.05
            assert x3.bestVolumeImbalance == 0.05
            assert x3.queueImbalance == 0.0
            assert x3.volumeImbalance == 0.06
            assert x3.gap == -0.3
            assert x3.isAggressorAsk == 1

            # === x4 (DOGEUSDT) ===
            assert x4.timestampOfReceive == 23
            assert x4.bestAskPrice == 42.1
            assert x4.bestBidPrice == 42.0
            assert x4.midPrice == 42.05
            assert x4.bestVolumeImbalance == 0.03
            assert x4.queueImbalance == 0.0
            assert x4.volumeImbalance == 0.05
            assert x4.gap == -0.3
            assert x4.isAggressorAsk == 0
