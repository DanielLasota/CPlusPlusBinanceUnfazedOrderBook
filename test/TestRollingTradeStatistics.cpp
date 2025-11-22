#include <cassert>
#include <chrono>
#include <array>
#include <iostream>
#include "../include/RollingTradeStatistics.h"

constexpr size_t M = 1024;

namespace {
    const std::array<TradeEntry, 1> oneTradeArray = {{
        {1760400002000000ULL, Symbol::BTCUSDT, 115100.0, 0.001, false, true, Market::SPOT},
    }};

    const std::array<TradeEntry, 8> baseTradeArray = {{
        {1760400002000000ULL, Symbol::BTCUSDT, 115100.0, 0.001, false, true, Market::SPOT}, // 6
        {1760400002000001ULL, Symbol::BTCUSDT, 115101.0, 0.001, false, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, 115102.0, 0.001, false, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, 115103.0, 0.002, true,  true,  Market::SPOT}, // 5
        {1760400004000000ULL, Symbol::BTCUSDT, 115104.0, 0.001, false, true,  Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, 115120.0, 0.0015, false, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, 115121.0, 0.0008, true,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, 115118.0, 0.002, false, true,  Market::SPOT}, // 1
    }};

    const std::array<TradeEntry, 13> baseTradeArrayDuringRolling = {{
        {1760400002000000ULL, Symbol::BTCUSDT, 115100.0, 0.001, false, true, Market::SPOT},
        {1760400002000001ULL, Symbol::BTCUSDT, 115101.0, 0.001, false, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, 115102.0, 0.001, false, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, 115103.0, 0.002, true,  true,  Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, 115104.0, 0.001, false, true,  Market::SPOT}, // 8
        {1760400005000000ULL, Symbol::BTCUSDT, 115120.0, 0.0015, false, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, 115121.0, 0.0008, true,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, 115118.0, 0.002, false, true,  Market::SPOT}, // 5
        {1760400008000000ULL, Symbol::BTCUSDT, 115122.0, 0.001, false, true,  Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, 115119.0, 0.001, true,  true,  Market::SPOT},
        {1760400010000000ULL, Symbol::BTCUSDT, 115125.0, 0.0015, false, true,  Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, 115130.0, 0.001, false, true,  Market::SPOT}, // 1
        {1760400011000001ULL, Symbol::BTCUSDT, 115131.0, 0.001, false, true,  Market::SPOT},
    }};

    const std::array<TradeEntry, 15> diffLargerThan1sArray = {{
        {1760400002000000ULL, Symbol::BTCUSDT, 115100.0, 0.001, false, true, Market::SPOT},
        {1760400002000001ULL, Symbol::BTCUSDT, 115101.0, 0.001, false, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, 115102.0, 0.001, false, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, 115103.0, 0.002, true,  true,  Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, 115104.0, 0.001, false, true,  Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, 115120.0, 0.0015, false, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, 115121.0, 0.0008, true,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, 115118.0, 0.002, false, true,  Market::SPOT},
        {1760400008000000ULL, Symbol::BTCUSDT, 115122.0, 0.001, false, true,  Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, 115119.0, 0.001, true,  true,  Market::SPOT},
        {1760400010000000ULL, Symbol::BTCUSDT, 115125.0, 0.0015, false, true,  Market::SPOT}, // 8
        {1760400011000000ULL, Symbol::BTCUSDT, 115130.0, 0.001, false, true,  Market::SPOT},
        {1760400011000001ULL, Symbol::BTCUSDT, 115131.0, 0.001, false, true,  Market::SPOT},
                                                                                                                                                      // 5
        {1760400014000001ULL, Symbol::BTCUSDT, 115132.69, 0.001, true, true,  Market::SPOT},
        {1760400017000001ULL, Symbol::BTCUSDT, 115133.11, 0.001, false, true,  Market::SPOT}, // 1
    }};
    template <class Container>
    void feed(RollingTradeStatistics& rts, const Container& trades) {
        for (const auto& t : trades) rts.update(t);
    }
}

bool approxEqual(const double x, const double expected, const double eps = 1e-9) {
    return std::abs(x - expected) < eps;
}

void testBenchmark(size_t loopsAmount = 10'000'000) {

    std::cout << "loopsAmount=" << loopsAmount << std::endl;

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getLastTradePrice();
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getLastTradePrice() mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile size_t sink = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink ^= rts.getBuyTradeCount(7);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getBuyTradeCount(7) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile size_t sink = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink ^= rts.getSellTradeCount(7);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getSellTradeCount(7) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getLowestPrice(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getLowestPrice(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getHighestPrice(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getHighestPrice(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getCumulatedBuyTradeVolume(7);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getCumulatedBuyTradeVolume(7) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getCumulatedSellTradeVolume(7);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getCumulatedSellTradeVolume(7) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getOldestPrice(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getOldestPrice(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getPriceDifference(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getPriceDifference(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getClosePrice(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getClosePrice(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getBiggestBuyTrade(7);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getBiggestBuyTrade(7) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getBiggestSellTrade(7);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getBiggestSellTrade(7) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getSMA(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getSMA(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getHLC3(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getHLC3(5) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getPriceDistance(3);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getPriceDistance(3) mean: " << mean_ns << " ns\n";
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        volatile double sink = 0.0;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < loopsAmount; ++i) {
            sink += rts.getBollingerBandsDistance(5);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        const double mean_ns = static_cast<double>(duration) / static_cast<double>(loopsAmount);
        std::cout << "getBollingerBandsDistance(5) mean: " << mean_ns << " ns\n";
    }
}

void testBaseCase() {
    {
        RollingTradeStatistics rts;
        assert(rts.getLastTradeTimestamp() == 0);
        assert(rts.getLastTradePrice() == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getLastTradeTimestamp() == 1760400002000000ULL);
        assert(rts.getLastTradePrice() == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getLastTradeTimestamp() == 1760400007000000ULL);
        assert(rts.getLastTradePrice() == 115118.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getLastTradeTimestamp() == 1760400011000001ULL);
        assert(rts.getLastTradePrice() == 115131.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getLastTradeTimestamp() == 1760400017000001ULL);
        assert(rts.getLastTradePrice() == 115133.11);
    }
}

void testGetBuyTradeCount() {
    {
        RollingTradeStatistics rts;
        assert(rts.getBuyTradeCount(1) == 0);
        assert(rts.getBuyTradeCount(5) == 0);
        assert(rts.getBuyTradeCount(M) == 0);
        assert(rts.getBuyTradeCount(10) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getBuyTradeCount(1) == 1);
        assert(rts.getBuyTradeCount(5) == 1);
        assert(rts.getBuyTradeCount(M) == 1);
        assert(rts.getBuyTradeCount(10) == 1);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getBuyTradeCount(1) == 1);
        assert(rts.getBuyTradeCount(5) == 3);
        assert(rts.getBuyTradeCount(M) == 6);
        assert(rts.getBuyTradeCount(10) == 6);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getBuyTradeCount(1) == 2);
        assert(rts.getBuyTradeCount(5) == 5);
        assert(rts.getBuyTradeCount(M) == 7);
        assert(rts.getBuyTradeCount(10) == 7);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getBuyTradeCount(1) == 1);
        assert(rts.getBuyTradeCount(5) == 1);
        assert(rts.getBuyTradeCount(M) == 4);
        assert(rts.getBuyTradeCount(10) == 4);
    }
}

void testGetSellTradeCount() {
    {
        RollingTradeStatistics rts;
        assert(rts.getSellTradeCount(1) == 0);
        assert(rts.getSellTradeCount(5) == 0);
        assert(rts.getSellTradeCount(M) == 0);
        assert(rts.getSellTradeCount(10) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getSellTradeCount(1) == 0);
        assert(rts.getSellTradeCount(5) == 0);
        assert(rts.getSellTradeCount(M) == 0);
        assert(rts.getSellTradeCount(10) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getSellTradeCount(1)  == 0);
        assert(rts.getSellTradeCount(5)  == 2);
        assert(rts.getSellTradeCount(M)  == 2);
        assert(rts.getSellTradeCount(10) == 2);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getSellTradeCount(1) == 0);
        assert(rts.getSellTradeCount(5) == 1);
        assert(rts.getSellTradeCount(M) == 2);
        assert(rts.getSellTradeCount(10) == 2);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getSellTradeCount(1)  == 0);
        assert(rts.getSellTradeCount(5)  == 1);
        assert(rts.getSellTradeCount(M) == 1);
        assert(rts.getSellTradeCount(10) == 1);
    }
}

void testGetLowestPrice() {
    {
        RollingTradeStatistics rts;
        assert(rts.getLowestPrice(1) == 0.0);
        assert(rts.getLowestPrice(5) == 0.0);
        assert(rts.getLowestPrice(M) == 0.0);
        assert(rts.getLowestPrice(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getLowestPrice(1) == 115100.0);
        assert(rts.getLowestPrice(5) == 115100.0);
        assert(rts.getLowestPrice(M) == 115100.0);
        assert(rts.getLowestPrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getLowestPrice(1) == 115118.0);
        assert(rts.getLowestPrice(5) == 115103.0);
        assert(rts.getLowestPrice(M) == 115100.0);
        assert(rts.getLowestPrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getLowestPrice(1) == 115130.0);
        assert(rts.getLowestPrice(5) == 115118.0);
        assert(rts.getLowestPrice(M) == 115104.0);
        assert(rts.getLowestPrice(10) == 115104.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getLowestPrice(1) == 115133.11);
        assert(rts.getLowestPrice(5) == 115131.0);
        assert(rts.getLowestPrice(M) == 115125.0);
        assert(rts.getLowestPrice(10) == 115125.0);
    }
}

void testGetHighestPrice() {
    {
        RollingTradeStatistics rts;
        assert(rts.getHighestPrice(1) == 0.0);
        assert(rts.getHighestPrice(5) == 0.0);
        assert(rts.getHighestPrice(M) == 0.0);
        assert(rts.getHighestPrice(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getHighestPrice(1) == 115100.0);
        assert(rts.getHighestPrice(5) == 115100.0);
        assert(rts.getHighestPrice(M) == 115100.0);
        assert(rts.getHighestPrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getHighestPrice(1) == 115118.0);
        assert(rts.getHighestPrice(5) == 115121.0);
        assert(rts.getHighestPrice(M) == 115121.0);
        assert(rts.getHighestPrice(10) == 115121.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getHighestPrice(1) == 115131.0);
        assert(rts.getHighestPrice(5) == 115131.0);
        assert(rts.getHighestPrice(M) == 115131.0);
        assert(rts.getHighestPrice(10) == 115131.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getHighestPrice(1)  == 115133.11);
        assert(rts.getHighestPrice(5) == 115133.11);
        assert(rts.getHighestPrice(M) == 115133.11);
        assert(rts.getHighestPrice(10) == 115133.11);
    }
}

void testGetCumulatedBuyVolume() {
    {
        RollingTradeStatistics rts;
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(1), 0.0));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(5), 0.0));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(M), 0.0));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(10), 0.0));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(1), 0.001));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(5), 0.001));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(M), 0.001));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(10), 0.001));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(1), 0.002));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(5), 0.0045));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(M), 0.0075));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(10), 0.0075));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(1), 0.0020));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(5), 0.0065));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(M), 0.0090));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(10), 0.0090));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(1), 0.001));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(5), 0.001));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(M), 0.0045));
        assert(approxEqual(rts.getCumulatedBuyTradeVolume(10), 0.0045));
    }
}

void testGetCumulatedSellVolume() {
    {
        RollingTradeStatistics rts;
        assert(approxEqual(rts.getCumulatedSellTradeVolume(1), 0.0));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(5), 0.0));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(M), 0.0));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(10), 0.0));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(approxEqual(rts.getCumulatedSellTradeVolume(1), 0.0));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(5), 0.0));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(M), 0.0));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(10), 0.0));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(approxEqual(rts.getCumulatedSellTradeVolume(1), 0.0000));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(5), 0.0028));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(M), 0.0028));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(10), 0.0028));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(approxEqual(rts.getCumulatedSellTradeVolume(1), 0.0000));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(5), 0.0010));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(M), 0.0018));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(10), 0.0018));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getCumulatedSellTradeVolume(1), 0.0000));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(5), 0.0010));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(M), 0.0010));
        assert(approxEqual(rts.getCumulatedSellTradeVolume(10), 0.0010));
    }
}

void testGetOldestPrice()
{
    {
       RollingTradeStatistics rts;
       assert(rts.getOldestPrice(1) == 0);
       assert(rts.getOldestPrice(5) == 0);
       assert(rts.getOldestPrice(M) == 0);
       assert(rts.getOldestPrice(10) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getOldestPrice(1) == 115100.0);
        assert(rts.getOldestPrice(5) == 115100.0);
        assert(rts.getOldestPrice(M) == 115100.0);
        assert(rts.getOldestPrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getOldestPrice(1) == 115118.0);
        assert(rts.getOldestPrice(5) == 115103.0);
        assert(rts.getOldestPrice(M) == 115100.0);
        assert(rts.getOldestPrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getOldestPrice(1) == 115130.0);
        assert(rts.getOldestPrice(5) == 115118.0);
        assert(rts.getOldestPrice(M) == 115104.0);
        assert(rts.getOldestPrice(10) == 115104.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getOldestPrice(1) == 115133.11);
        assert(rts.getOldestPrice(5) == 115131.00);
        assert(rts.getOldestPrice(M) == 115125.0);
        assert(rts.getOldestPrice(10) == 115125.0);
    }
}

void testGetPriceDifference()
{
    {
        RollingTradeStatistics rts;
        assert(rts.getPriceDifference(1) == 0);
        assert(rts.getPriceDifference(5) == 0);
        assert(rts.getPriceDifference(M) == 0);
        assert(rts.getPriceDifference(10) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getPriceDifference(1) == 0.0);
        assert(rts.getPriceDifference(5) == 0.0);
        assert(rts.getPriceDifference(M) == 0.0);
        assert(rts.getPriceDifference(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getPriceDifference(1) == 0.0);
        assert(rts.getPriceDifference(5) == 15.0);
        assert(rts.getPriceDifference(M) == 18.0);
        assert(rts.getPriceDifference(10) == 18.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getPriceDifference(1) == 1.0);
        assert(rts.getPriceDifference(5) == 13.0);
        assert(rts.getPriceDifference(M) == 27.0);
        assert(rts.getPriceDifference(10) == 27.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getPriceDifference(1), 0.0));
        assert(approxEqual(rts.getPriceDifference(5), 2.11));
        assert(approxEqual(rts.getPriceDifference(M), 8.11));
        assert(approxEqual(rts.getPriceDifference(10), 8.11));
    }
}

void testGetClosePrice()
{
    {
        RollingTradeStatistics rts;
        assert(rts.getClosePrice(1) == 0);
        assert(rts.getClosePrice(5) == 0);
        assert(rts.getClosePrice(M) == 0);
        assert(rts.getClosePrice(10) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getClosePrice(1) == 115100.0);
        assert(rts.getClosePrice(5) == 115100.0);
        assert(rts.getClosePrice(M) == 115100.0);
        assert(rts.getClosePrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getClosePrice(1) == 115121.0);
        assert(rts.getClosePrice(5) == 115102.0);
        assert(rts.getClosePrice(M) == 115100.0);
        assert(rts.getClosePrice(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getClosePrice(1) == 115125.0);
        assert(rts.getClosePrice(5) == 115121.0);
        assert(rts.getClosePrice(M) == 115104.0);
        assert(rts.getClosePrice(10) == 115104.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getClosePrice(1) == 115132.69);
        assert(rts.getClosePrice(5) == 115131.00);
        assert(rts.getClosePrice(M) == 115125.00);
        assert(rts.getClosePrice(10) == 115125.00);
    }
}

void testGetBiggestBuyTrade(){
    {
        RollingTradeStatistics rts;
        assert(rts.getBiggestBuyTrade(1) == 0.0);
        assert(rts.getBiggestBuyTrade(5) == 0.0);
        assert(rts.getBiggestBuyTrade(M) == 0.0);
        assert(rts.getBiggestBuyTrade(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getBiggestBuyTrade(1) == 0.001);
        assert(rts.getBiggestBuyTrade(5) == 0.001);
        assert(rts.getBiggestBuyTrade(M) == 0.001);
        assert(rts.getBiggestBuyTrade(10) == 0.001);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getBiggestBuyTrade(1) == 0.002);
        assert(rts.getBiggestBuyTrade(5) == 0.002);
        assert(rts.getBiggestBuyTrade(M) == 0.002);
        assert(rts.getBiggestBuyTrade(10) == 0.002);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getBiggestBuyTrade(1) == 0.001);
        assert(rts.getBiggestBuyTrade(5) == 0.002);
        assert(rts.getBiggestBuyTrade(M) == 0.002);
        assert(rts.getBiggestBuyTrade(10) == 0.002);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getBiggestBuyTrade(1) == 0.001);
        assert(rts.getBiggestBuyTrade(5) == 0.001);
        assert(rts.getBiggestBuyTrade(M) == 0.0015);
        assert(rts.getBiggestBuyTrade(10) == 0.0015);
    }
}

void testGetBiggestSellTrade(){
    {
        RollingTradeStatistics rts;
        assert(rts.getBiggestSellTrade(1) == 0.0);
        assert(rts.getBiggestSellTrade(5) == 0.0);
        assert(rts.getBiggestSellTrade(M) == 0.0);
        assert(rts.getBiggestSellTrade(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getBiggestSellTrade(1) == 0.0);
        assert(rts.getBiggestSellTrade(5) == 0.0);
        assert(rts.getBiggestSellTrade(M) == 0.0);
        assert(rts.getBiggestSellTrade(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getBiggestSellTrade(1) == 0.0);
        assert(rts.getBiggestSellTrade(5) == 0.002);
        assert(rts.getBiggestSellTrade(M) == 0.002);
        assert(rts.getBiggestSellTrade(10) == 0.002);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getBiggestSellTrade(1) == 0.0);
        assert(rts.getBiggestSellTrade(5) == 0.001);
        assert(rts.getBiggestSellTrade(M) == 0.001);
        assert(rts.getBiggestSellTrade(10) == 0.001);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getBiggestSellTrade(1) == 0.0);
        assert(rts.getBiggestSellTrade(5) == 0.001);
        assert(rts.getBiggestSellTrade(M) == 0.001);
        assert(rts.getBiggestSellTrade(10) == 0.001);
    }
}

void testGetSMA(){
    {
        RollingTradeStatistics rts;
        assert(rts.getSMA(1) == 0.0);
        assert(rts.getSMA(5) == 0.0);
        assert(rts.getSMA(M) == 0.0);
        assert(rts.getSMA(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getSMA(1) == 115100.0);
        assert(rts.getSMA(5) == 115100.0);
        assert(rts.getSMA(M) == 115100.0);
        assert(rts.getSMA(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getSMA(1) == 115118.0);
        assert(rts.getSMA(5) == 115113.2);
        assert(rts.getSMA(M) == 115111.33333333333);
        assert(rts.getSMA(10) == 115111.33333333333);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getSMA(1) == 115131.0);
        assert(rts.getSMA(5) == 115123.0);
        assert(rts.getSMA(M) == 115120.0);
        assert(rts.getSMA(10) == 115120.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getSMA(1), 115133.11));
        assert(approxEqual(rts.getSMA(5), 115132.436));
        assert(approxEqual(rts.getSMA(M), 115131.1474999999773));
        assert(approxEqual(rts.getSMA(10), 115131.1474999999773));
    }
}

void testGetHLC3(){
    {
        RollingTradeStatistics rts;
        assert(rts.getHLC3(1) == 0.0);
        assert(rts.getHLC3(5) == 0.0);
        assert(rts.getHLC3(M) == 0.0);
        assert(rts.getHLC3(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getHLC3(1) == 115100.0);
        assert(rts.getHLC3(5) == 115100.0);
        assert(rts.getHLC3(M) == 115100.0);
        assert(rts.getHLC3(10) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getHLC3(1) == 115118.0);
        assert(rts.getHLC3(5) == 115114.0);
        assert(rts.getHLC3(M) == 115113.0);
        assert(rts.getHLC3(10) == 115113.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getHLC3(1) == 115130.66666666667);
        assert(rts.getHLC3(5) == 115126.66666666667);
        assert(rts.getHLC3(M) == 115122.0);
        assert(rts.getHLC3(10) == 115122.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getHLC3(1), 115133.11));
        assert(approxEqual(rts.getHLC3(5), 115132.40666666666));
        assert(approxEqual(rts.getHLC3(M), 115130.40666666666));
        assert(approxEqual(rts.getHLC3(10), 115130.40666666666));
    }
}

void testGetPriceDistance(){
    {
        RollingTradeStatistics rts;
        assert(rts.getPriceDistance(1) == 0.0);
        assert(rts.getPriceDistance(3) == 0.0);
        assert(rts.getPriceDistance(M) == 0.0);
        assert(rts.getPriceDistance(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getPriceDistance(1) == 0.0);
        assert(rts.getPriceDistance(3) == 0.0);
        assert(rts.getPriceDistance(M) == 0.0);
        assert(rts.getPriceDistance(10) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getPriceDistance(1) == 3.0);
        assert(rts.getPriceDistance(3) == 20.0);
        assert(rts.getPriceDistance(M) == 24.0);
        assert(rts.getPriceDistance(10) == 24.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getPriceDistance(1) == 6.0);
        assert(rts.getPriceDistance(3) == 15.0);
        assert(rts.getPriceDistance(M) == 31.0);
        assert(rts.getPriceDistance(10) == 31.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getPriceDistance(1), 0.41999999999825377));
        assert(approxEqual(rts.getPriceDistance(3), 0.41999999999825377));
        assert(approxEqual(rts.getPriceDistance(M), 8.110000000000582));
        assert(approxEqual(rts.getPriceDistance(10), 8.110000000000582));
    }
}

void testGetBollingerBandsDistance(){
    {
        RollingTradeStatistics rts;
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(10), 0.0));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(10), 0.0));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.6011281708244));
        assert(approxEqual(rts.getBollingerBandsDistance(10), 0.7936757802761));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 1.7056057308445));
        assert(approxEqual(rts.getBollingerBandsDistance(10), 1.5328126314413));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.9155201871654));
        assert(approxEqual(rts.getBollingerBandsDistance(10), 0.7942156884910));
    }
}

int main() {
	/* tests below workz only with MAX_BUCKETS = 8 set in RollingTradeStatistics.h */

    // std::cout << std::fixed << std::setprecision(13);
    testBenchmark();

    testBaseCase();
    testGetBuyTradeCount();
    testGetSellTradeCount();
    testGetLowestPrice();
    testGetHighestPrice();
    testGetCumulatedBuyVolume();
    testGetCumulatedSellVolume();
    testGetOldestPrice();
    testGetPriceDifference();
    testGetClosePrice();
    testGetBiggestBuyTrade();
    testGetBiggestSellTrade();
    testGetSMA();
    testGetHLC3();

    testGetPriceDistance();
    testGetBollingerBandsDistance();

    return 0;
}
