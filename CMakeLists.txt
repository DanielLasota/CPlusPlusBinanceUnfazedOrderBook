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
        src/FinalOrderBookSnapshot.cpp
        src/CSVHeader.cpp
        src/OrderBookMetrics.cpp
#        test/TestSingleVariableCounter.cpp
#        test/TestOrderBook.cpp
)

pybind11_add_module(cpp_binance_orderbook ${SOURCES})

set_target_properties(cpp_binance_orderbook PROPERTIES SUFFIX ".pyd")
