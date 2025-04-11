# Binance Unfazed

C++ Binance OrderBook that You can embed in Your python code!
Simulate whole market day of 3,389,470 OrderBookEntry in 109ms*

*Ran on i9-13980hx

## installation 
```bash
pip install cpp_binance_orderbook
```

```python
import cpp_binance_orderbook

def orderbook_callback(best_bid, best_ask, mid_price, orderbook_imbalance):
    # ...
    a, b, c, d = best_bid, best_ask, mid_price, orderbook_imbalance

if __name__ == '__main__':

    csv_path = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_25-03-2025.csv"

    orderbook_session_simulator = cpp_binance_orderbook.OrderbookSessionSimulator()
    orderbook_session_simulator.processOrderbook(csv_path, orderbook_callback)
```

![Control-V](https://github.com/user-attachments/assets/a90a5dfc-88c9-4625-8c7e-5456468b6a41)
 
![Control-V (1)](https://github.com/user-attachments/assets/afe41fee-8f34-4493-aabb-be7d6e8f25e7)
