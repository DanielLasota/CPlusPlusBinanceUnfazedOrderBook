from types import NoneType
import pytest

from cpp_binance_orderbook import (
    GlobalMarketState,
    DifferenceDepthEntry,
    parse_mask,
    Market,
    TradeEntry,
    OrderBookMetricsEntry
)

def sample_order_list(
        symbol: str,
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
                stream=f'{symbol.lower()}@depth@100ms',
                event_type='depthUpdate',
                event_time=timestamp_of_receive,
                transaction_time=timestamp_of_receive,
                symbol=symbol,
                first_update_id=timestamp_of_receive,
                final_update_id=timestamp_of_receive,
                final_update_id_in_last_stream=timestamp_of_receive,
                is_ask=is_ask,
                price=price,
                quantity=quantity,
                ps_unknown_field='market',
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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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

            ms_trxusdt = gms.get_market_state("TRXUSDT", Market.SPOT)
            ms_btcusdt = gms.get_market_state("BTCUSDT", Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state("ADAUSDT", Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state("DOGEUSDT", Market.USD_M_FUTURES)

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
            assert gms.get_market_state_list() == [
                ('TRXUSDT', Market.SPOT),
                ('DOGEUSDT', Market.USD_M_FUTURES),
                ('BTCUSDT', Market.USD_M_FUTURES),
                ('ADAUSDT', Market.COIN_M_FUTURES),
            ]

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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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

            ms_trxusdt = gms.get_market_state("TRXUSDT", Market.SPOT)
            ms_btcusdt = gms.get_market_state("BTCUSDT", Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state("ADAUSDT", Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state("DOGEUSDT", Market.USD_M_FUTURES)

            # -------------------------------------------------------- #
            # -------------------------------------------------------- #
            new_ask_trxusdt_timestamp_of_receive = 21
            new_ask_trxusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_trxusdt_timestamp_of_receive,
                stream=f'trxusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_trxusdt_timestamp_of_receive,
                transaction_time=11,
                symbol="TRXUSDT",
                first_update_id=new_ask_trxusdt_timestamp_of_receive,
                final_update_id=new_ask_trxusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_trxusdt_timestamp_of_receive,
                is_ask=1,
                price=12.54589,
                quantity=3.0,
                ps_unknown_field='market',
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
                stream=f'trxusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_trxusdt_timestamp_of_receive,
                transaction_time=new_bid_trxusdt_timestamp_of_receive,
                symbol="TRXUSDT",
                first_update_id=new_bid_trxusdt_timestamp_of_receive,
                final_update_id=new_bid_trxusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_trxusdt_timestamp_of_receive,
                is_ask=0,
                price=10.24589,
                quantity=1.0,
                ps_unknown_field='market',
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
                stream=f'btcusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_btcusdt_timestamp_of_receive,
                transaction_time=new_ask_btcusdt_timestamp_of_receive,
                symbol="BTCUSDT",
                first_update_id=new_ask_btcusdt_timestamp_of_receive,
                final_update_id=new_ask_btcusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_btcusdt_timestamp_of_receive,
                is_ask=1,
                price=22.3,
                quantity=3.0,
                ps_unknown_field='market',
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

            new_bid_trxusdt_timestamp_of_receive = 24
            new_bid_btcusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_bid_trxusdt_timestamp_of_receive,
                stream=f'btcusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_trxusdt_timestamp_of_receive,
                transaction_time=11,
                symbol="BTCUSDT",
                first_update_id=new_bid_trxusdt_timestamp_of_receive,
                final_update_id=new_bid_trxusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_trxusdt_timestamp_of_receive,
                is_ask=0,
                price=22.0,
                quantity=1.0,
                ps_unknown_field='market',
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
                stream=f'adausdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_adausdt_timestamp_of_receive,
                transaction_time=new_ask_adausdt_timestamp_of_receive,
                symbol="ADAUSDT",
                first_update_id=new_ask_adausdt_timestamp_of_receive,
                final_update_id=new_ask_adausdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_adausdt_timestamp_of_receive,
                is_ask=1,
                price=33.1,
                quantity=3.0,
                ps_unknown_field='market',
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
                stream=f'adausdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_adausdt_timestamp_of_receive,
                transaction_time=11,
                symbol="ADAUSDT",
                first_update_id=new_bid_adausdt_timestamp_of_receive,
                final_update_id=new_bid_adausdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_adausdt_timestamp_of_receive,
                is_ask=0,
                price=30.0,
                quantity=1.0,
                ps_unknown_field='market',
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
                stream=f'dogeusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_dogeusdt_timestamp_of_receive,
                transaction_time=11,
                symbol="DOGEUSDT",
                first_update_id=new_ask_dogeusdt_timestamp_of_receive,
                final_update_id=new_ask_dogeusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_dogeusdt_timestamp_of_receive,
                is_ask=1,
                price=42.1,
                quantity=3.0,
                ps_unknown_field='market',
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
                stream=f'dogeusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_dogeusdt_timestamp_of_receive,
                transaction_time=new_bid_dogeusdt_timestamp_of_receive,
                symbol="DOGEUSDT",
                first_update_id=new_bid_dogeusdt_timestamp_of_receive,
                final_update_id=new_bid_dogeusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_dogeusdt_timestamp_of_receive,
                is_ask=0,
                price=42.0,
                quantity=1.0,
                ps_unknown_field='market',
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
            assert gms.get_market_state_list() == [
                ('TRXUSDT', Market.SPOT),
                ('DOGEUSDT', Market.USD_M_FUTURES),
                ('BTCUSDT', Market.USD_M_FUTURES),
                ('ADAUSDT', Market.COIN_M_FUTURES),
            ]

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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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

            ms_trxusdt = gms.get_market_state("TRXUSDT", Market.SPOT)
            ms_btcusdt = gms.get_market_state("BTCUSDT", Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state("ADAUSDT", Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state("DOGEUSDT", Market.USD_M_FUTURES)

            # -------------------------------------------------------- #
            # -------------------------------------------------------- #

            new_ask_trxusdt_timestamp_of_receive = 21
            new_ask_trxusdt = DifferenceDepthEntry(
                timestamp_of_receive=new_ask_trxusdt_timestamp_of_receive,
                stream=f'trxusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_trxusdt_timestamp_of_receive,
                transaction_time=11,
                symbol="TRXUSDT",
                first_update_id=new_ask_trxusdt_timestamp_of_receive,
                final_update_id=new_ask_trxusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_trxusdt_timestamp_of_receive,
                is_ask=1,
                price=12.54589,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'trxusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_trxusdt_timestamp_of_receive,
                transaction_time=new_bid_trxusdt_timestamp_of_receive,
                symbol="TRXUSDT",
                first_update_id=new_bid_trxusdt_timestamp_of_receive,
                final_update_id=new_bid_trxusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_trxusdt_timestamp_of_receive,
                is_ask=0,
                price=10.24589,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'btcusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_btcusdt_timestamp_of_receive,
                transaction_time=new_ask_btcusdt_timestamp_of_receive,
                symbol="BTCUSDT",
                first_update_id=new_ask_btcusdt_timestamp_of_receive,
                final_update_id=new_ask_btcusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_btcusdt_timestamp_of_receive,
                is_ask=1,
                price=22.3,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'btcusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_btcusdt_timestamp_of_receive,
                transaction_time=11,
                symbol="BTCUSDT",
                first_update_id=new_bid_btcusdt_timestamp_of_receive,
                final_update_id=new_bid_btcusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_btcusdt_timestamp_of_receive,
                is_ask=0,
                price=22.0,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'adausdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_adausdt_timestamp_of_receive,
                transaction_time=new_ask_adausdt_timestamp_of_receive,
                symbol="ADAUSDT",
                first_update_id=new_ask_adausdt_timestamp_of_receive,
                final_update_id=new_ask_adausdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_adausdt_timestamp_of_receive,
                is_ask=1,
                price=33.1,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'adausdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_adausdt_timestamp_of_receive,
                transaction_time=11,
                symbol="ADAUSDT",
                first_update_id=new_bid_adausdt_timestamp_of_receive,
                final_update_id=new_bid_adausdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_adausdt_timestamp_of_receive,
                is_ask=0,
                price=30.0,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'dogeusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_ask_dogeusdt_timestamp_of_receive,
                transaction_time=11,
                symbol="DOGEUSDT",
                first_update_id=new_ask_dogeusdt_timestamp_of_receive,
                final_update_id=new_ask_dogeusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_ask_dogeusdt_timestamp_of_receive,
                is_ask=1,
                price=42.1,
                quantity=0.0,
                ps_unknown_field='market',
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
                stream=f'dogeusdt@depth@100ms',
                event_type='depthUpdate',
                event_time=new_bid_dogeusdt_timestamp_of_receive,
                transaction_time=new_bid_dogeusdt_timestamp_of_receive,
                symbol="DOGEUSDT",
                first_update_id=new_bid_dogeusdt_timestamp_of_receive,
                final_update_id=new_bid_dogeusdt_timestamp_of_receive,
                final_update_id_in_last_stream=new_bid_dogeusdt_timestamp_of_receive,
                is_ask=0,
                price=42.0,
                quantity=0.0,
                ps_unknown_field='market',
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
            assert gms.get_market_state_list() == [
                ('TRXUSDT', Market.SPOT),
                ('DOGEUSDT', Market.USD_M_FUTURES),
                ('BTCUSDT', Market.USD_M_FUTURES),
                ('ADAUSDT', Market.COIN_M_FUTURES),
            ]

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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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
                stream='trxusdt@trade',
                event_type='trade',
                event_time=20,
                transaction_time=20,
                symbol='TRXUSDT',
                trade_id=1,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                stream='btcusdt@trade',
                event_type='trade',
                event_time=21,
                transaction_time=21,
                symbol='BTCUSDT',
                trade_id=2,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                stream='adausdt@trade',
                event_type='trade',
                event_time=22,
                transaction_time=22,
                symbol='ADAUSDT',
                trade_id=3,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                stream='dogeusdt@trade',
                event_type='trade',
                event_time=23,
                transaction_time=23,
                symbol='DOGEUSDT',
                trade_id=4,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            ms_trxusdt = gms.get_market_state("TRXUSDT", Market.SPOT)
            ms_btcusdt = gms.get_market_state("BTCUSDT", Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state("ADAUSDT", Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state("DOGEUSDT", Market.USD_M_FUTURES)

            # TRXUSDT
            assert ms_trxusdt.last_trade.timestamp_of_receive     == 20
            assert ms_trxusdt.last_trade.stream                   == 'trxusdt@trade'
            assert ms_trxusdt.last_trade.event_type               == 'trade'
            assert ms_trxusdt.last_trade.event_time               == 20
            assert ms_trxusdt.last_trade.transaction_time         == 20
            assert ms_trxusdt.last_trade.symbol                   == 'TRXUSDT'
            assert ms_trxusdt.last_trade.trade_id                 == 1
            assert ms_trxusdt.last_trade.price                    == 10.0
            assert ms_trxusdt.last_trade.quantity                 == 1.0
            assert ms_trxusdt.last_trade.is_buyer_market_maker    == 1
            assert ms_trxusdt.last_trade.m_unknown_parameter      == 'TRUE'
            assert ms_trxusdt.last_trade.x_unknown_parameter      == 'MARKET'
            assert ms_trxusdt.last_trade.is_last                  == 1
            assert ms_trxusdt.last_trade.market                   == Market.SPOT

            # BTCUSDT
            assert ms_btcusdt.last_trade.timestamp_of_receive     == 21
            assert ms_btcusdt.last_trade.stream                   == 'btcusdt@trade'
            assert ms_btcusdt.last_trade.event_type               == 'trade'
            assert ms_btcusdt.last_trade.event_time               == 21
            assert ms_btcusdt.last_trade.transaction_time         == 21
            assert ms_btcusdt.last_trade.symbol                   == 'BTCUSDT'
            assert ms_btcusdt.last_trade.trade_id                 == 2
            assert ms_btcusdt.last_trade.price                    == 11.0
            assert ms_btcusdt.last_trade.quantity                 == 2.0
            assert ms_btcusdt.last_trade.is_buyer_market_maker    == 0
            assert ms_btcusdt.last_trade.m_unknown_parameter      == 'FALSE'
            assert ms_btcusdt.last_trade.x_unknown_parameter      == ''
            assert ms_btcusdt.last_trade.is_last                  == 1
            assert ms_btcusdt.last_trade.market                   == Market.USD_M_FUTURES

            # ADAUSDT
            assert ms_adausdt.last_trade.timestamp_of_receive     == 22
            assert ms_adausdt.last_trade.stream                   == 'adausdt@trade'
            assert ms_adausdt.last_trade.event_type               == 'trade'
            assert ms_adausdt.last_trade.event_time               == 22
            assert ms_adausdt.last_trade.transaction_time         == 22
            assert ms_adausdt.last_trade.symbol                   == 'ADAUSDT'
            assert ms_adausdt.last_trade.trade_id                 == 3
            assert ms_adausdt.last_trade.price                    == 12.0
            assert ms_adausdt.last_trade.quantity                 == 3.0
            assert ms_adausdt.last_trade.is_buyer_market_maker    == 1
            assert ms_adausdt.last_trade.m_unknown_parameter      == 'TRUE'
            assert ms_adausdt.last_trade.x_unknown_parameter      == 'MARKET'
            assert ms_adausdt.last_trade.is_last                  == 1
            assert ms_adausdt.last_trade.market                   == Market.COIN_M_FUTURES

            # DOGEUSDT
            assert ms_dogeusdt.last_trade.timestamp_of_receive    == 23
            assert ms_dogeusdt.last_trade.stream                  == 'dogeusdt@trade'
            assert ms_dogeusdt.last_trade.event_type              == 'trade'
            assert ms_dogeusdt.last_trade.event_time              == 23
            assert ms_dogeusdt.last_trade.transaction_time        == 23
            assert ms_dogeusdt.last_trade.symbol                  == 'DOGEUSDT'
            assert ms_dogeusdt.last_trade.trade_id                == 4
            assert ms_dogeusdt.last_trade.price                   == 13.0
            assert ms_dogeusdt.last_trade.quantity                == 4.0
            assert ms_dogeusdt.last_trade.is_buyer_market_maker   == 0
            assert ms_dogeusdt.last_trade.m_unknown_parameter     == 'FALSE'
            assert ms_dogeusdt.last_trade.x_unknown_parameter     == ''
            assert ms_dogeusdt.last_trade.is_last                 == 1
            assert ms_dogeusdt.last_trade.market                  == Market.USD_M_FUTURES

            assert ms_trxusdt.last_timestamp_of_receive == 20
            assert ms_btcusdt.last_timestamp_of_receive == 21
            assert ms_adausdt.last_timestamp_of_receive == 22
            assert ms_dogeusdt.last_timestamp_of_receive == 23

            assert gms.get_market_state_count() == 4
            assert gms.get_market_state_list() == [
                ('TRXUSDT', Market.SPOT),
                ('DOGEUSDT', Market.USD_M_FUTURES),
                ('BTCUSDT', Market.USD_M_FUTURES),
                ('ADAUSDT', Market.COIN_M_FUTURES),
            ]

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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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
                stream='trxusdt@trade',
                event_type='trade',
                event_time=20,
                transaction_time=20,
                symbol='TRXUSDT',
                trade_id=1,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                stream='btcusdt@trade',
                event_type='trade',
                event_time=21,
                transaction_time=21,
                symbol='BTCUSDT',
                trade_id=2,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                stream='adausdt@trade',
                event_type='trade',
                event_time=22,
                transaction_time=22,
                symbol='ADAUSDT',
                trade_id=3,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                stream='dogeusdt@trade',
                event_type='trade',
                event_time=23,
                transaction_time=23,
                symbol='DOGEUSDT',
                trade_id=4,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            ms_trxusdt = gms.get_market_state("TRXUSDT", Market.SPOT)
            ms_btcusdt = gms.get_market_state("BTCUSDT", Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state("ADAUSDT", Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state("DOGEUSDT", Market.USD_M_FUTURES)

            # TRXUSDT
            assert ms_trxusdt.last_trade.timestamp_of_receive     == 20
            assert ms_trxusdt.last_trade.stream                   == 'trxusdt@trade'
            assert ms_trxusdt.last_trade.event_type               == 'trade'
            assert ms_trxusdt.last_trade.event_time               == 20
            assert ms_trxusdt.last_trade.transaction_time         == 20
            assert ms_trxusdt.last_trade.symbol                   == 'TRXUSDT'
            assert ms_trxusdt.last_trade.trade_id                 == 1
            assert ms_trxusdt.last_trade.price                    == 10.0
            assert ms_trxusdt.last_trade.quantity                 == 1.0
            assert ms_trxusdt.last_trade.is_buyer_market_maker    == 1
            assert ms_trxusdt.last_trade.m_unknown_parameter      == 'TRUE'
            assert ms_trxusdt.last_trade.x_unknown_parameter      == 'MARKET'
            assert ms_trxusdt.last_trade.is_last                  == 1
            assert ms_trxusdt.last_trade.market                   == Market.SPOT

            # BTCUSDT
            assert ms_btcusdt.last_trade.timestamp_of_receive     == 21
            assert ms_btcusdt.last_trade.stream                   == 'btcusdt@trade'
            assert ms_btcusdt.last_trade.event_type               == 'trade'
            assert ms_btcusdt.last_trade.event_time               == 21
            assert ms_btcusdt.last_trade.transaction_time         == 21
            assert ms_btcusdt.last_trade.symbol                   == 'BTCUSDT'
            assert ms_btcusdt.last_trade.trade_id                 == 2
            assert ms_btcusdt.last_trade.price                    == 11.0
            assert ms_btcusdt.last_trade.quantity                 == 2.0
            assert ms_btcusdt.last_trade.is_buyer_market_maker    == 0
            assert ms_btcusdt.last_trade.m_unknown_parameter      == 'FALSE'
            assert ms_btcusdt.last_trade.x_unknown_parameter      == ''
            assert ms_btcusdt.last_trade.is_last                  == 1
            assert ms_btcusdt.last_trade.market                   == Market.USD_M_FUTURES

            # ADAUSDT
            assert ms_adausdt.last_trade.timestamp_of_receive     == 22
            assert ms_adausdt.last_trade.stream                   == 'adausdt@trade'
            assert ms_adausdt.last_trade.event_type               == 'trade'
            assert ms_adausdt.last_trade.event_time               == 22
            assert ms_adausdt.last_trade.transaction_time         == 22
            assert ms_adausdt.last_trade.symbol                   == 'ADAUSDT'
            assert ms_adausdt.last_trade.trade_id                 == 3
            assert ms_adausdt.last_trade.price                    == 12.0
            assert ms_adausdt.last_trade.quantity                 == 3.0
            assert ms_adausdt.last_trade.is_buyer_market_maker    == 1
            assert ms_adausdt.last_trade.m_unknown_parameter      == 'TRUE'
            assert ms_adausdt.last_trade.x_unknown_parameter      == 'MARKET'
            assert ms_adausdt.last_trade.is_last                  == 1
            assert ms_adausdt.last_trade.market                   == Market.COIN_M_FUTURES

            # DOGEUSDT
            assert ms_dogeusdt.last_trade.timestamp_of_receive    == 23
            assert ms_dogeusdt.last_trade.stream                  == 'dogeusdt@trade'
            assert ms_dogeusdt.last_trade.event_type              == 'trade'
            assert ms_dogeusdt.last_trade.event_time              == 23
            assert ms_dogeusdt.last_trade.transaction_time        == 23
            assert ms_dogeusdt.last_trade.symbol                  == 'DOGEUSDT'
            assert ms_dogeusdt.last_trade.trade_id                == 4
            assert ms_dogeusdt.last_trade.price                   == 13.0
            assert ms_dogeusdt.last_trade.quantity                == 4.0
            assert ms_dogeusdt.last_trade.is_buyer_market_maker   == 0
            assert ms_dogeusdt.last_trade.m_unknown_parameter     == 'FALSE'
            assert ms_dogeusdt.last_trade.x_unknown_parameter     == ''
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
                stream='trxusdt@trade',
                event_type='trade',
                event_time=30,
                transaction_time=30,
                symbol='TRXUSDT',
                trade_id=5,
                price=20.0,
                quantity=10.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_next_trade)

            btcusdt_next_trade = TradeEntry(
                timestamp_of_receive=31,
                stream='btcusdt@trade',
                event_type='trade',
                event_time=31,
                transaction_time=31,
                symbol='BTCUSDT',
                trade_id=6,
                price=21.0,
                quantity=11.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_next_trade)

            adausdt_next_trade = TradeEntry(
                timestamp_of_receive=32,
                stream='adausdt@trade',
                event_type='trade',
                event_time=32,
                transaction_time=32,
                symbol='ADAUSDT',
                trade_id=7,
                price=22.0,
                quantity=12.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_next_trade)

            dogeusdt_next_trade = TradeEntry(
                timestamp_of_receive=33,
                stream='dogeusdt@trade',
                event_type='trade',
                event_time=33,
                transaction_time=33,
                symbol='DOGEUSDT',
                trade_id=8,
                price=13.0,
                quantity=33.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_next_trade)

            # TRXUSDT
            assert ms_trxusdt.last_trade.timestamp_of_receive     == 30
            assert ms_trxusdt.last_trade.trade_id                 == 5
            assert ms_trxusdt.last_trade.price                    == 20.0
            assert ms_trxusdt.last_trade.quantity                 == 10.0
            assert ms_trxusdt.last_trade.is_buyer_market_maker    == 0
            assert ms_trxusdt.last_trade.stream                   == 'trxusdt@trade'
            assert ms_trxusdt.last_trade.event_type               == 'trade'
            assert ms_trxusdt.last_trade.event_time               == 30
            assert ms_trxusdt.last_trade.transaction_time         == 30
            assert ms_trxusdt.last_trade.m_unknown_parameter      == 'TRUE'
            assert ms_trxusdt.last_trade.x_unknown_parameter      == 'MARKET'
            assert ms_trxusdt.last_trade.is_last                  == 1
            assert ms_trxusdt.last_trade.market                   == Market.SPOT

            # BTCUSDT
            assert ms_btcusdt.last_trade.timestamp_of_receive     == 31
            assert ms_btcusdt.last_trade.trade_id                 == 6
            assert ms_btcusdt.last_trade.price                    == 21.0
            assert ms_btcusdt.last_trade.quantity                 == 11.0
            assert ms_btcusdt.last_trade.is_buyer_market_maker    == 1
            assert ms_btcusdt.last_trade.stream                   == 'btcusdt@trade'
            assert ms_btcusdt.last_trade.event_type               == 'trade'
            assert ms_btcusdt.last_trade.event_time               == 31
            assert ms_btcusdt.last_trade.transaction_time         == 31
            assert ms_btcusdt.last_trade.m_unknown_parameter      == 'FALSE'
            assert ms_btcusdt.last_trade.x_unknown_parameter      == ''
            assert ms_btcusdt.last_trade.is_last                  == 1
            assert ms_btcusdt.last_trade.market                   == Market.USD_M_FUTURES

            # ADAUSDT
            assert ms_adausdt.last_trade.timestamp_of_receive     == 32
            assert ms_adausdt.last_trade.trade_id                 == 7
            assert ms_adausdt.last_trade.price                    == 22.0
            assert ms_adausdt.last_trade.quantity                 == 12.0
            assert ms_adausdt.last_trade.is_buyer_market_maker    == 0
            assert ms_adausdt.last_trade.stream                   == 'adausdt@trade'
            assert ms_adausdt.last_trade.event_type               == 'trade'
            assert ms_adausdt.last_trade.event_time               == 32
            assert ms_adausdt.last_trade.transaction_time         == 32
            assert ms_adausdt.last_trade.m_unknown_parameter      == 'TRUE'
            assert ms_adausdt.last_trade.x_unknown_parameter      == 'MARKET'
            assert ms_adausdt.last_trade.is_last                  == 1
            assert ms_adausdt.last_trade.market                   == Market.COIN_M_FUTURES

            # DOGEUSDT
            assert ms_dogeusdt.last_trade.timestamp_of_receive    == 33
            assert ms_dogeusdt.last_trade.trade_id                == 8
            assert ms_dogeusdt.last_trade.price                   == 13.0
            assert ms_dogeusdt.last_trade.quantity                == 33.0
            assert ms_dogeusdt.last_trade.is_buyer_market_maker   == 1
            assert ms_dogeusdt.last_trade.stream                  == 'dogeusdt@trade'
            assert ms_dogeusdt.last_trade.event_type              == 'trade'
            assert ms_dogeusdt.last_trade.event_time              == 33
            assert ms_dogeusdt.last_trade.transaction_time        == 33
            assert ms_dogeusdt.last_trade.m_unknown_parameter     == 'FALSE'
            assert ms_dogeusdt.last_trade.x_unknown_parameter     == ''
            assert ms_dogeusdt.last_trade.is_last                 == 1
            assert ms_dogeusdt.last_trade.market                  == Market.USD_M_FUTURES

            assert ms_trxusdt.last_timestamp_of_receive == 30
            assert ms_btcusdt.last_timestamp_of_receive == 31
            assert ms_adausdt.last_timestamp_of_receive == 32
            assert ms_dogeusdt.last_timestamp_of_receive == 33

            assert gms.get_market_state_count() == 4
            assert gms.get_market_state_list() == [
                ('TRXUSDT', Market.SPOT),
                ('DOGEUSDT', Market.USD_M_FUTURES),
                ('BTCUSDT', Market.USD_M_FUTURES),
                ('ADAUSDT', Market.COIN_M_FUTURES),
            ]

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
                x = gms.count_market_state_metrics("TRXUSDT", Market.SPOT)

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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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

            x1 = gms.count_market_state_metrics("TRXUSDT", Market.SPOT)
            x2 = gms.count_market_state_metrics("BTCUSDT", Market.USD_M_FUTURES)
            x3 = gms.count_market_state_metrics("ADAUSDT", Market.COIN_M_FUTURES)
            x4 = gms.count_market_state_metrics("DOGEUSDT", Market.USD_M_FUTURES)

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
                stream='trxusdt@trade',
                event_type='trade',
                event_time=20,
                transaction_time=20,
                symbol='TRXUSDT',
                trade_id=1,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                stream='btcusdt@trade',
                event_type='trade',
                event_time=21,
                transaction_time=21,
                symbol='BTCUSDT',
                trade_id=2,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                stream='adausdt@trade',
                event_type='trade',
                event_time=22,
                transaction_time=22,
                symbol='ADAUSDT',
                trade_id=3,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                stream='dogeusdt@trade',
                event_type='trade',
                event_time=23,
                transaction_time=23,
                symbol='DOGEUSDT',
                trade_id=4,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            x1 = gms.count_market_state_metrics("TRXUSDT", Market.SPOT)
            x2 = gms.count_market_state_metrics("BTCUSDT", Market.USD_M_FUTURES)
            x3 = gms.count_market_state_metrics("ADAUSDT", Market.COIN_M_FUTURES)
            x4 = gms.count_market_state_metrics("DOGEUSDT", Market.USD_M_FUTURES)
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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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
                stream='trxusdt@trade',
                event_type='trade',
                event_time=20,
                transaction_time=20,
                symbol='TRXUSDT',
                trade_id=1,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                stream='btcusdt@trade',
                event_type='trade',
                event_time=21,
                transaction_time=21,
                symbol='BTCUSDT',
                trade_id=2,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                stream='adausdt@trade',
                event_type='trade',
                event_time=22,
                transaction_time=22,
                symbol='ADAUSDT',
                trade_id=3,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                stream='dogeusdt@trade',
                event_type='trade',
                event_time=23,
                transaction_time=23,
                symbol='DOGEUSDT',
                trade_id=4,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            x1 = gms.count_market_state_metrics("TRXUSDT", Market.SPOT)
            x2 = gms.count_market_state_metrics("BTCUSDT", Market.USD_M_FUTURES)
            x3 = gms.count_market_state_metrics("ADAUSDT", Market.COIN_M_FUTURES)
            x4 = gms.count_market_state_metrics("DOGEUSDT", Market.USD_M_FUTURES)

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
                symbol='TRXUSDT',
                market=Market.SPOT,
                price_hash=10.24589,
                quantity_hash=0.0
            )
            sample_btcusdt_usd_m_futures_order_list = sample_order_list(
                symbol='BTCUSDT',
                market=Market.USD_M_FUTURES,
                price_hash=20.0,
                quantity_hash=10.0
            )
            sample_adausdt_usd_coin_m_futures_order_list = sample_order_list(
                symbol='ADAUSDT',
                market=Market.COIN_M_FUTURES,
                price_hash=30.0,
                quantity_hash=20.0
            )
            sample_dogeusdt_usd_m_futures_order_list = sample_order_list(
                symbol='DOGEUSDT',
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
                stream='trxusdt@trade',
                event_type='trade',
                event_time=20,
                transaction_time=20,
                symbol='TRXUSDT',
                trade_id=1,
                price=10.0,
                quantity=1.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.SPOT
            )
            gms.update(trxusdt_trade)

            btcusdt_trade = TradeEntry(
                timestamp_of_receive=21,
                stream='btcusdt@trade',
                event_type='trade',
                event_time=21,
                transaction_time=21,
                symbol='BTCUSDT',
                trade_id=2,
                price=11.0,
                quantity=2.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(btcusdt_trade)

            adausdt_trade = TradeEntry(
                timestamp_of_receive=22,
                stream='adausdt@trade',
                event_type='trade',
                event_time=22,
                transaction_time=22,
                symbol='ADAUSDT',
                trade_id=3,
                price=12.0,
                quantity=3.0,
                is_buyer_market_maker=1,
                m_unknown_parameter='TRUE',
                x_unknown_parameter='MARKET',
                is_last=1,
                market=Market.COIN_M_FUTURES
            )
            gms.update(adausdt_trade)

            dogeusdt_trade = TradeEntry(
                timestamp_of_receive=23,
                stream='dogeusdt@trade',
                event_type='trade',
                event_time=23,
                transaction_time=23,
                symbol='DOGEUSDT',
                trade_id=4,
                price=13.0,
                quantity=4.0,
                is_buyer_market_maker=0,
                m_unknown_parameter='FALSE',
                x_unknown_parameter='',
                is_last=1,
                market=Market.USD_M_FUTURES
            )
            gms.update(dogeusdt_trade)

            ms_trxusdt = gms.get_market_state("TRXUSDT", Market.SPOT)
            ms_btcusdt = gms.get_market_state("BTCUSDT", Market.USD_M_FUTURES)
            ms_adausdt = gms.get_market_state("ADAUSDT", Market.COIN_M_FUTURES)
            ms_dogeusdt = gms.get_market_state("DOGEUSDT", Market.USD_M_FUTURES)

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

            x1: OrderBookMetricsEntry = gms.count_market_state_metrics("TRXUSDT", Market.SPOT)
            x2: OrderBookMetricsEntry = gms.count_market_state_metrics("BTCUSDT", Market.USD_M_FUTURES)
            x3: OrderBookMetricsEntry = gms.count_market_state_metrics("ADAUSDT", Market.COIN_M_FUTURES)
            x4: OrderBookMetricsEntry = gms.count_market_state_metrics("DOGEUSDT", Market.USD_M_FUTURES)

            # === x1 (TRXUSDT) ===
            assert x1.timestampOfReceive == 20
            assert x1.bestAskPrice == 12.34589
            assert x1.bestBidPrice == 12.24589
            assert x1.midPrice == 12.29589
            assert round(x1.bestVolumeImbalance, 2) == 0.5
            assert round(x1.queueImbalance, 2) == 0.0
            assert round(x1.volumeImbalance, 2) == 0.45
            assert round(x1.gap, 2) == -0.3
            assert x1.isAggressorAsk == 1

            # === x2 (BTCUSDT) ===
            assert x2.timestampOfReceive == 21
            assert x2.bestAskPrice == 22.1
            assert x2.bestBidPrice == 22.0
            assert x2.midPrice == 22.05
            assert round(x2.bestVolumeImbalance, 2) == 0.08
            assert round(x2.queueImbalance, 2) == 0.0
            assert round(x2.volumeImbalance, 2) == 0.11
            assert round(x2.gap, 2) == -0.3
            assert x2.isAggressorAsk == 0

            # === x3 (ADAUSDT) ===
            assert x3.timestampOfReceive == 22
            assert x3.bestAskPrice == 32.1
            assert x3.bestBidPrice == 32.0
            assert x3.midPrice == 32.05
            assert round(x3.bestVolumeImbalance, 2) == 0.05
            assert round(x3.queueImbalance, 2) == 0.0
            assert round(x3.volumeImbalance, 2) == 0.06
            assert round(x3.gap, 2) == -0.3
            assert x3.isAggressorAsk == 1

            # === x4 (DOGEUSDT) ===
            assert x4.timestampOfReceive == 23
            assert x4.bestAskPrice == 42.1
            assert x4.bestBidPrice == 42.0
            assert x4.midPrice == 42.05
            assert round(x4.bestVolumeImbalance, 2) == 0.03
            assert round(x4.queueImbalance, 2) == 0.0
            assert round(x4.volumeImbalance, 2) == 0.05
            assert round(x4.gap, 2) == -0.3
            assert x4.isAggressorAsk == 0
