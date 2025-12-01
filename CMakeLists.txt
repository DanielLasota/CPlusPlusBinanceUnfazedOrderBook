################################################################################
# Env Variables to be set for intel C++ Compiler:
#   System:
#       path: C:\Program Files (x86)\Intel\oneAPI\compiler\2025.3\bin
#       LIB: C:\Program Files (x86)\Intel\oneAPI\compiler\2025.3\lib
#       INCLUDE: C:\Program Files (x86)\Intel\oneAPI\compiler\2025.3\include
################################################################################
cmake_minimum_required(VERSION 3.20)
project(CPlusPlusBinanceUnfazedOrderBook LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
add_subdirectory(pybind11)

set(FLAGS_INTEL
        /O3
        /Qxhost
        /fp:fast
)
set(FLAGS_INTEL_NUCLEAR
        /O3                               # max optimization level (aggressive speed)
        /Qxhost                           # optimize for the exact CPU (i9-13980HX)
        /fp:fast                          # fast-math: unsafe but fastest FP optimizations
        /Qimf-arch-consistency:false      # use fastest CPU-specific math functions (exp/log/sin)
        /Qfast_transcendentals            # ultra-fast approximated transcendental functions
        /Qansi-alias                      # strict aliasing assumptions (more aggressive opts)
        /Qunroll                          # aggressively unroll loops for speed
        /Qftz                             # flush denormals to zero (FTZ/DAZ) for faster FP
        /Qopt-zmm-usage:high              # prefer wide vector instructions aggressively
        /Qopt-prefetch=2                  # aggressive prefetching (may help or hurt)
        /Qopt-streaming-stores=always     # use non-temporal stores (bypass cache)
        /Qvec-threshold:0                 # force vectorization even on small loops
        /Qoverride-limits                 # allow compiler to break internal safety limits
)

# =========================
# 1) CORE LIB (orderbook_core)
# =========================

set(SRC_CORE
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
        src/GlobalMarketState.cpp
        src/TradeRoll.cpp
        src/DepthRoll.cpp
)
add_library(orderbook_core STATIC ${SRC_CORE})
target_include_directories(orderbook_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/pybind11/include
        ${Python3_INCLUDE_DIRS}
)
target_compile_options(orderbook_core PRIVATE ${FLAGS_INTEL})

# =========================
# 2) EXECUTABLE (main)
# =========================

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE orderbook_core Python3::Python)
target_compile_options(main PRIVATE ${FLAGS_INTEL})
#target_compile_definitions(main PRIVATE BUILD_FLAGS="${FLAGS_INTEL}")

# =========================
# 3) PYBIND11 MODULE (pybind11_module)
# =========================

pybind11_add_module(pybind11_module bindings/orderbook_module.cpp)
target_link_libraries(pybind11_module PRIVATE orderbook_core Python3::Python)
target_include_directories(pybind11_module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(pybind11_module PRIVATE ${FLAGS_INTEL})
