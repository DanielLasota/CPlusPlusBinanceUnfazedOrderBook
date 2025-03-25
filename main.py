import sys
# import platform
import os
# print(sys.path)
# print(platform.architecture())
sys.path.insert(0, os.path.abspath(os.path.dirname(__file__)))
# print(os.environ["PATH"])
# import ctypes
# import os

os.add_dll_directory(os.path.dirname(__file__))
# def check_dll(dll_name):
#     try:
#         lib = ctypes.CDLL(dll_name)
#         # print(f"{dll_name} załadowany poprawnie.")
#     except Exception as e:
#         print(f"Nie udało się załadować {dll_name}: {e}")
#
# dll_list = ["libwinpthread-1.dll", "python311.dll", "msvcrt.dll", "kernel32.dll"]
#
# for dll in dll_list:
#     check_dll(dll)

import orderbook

if __name__ == '__main__':
    print(f'VVVVV')
    print(orderbook.get_first_symbol())
    print(f'^^^^^')
