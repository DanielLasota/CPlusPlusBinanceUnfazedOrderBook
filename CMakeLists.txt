cmake_minimum_required(VERSION 3.30)

project(orderbook_prototype)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(pybind11)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
        src/main.cpp
        src/CSVParser.cpp
        src/OrderBook.cpp
)

pybind11_add_module(orderbook ${SOURCES})

set_target_properties(orderbook PROPERTIES SUFFIX ".pyd")
