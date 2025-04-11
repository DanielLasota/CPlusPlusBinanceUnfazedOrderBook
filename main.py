import sys
import os
import ctypes
sys.path.insert(0, "./cmake-build-release")
os.add_dll_directory(os.path.dirname(__file__))

def check_dll(dll_name):
    try:
        lib = ctypes.CDLL(dll_name)
        # print(f"{dll_name} załadowany poprawnie.")
    except Exception as e:
        print(f"Nie udało się załadować {dll_name}: {e}")

dll_list = ["libwinpthread-1.dll", "python311.dll", "msvcrt.dll", "kernel32.dll"]

for dll in dll_list:
    check_dll(dll)

import cpp_binance_orderbook
print(cpp_binance_orderbook.__file__)
print(f'VVVVV')
print(dir(cpp_binance_orderbook))
for attribute in dir(cpp_binance_orderbook):
    print(attribute)

def orderbook_callback(best_bid, best_ask, mid_price, orderbook_imbalance):
    # ...
    a, b, c, d = best_bid, best_ask, mid_price, orderbook_imbalance


if __name__ == '__main__':

    orderbook_session_simulator = cpp_binance_orderbook.OrderbookSessionSimulator()

    csv_path = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_10-04-2025.csv"

    # orderbook_session_simulator.processOrderbook(csv_path, orderbook_callback)
    orderbook_snapshot = orderbook_session_simulator.getFinalOrderBookSnapshot(csv_path)

    for bid in orderbook_snapshot.bids:
        print(bid)
