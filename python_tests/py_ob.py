from dataclasses import dataclass
import bisect
import time


@dataclass
class DifferenceDepthEntry:
    symbol: str
    price: float
    quantity: float
    is_ask: bool

class OrderBook:
    def __init__(self):
        # asks sorted by price ascending, bids sorted by price descending
        self.asks: list[DifferenceDepthEntry] = []
        self.bids: list[DifferenceDepthEntry] = []

    def add_order(self, order: DifferenceDepthEntry):
        if order.is_ask:
            # find insert/update/remove index in asks
            prices = [o.price for o in self.asks]
            idx = bisect.bisect_left(prices, order.price)
            if order.quantity == 0:
                # remove if exists
                if idx < len(self.asks) and self.asks[idx].price == order.price:
                    self.asks.pop(idx)
            else:
                # update existing or insert
                if idx < len(self.asks) and self.asks[idx].price == order.price:
                    self.asks[idx] = order
                else:
                    self.asks.insert(idx, order)
        else:
            # bids: keep sorted descending (use negative prices for bisect)
            neg_prices = [-o.price for o in self.bids]
            idx = bisect.bisect_left(neg_prices, -order.price)
            if order.quantity == 0:
                if idx < len(self.bids) and self.bids[idx].price == order.price:
                    self.bids.pop(idx)
            else:
                if idx < len(self.bids) and self.bids[idx].price == order.price:
                    self.bids[idx] = order
                else:
                    self.bids.insert(idx, order)

    def print_order_book(self):
        print("ORDERBOOK:")
        # Asks in reverse (highest first)
        print("\033[31mAsks (odwrotnie):\033[0m")
        for ask in reversed(self.asks):
            print(
                f"\033[31m"
                f"SYMBOL: {ask.symbol} Price: {ask.price} "
                f"Quantity: {ask.quantity} IsAsk: {ask.is_ask}"
                f"\033[0m"
            )

        # Bids normal (highest first)
        print("\033[32mBids:\033[0m")
        for bid in self.bids:
            print(
                f"\033[32m"
                f"SYMBOL: {bid.symbol} Price: {bid.price} "
                f"Quantity: {bid.quantity} IsAsk: {bid.is_ask}"
                f"\033[0m"
            )
        print()

if __name__ == "__main__":
    ob = OrderBook()
    start = time.perf_counter()
    ob.add_order(DifferenceDepthEntry("AAPL", 12.0, 5, is_ask=True))
    ob.add_order(DifferenceDepthEntry("AAPL", 11.0, 6, is_ask=True))
    ob.add_order(DifferenceDepthEntry("AAPL", 9, 5, is_ask=False))
    ob.add_order(DifferenceDepthEntry("AAPL", 10.0, 5, is_ask=False))
    elapsed = (time.perf_counter() - start) * 1e6
    print(f"\n🏷️  Czas wstawienia 4 zleceń: {elapsed:.2f} µs")
    ob.print_order_book()
