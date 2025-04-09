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
        src/VariablesCounter.cpp
        src/SingleVariableCounter.cpp
        src/DataVectorLoader.cpp
        src/AssetParameters.cpp
        src/EntryDecoder.cpp
#        test/TestSingleVariableCounter.cpp
)

pybind11_add_module(orderbook ${SOURCES})

set_target_properties(orderbook PROPERTIES SUFFIX ".pyd")
