cmake_minimum_required(VERSION 3.30)

project(CPlusPlusBinanceUnfazedOrderBook)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(pybind11)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/enums
)

set(SOURCES
        bindings/orderbook_module.cpp
        src/main.cpp
        src/OrderBook.cpp
        src/OrderBookSessionSimulator.cpp
        src/MarketState.cpp
        src/SingleVariableCounter.cpp
        src/DataVectorLoader.cpp
        src/AssetParameters.cpp
        src/EntryDecoder.cpp
        src/CSVHeader.cpp
        src/OrderBookMetrics.cpp
        src/OrderBookMetricsCalculator.cpp
#        test/TestSingleVariableCounter.cpp
#        test/TestOrderBook.cpp
)

pybind11_add_module(cpp_binance_orderbook ${SOURCES})

set_target_properties(cpp_binance_orderbook PROPERTIES SUFFIX ".pyd")

#-std=c++20
#-O3
#-march=native
#-pthread
#-IC:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\pybind11\include
#-IC:\Users\daniel\AppData\Local\Programs\Python\Python311\include
#-LC:\Users\daniel\AppData\Local\Programs\Python\Python311\libs
#-IC:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\include
#-lpython311
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\AssetParameters.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\DataVectorLoader.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\EntryDecoder.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\OrderBook.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\OrderBookSessionSimulator.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\SingleVariableCounter.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\MarketState.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\CSVHeader.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\OrderBookMetrics.cpp
#C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook\src\OrderBookMetricsCalculator.cpp