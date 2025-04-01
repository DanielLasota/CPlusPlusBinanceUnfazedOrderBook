# Binance Unfazed

C++ OrderBook that You can embed in Your python code!

## installation 
```bash
pip install BinanceCPPOrderbookPrototype
```

```python
import orderbook

def orderbook_callback(best_bid, best_ask, mid_price, orderbook_imbalance):
    # ...
    a, b, c, d = best_bid, best_ask, mid_price, orderbook_imbalance

if __name__ == '__main__':

    csv_path = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_25-03-2025.csv"

    orderbook_session_simulator = orderbook.OrderbookSessionSimulator()
    orderbook_session_simulator.processOrderbook(csv_path, orderbook_callback)
```

![Control-V](https://github.com/user-attachments/assets/a90a5dfc-88c9-4625-8c7e-5456468b6a41)
 
![Control-V (1)](https://github.com/user-attachments/assets/afe41fee-8f34-4493-aabb-be7d6e8f25e7)
