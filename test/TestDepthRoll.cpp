#include <array>
#include <cassert>

#include "../include/DepthRoll.h"

constexpr size_t M = 8;

namespace {
    const std::array<DifferenceDepthEntry, 1> oneTradeArray = {{
        {1760400002000000ULL, Symbol::BTCUSDT, false, 115100.0, 0.001, true, Market::SPOT},
    }};

    const std::array<DifferenceDepthEntry, 8> baseTradeArray = {{
        {1760400002000000ULL, Symbol::BTCUSDT, false, 115100.0, 0.001, true, Market::SPOT}, // 6
        {1760400002000001ULL, Symbol::BTCUSDT, false, 115101.0, 0.001, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, false, 115102.0, 0.001, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, true, 115103.0, 0.002,  true,  Market::SPOT}, // 5
        {1760400004000000ULL, Symbol::BTCUSDT, false, 115104.0, 0.001, true,  Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, false, 115120.0, 0.0015, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, true, 115121.0, 0.0008,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, false, 115118.0, 0.002, true,  Market::SPOT}, // 1
    }};

    const std::array<DifferenceDepthEntry, 13> baseTradeArrayDuringRolling = {{
        {1760400002000000ULL, Symbol::BTCUSDT, false, 115100.0, 0.001, true, Market::SPOT},
        {1760400002000001ULL, Symbol::BTCUSDT, false, 115101.0, 0.001, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, false, 115102.0, 0.001, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, true, 115103.0, 0.002,  true,  Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, false, 115104.0, 0.001, true,  Market::SPOT}, // 8
        {1760400005000000ULL, Symbol::BTCUSDT, false, 115120.0, 0.0015, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, true, 115121.0, 0.0008,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, false, 115118.0, 0.002, true,  Market::SPOT}, // 5
        {1760400008000000ULL, Symbol::BTCUSDT, false, 115122.0, 0.001, true,  Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, true, 115119.0, 0.001,  true,  Market::SPOT},
        {1760400010000000ULL, Symbol::BTCUSDT, false, 115125.0, 0.0015, true,  Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, false, 115130.0, 0.001, true,  Market::SPOT}, // 1
        {1760400011000001ULL, Symbol::BTCUSDT, false, 115131.0, 0.001, true,  Market::SPOT},
    }};

    const std::array<DifferenceDepthEntry, 15> diffLargerThan1sArray = {{
        {1760400002000000ULL, Symbol::BTCUSDT, false, 115100.0, 0.001, true, Market::SPOT},
        {1760400002000001ULL, Symbol::BTCUSDT, false, 115101.0, 0.001, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, false, 115102.0, 0.001, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, true, 115103.0, 0.002,  true,  Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, false, 115104.0, 0.001, true,  Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, false, 115120.0, 0.0015, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, true, 115121.0, 0.0008,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, false, 115118.0, 0.002, true,  Market::SPOT},
        {1760400008000000ULL, Symbol::BTCUSDT, false, 115122.0, 0.001, true,  Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, true, 115119.0, 0.001,  true,  Market::SPOT},
        {1760400010000000ULL, Symbol::BTCUSDT, false, 115125.0, 0.0015, true,  Market::SPOT}, // 8
        {1760400011000000ULL, Symbol::BTCUSDT, false, 115130.0, 0.001, true,  Market::SPOT},
        {1760400011000001ULL, Symbol::BTCUSDT, false, 115131.0, 0.001, true,  Market::SPOT},
		// 5
        {1760400014000001ULL, Symbol::BTCUSDT, true, 115132.69, 0.001, true,  Market::SPOT},
        {1760400017000001ULL, Symbol::BTCUSDT, false, 115133.11, 0.001, true,  Market::SPOT}, // 1
    }};

    const std::array<DifferenceDepthEntry, 16> deltaEqualMaxBuckets = {{
        {1760400002000000ULL, Symbol::BTCUSDT, false, 115100.0, 0.001, true, Market::SPOT},
        {1760400002000001ULL, Symbol::BTCUSDT, false, 115101.0, 0.001, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, false, 115102.0, 0.001, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, true, 115103.0, 0.002,  true,  Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, false, 115104.0, 0.001, true,  Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, false, 115120.0, 0.0015, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, true, 115121.0, 0.0008,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, false, 115118.0, 0.002, true,  Market::SPOT},
        {1760400008000000ULL, Symbol::BTCUSDT, false, 115122.0, 0.001, true,  Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, true, 115119.0, 0.001,  true,  Market::SPOT},
        {1760400010000000ULL, Symbol::BTCUSDT, false, 115125.0, 0.0015, true,  Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, false, 115130.0, 0.001, true,  Market::SPOT},
        {1760400011000001ULL, Symbol::BTCUSDT, false, 115131.0, 0.001, true,  Market::SPOT},
        {1760400014000001ULL, Symbol::BTCUSDT, true, 115132.69, 0.001, true,  Market::SPOT},
        {1760400017000001ULL, Symbol::BTCUSDT, false, 115133.11, 0.001, true,  Market::SPOT},
		// 5s 8s
        {1760400025000001ULL, Symbol::BTCUSDT, false, 115199.97, 0.001, true,  Market::SPOT}, //1s
    }};

    const std::array<DifferenceDepthEntry, 17> deltaLargerThanMaxBuckets = {{
        {1760400002000000ULL, Symbol::BTCUSDT, false, 115100.0, 0.001, true, Market::SPOT},
        {1760400002000001ULL, Symbol::BTCUSDT, false, 115101.0, 0.001, true, Market::SPOT},
        {1760400002999999ULL, Symbol::BTCUSDT, false, 115102.0, 0.001, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, true, 115103.0, 0.002,  true,  Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, false, 115104.0, 0.001, true,  Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, false, 115120.0, 0.0015, true,  Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, true, 115121.0, 0.0008,  true,  Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, false, 115118.0, 0.002, true,  Market::SPOT},
        {1760400008000000ULL, Symbol::BTCUSDT, false, 115122.0, 0.001, true,  Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, true, 115119.0, 0.001,  true,  Market::SPOT},
        {1760400010000000ULL, Symbol::BTCUSDT, false, 115125.0, 0.0015, true,  Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, false, 115130.0, 0.001, true,  Market::SPOT},
        {1760400011000001ULL, Symbol::BTCUSDT, false, 115131.0, 0.001, true,  Market::SPOT},
        {1760400014000001ULL, Symbol::BTCUSDT, true, 115132.69, 0.001, true,  Market::SPOT},
        {1760400017000001ULL, Symbol::BTCUSDT, false, 115133.11, 0.001, true,  Market::SPOT},
		// 8s
		// 5s
        {1760400026000001ULL, Symbol::BTCUSDT, false, 115666.66, 0.001, true,  Market::SPOT},	// 4s
        {1760400029000001ULL, Symbol::BTCUSDT, true, 115777.03, 0.001, true,  Market::SPOT},   // 1s
    }};

    template <class Container>
    void feed(DepthRoll &dr, const Container& depthEntries) {
        for (const auto& de : depthEntries) dr.update(de);
    }
}

bool approxEqual(const double x, const double expected, const double eps = 1e-9) {
    return std::abs(x - expected) < eps;
}


void testBaseCase() {
    {
        DepthRoll dr;
        assert(dr.getNewestDepthEntryTimestamp() == 0);
        assert(dr.getNewestPrice() == 0.0);
    }

    {
        DepthRoll dr;
        feed(dr, oneTradeArray);
        assert(dr.getNewestDepthEntryTimestamp() == 1760400002000000ULL);
        assert(dr.getNewestPrice() == 115100.0);
    }

    {
        DepthRoll dr;
        feed(dr, baseTradeArray);
        assert(dr.getNewestDepthEntryTimestamp() == 1760400007000000ULL);
        assert(dr.getNewestPrice() == 115118.0);
    }

    {
        DepthRoll dr;
        feed(dr, baseTradeArrayDuringRolling);
        assert(dr.getNewestDepthEntryTimestamp() == 1760400011000001ULL);
        assert(dr.getNewestPrice() == 115131.0);
    }

    {
        DepthRoll dr;
        feed(dr, diffLargerThan1sArray);
        assert(dr.getNewestDepthEntryTimestamp() == 1760400017000001ULL);
        assert(dr.getNewestPrice() == 115133.11);
    }

    {
        DepthRoll dr;
        feed(dr, deltaEqualMaxBuckets);
        assert(dr.getNewestDepthEntryTimestamp() == 1760400025000001ULL);
        assert(dr.getNewestPrice() == 115199.97);
    }

    {
        DepthRoll dr;
        feed(dr, deltaLargerThanMaxBuckets);
        assert(dr.getNewestDepthEntryTimestamp() == 1760400029000001ULL);
        assert(dr.getNewestPrice() == 115777.03);
    }

}

void testgetBidAskCount() {
    {
        DepthRoll dr;

        assert(dr.getBidAskCount(1).bid == 0);
        assert(dr.getBidAskCount(5).bid == 0);
        assert(dr.getBidAskCount(M).bid == 0);

        assert(dr.getBidAskCount(1).ask == 0);
        assert(dr.getBidAskCount(5).ask == 0);
        assert(dr.getBidAskCount(M).ask == 0);
    }

    {
        DepthRoll dr;
        feed(dr, oneTradeArray);

        assert(dr.getBidAskCount(1).bid == 1);
        assert(dr.getBidAskCount(5).bid == 1);
        assert(dr.getBidAskCount(M).bid == 1);

        assert(dr.getBidAskCount(1).ask == 0);
        assert(dr.getBidAskCount(5).ask == 0);
        assert(dr.getBidAskCount(M).ask == 0);
    }

    {
        DepthRoll dr;
        feed(dr, baseTradeArray);

        assert(dr.getBidAskCount(1).bid  == 1);
        assert(dr.getBidAskCount(5).bid  == 3);
        assert(dr.getBidAskCount(M).bid  == 6);

        assert(dr.getBidAskCount(1).ask == 0);
        assert(dr.getBidAskCount(5).ask == 2);
        assert(dr.getBidAskCount(M).ask == 2);
    }

    {
        DepthRoll dr;
        feed(dr, baseTradeArrayDuringRolling);

        assert(dr.getBidAskCount(1).bid == 2);
        assert(dr.getBidAskCount(5).bid == 5);
        assert(dr.getBidAskCount(M).bid == 7);

        assert(dr.getBidAskCount(1).ask == 0);
        assert(dr.getBidAskCount(5).ask == 1);
        assert(dr.getBidAskCount(M).ask == 2);
    }

    {
        DepthRoll dr;
        feed(dr, diffLargerThan1sArray);

        assert(dr.getBidAskCount(1).bid  == 1);
        assert(dr.getBidAskCount(5).bid  == 1);
        assert(dr.getBidAskCount(M).bid  == 4);

        assert(dr.getBidAskCount(1).ask == 0);
        assert(dr.getBidAskCount(5).ask == 1);
        assert(dr.getBidAskCount(M).ask == 1);
    }

    {
        DepthRoll dr;
        feed(dr, deltaEqualMaxBuckets);

        assert(dr.getBidAskCount(1).bid  == 1);
        assert(dr.getBidAskCount(5).bid  == 1);
        assert(dr.getBidAskCount(M).bid  == 1);

        assert(dr.getBidAskCount(1).ask == 0);
        assert(dr.getBidAskCount(5).ask == 0);
        assert(dr.getBidAskCount(M).ask == 0);
    }

    {
        DepthRoll dr;
        feed(dr, deltaLargerThanMaxBuckets);

        assert(dr.getBidAskCount(1).bid  == 0);
        assert(dr.getBidAskCount(5).bid  == 1);
        assert(dr.getBidAskCount(M).bid  == 1);

        assert(dr.getBidAskCount(1).ask == 1);
        assert(dr.getBidAskCount(5).ask == 1);
        assert(dr.getBidAskCount(M).ask == 1);
    }
}


int main() {
	// tests below workz only with MAX_BUCKETS = 8 set in DepthRoll.h
    // std::cout << std::fixed << std::setprecision(13);

    testBaseCase();
    testgetBidAskCount();

    return 0;
}
