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

import orderbook
print(orderbook.__file__)

if __name__ == '__main__':
    print(f'VVVVV')
    print(dir(orderbook))
    for attribute in dir(orderbook):
        print(attribute)

    csv_path = "C:/Users/daniel/Documents/binance_archival_data/binance_difference_depth_stream_usd_m_futures_trxusdt_25-03-2025.csv"
    orderbook.processOrderbook(csv_path)

    print(f'^^^^^')
