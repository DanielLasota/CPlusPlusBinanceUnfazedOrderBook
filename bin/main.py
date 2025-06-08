import sys
import os
import ctypes
sys.path.insert(0, "../../cmake-build-release")
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


def orderbook_callback(x):
    ...


if __name__ == '__main__':

    orderbook_session_simulator = cpp_binance_orderbook.OrderBookSessionSimulator()
    print(os.getcwd())
    csv_path = "./sample_csvs/binance_merged_depth_snapshot_difference_depth_stream_trade_stream_usd_m_futures_trxusdt_14-04-2025.csv"

    variables = [
        "timestampOfReceive",
        "market",
        "symbol",
        "bestAskPrice",
        "bestBidPrice",
        "midPrice",
        "bestVolumeImbalance",
        "queueImbalance",
        "volumeImbalance",
        "gap",
        "isAggressorAsk"
    ]

    variables = orderbook_snapshot = orderbook_session_simulator.compute_variables(csv_path=csv_path, variables=variables)

