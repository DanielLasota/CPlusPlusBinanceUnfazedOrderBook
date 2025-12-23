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

## Tutorial:

### Add feature

```text
    include/detail/metrics_list.def
    
    SingleVariableCounter.h
    SingleVariableCounter.cpp
    
    OrderBookMetricsCalculator.cpp
```

### install python package with intel c++ compiler
```text
    1. install intel c++
    
    2. set system env variables:
        ################################################################################
        # Env Variables to be set for intel C++ Compiler:
        #   System:
        #       path =       C:\Program Files (x86)\Intel\oneAPI\compiler\2025.3\bin
        #       LIB =        C:\Program Files (x86)\Intel\oneAPI\compiler\2025.3\lib
        #       INCLUDE =    C:\Program Files (x86)\Intel\oneAPI\compiler\2025.3\include
        ################################################################################
        
    3. (.venv) PS C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook> .\python_build_intel.bat   
    
    4. (.venv) PS C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook> pip uninstall cpp_binance_orderbook 
    
    5. (.venv) PS C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook> pip install --upgrade C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\dist\cpp_binance_orderbook-0.0.3-cp311-cp311-win_amd64.whl

```