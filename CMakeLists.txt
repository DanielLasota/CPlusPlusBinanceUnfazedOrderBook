cmake_minimum_required(VERSION 3.30)

project(orderbook_prototype)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(pybind11)

pybind11_add_module(orderbook main.cpp)

#add_executable(testest main.cpp)

set_target_properties(orderbook PROPERTIES SUFFIX ".pyd")
