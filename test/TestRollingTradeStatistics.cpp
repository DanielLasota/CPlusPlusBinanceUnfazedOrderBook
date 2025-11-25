#include <cassert>
#include <array>
#include "../include/RollingTradeStatistics.h"

constexpr size_t M = 8;

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

    const std::array<TradeEntry, 16> deltaEqualMaxBuckets = {{
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
        {1760400010000000ULL, Symbol::BTCUSDT, 115125.0, 0.0015, false, true,  Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, 115130.0, 0.001, false, true,  Market::SPOT},
        {1760400011000001ULL, Symbol::BTCUSDT, 115131.0, 0.001, false, true,  Market::SPOT},
        {1760400014000001ULL, Symbol::BTCUSDT, 115132.69, 0.001, true, true,  Market::SPOT},
        {1760400017000001ULL, Symbol::BTCUSDT, 115133.11, 0.001, false, true,  Market::SPOT},
		// 5s 8s
        {1760400025000001ULL, Symbol::BTCUSDT, 115199.97, 0.001, false, true,  Market::SPOT}, //1s
    }};

    const std::array<TradeEntry, 17> deltaLargerThanMaxBuckets = {{
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
        {1760400010000000ULL, Symbol::BTCUSDT, 115125.0, 0.0015, false, true,  Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, 115130.0, 0.001, false, true,  Market::SPOT},
        {1760400011000001ULL, Symbol::BTCUSDT, 115131.0, 0.001, false, true,  Market::SPOT},
        {1760400014000001ULL, Symbol::BTCUSDT, 115132.69, 0.001, true, true,  Market::SPOT},
        {1760400017000001ULL, Symbol::BTCUSDT, 115133.11, 0.001, false, true,  Market::SPOT},
		// 8s
		// 5s
        {1760400026000001ULL, Symbol::BTCUSDT, 115666.66, 0.001, false, true,  Market::SPOT},	// 4s
        {1760400029000001ULL, Symbol::BTCUSDT, 115777.03, 0.001, true, true,  Market::SPOT},   // 1s
    }};

    template <class Container>
    void feed(RollingTradeStatistics& rts, const Container& trades) {
        for (const auto& t : trades) rts.update(t);
    }
}

bool approxEqual(const double x, const double expected, const double eps = 1e-9) {
    return std::abs(x - expected) < eps;
}


void testBaseCase() {
    {
        RollingTradeStatistics rts;
        assert(rts.getLastTradeTimestamp() == 0);
        assert(rts.getNewestPrice() == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getLastTradeTimestamp() == 1760400002000000ULL);
        assert(rts.getNewestPrice() == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getLastTradeTimestamp() == 1760400007000000ULL);
        assert(rts.getNewestPrice() == 115118.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getLastTradeTimestamp() == 1760400011000001ULL);
        assert(rts.getNewestPrice() == 115131.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getLastTradeTimestamp() == 1760400017000001ULL);
        assert(rts.getNewestPrice() == 115133.11);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(rts.getLastTradeTimestamp() == 1760400025000001ULL);
        assert(rts.getNewestPrice() == 115199.97);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(rts.getLastTradeTimestamp() == 1760400029000001ULL);
        assert(rts.getNewestPrice() == 115777.03);
    }

}

void testGetBuySellTradeCount() {
    {
        RollingTradeStatistics rts;

        assert(rts.getBuySellTradeCount(1).buy == 0);
        assert(rts.getBuySellTradeCount(5).buy == 0);
        assert(rts.getBuySellTradeCount(M).buy == 0);

        assert(rts.getBuySellTradeCount(1).sell == 0);
        assert(rts.getBuySellTradeCount(5).sell == 0);
        assert(rts.getBuySellTradeCount(M).sell == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);

        assert(rts.getBuySellTradeCount(1).buy == 1);
        assert(rts.getBuySellTradeCount(5).buy == 1);
        assert(rts.getBuySellTradeCount(M).buy == 1);

        assert(rts.getBuySellTradeCount(1).sell == 0);
        assert(rts.getBuySellTradeCount(5).sell == 0);
        assert(rts.getBuySellTradeCount(M).sell == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);

        assert(rts.getBuySellTradeCount(1).buy  == 1);
        assert(rts.getBuySellTradeCount(5).buy  == 3);
        assert(rts.getBuySellTradeCount(M).buy  == 6);

        assert(rts.getBuySellTradeCount(1).sell == 0);
        assert(rts.getBuySellTradeCount(5).sell == 2);
        assert(rts.getBuySellTradeCount(M).sell == 2);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);

        assert(rts.getBuySellTradeCount(1).buy == 2);
        assert(rts.getBuySellTradeCount(5).buy == 5);
        assert(rts.getBuySellTradeCount(M).buy == 7);

        assert(rts.getBuySellTradeCount(1).sell == 0);
        assert(rts.getBuySellTradeCount(5).sell == 1);
        assert(rts.getBuySellTradeCount(M).sell == 2);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        assert(rts.getBuySellTradeCount(1).buy  == 1);
        assert(rts.getBuySellTradeCount(5).buy  == 1);
        assert(rts.getBuySellTradeCount(M).buy  == 4);

        assert(rts.getBuySellTradeCount(1).sell == 0);
        assert(rts.getBuySellTradeCount(5).sell == 1);
        assert(rts.getBuySellTradeCount(M).sell == 1);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);

        assert(rts.getBuySellTradeCount(1).buy  == 1);
        assert(rts.getBuySellTradeCount(5).buy  == 1);
        assert(rts.getBuySellTradeCount(M).buy  == 1);

        assert(rts.getBuySellTradeCount(1).sell == 0);
        assert(rts.getBuySellTradeCount(5).sell == 0);
        assert(rts.getBuySellTradeCount(M).sell == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);

        assert(rts.getBuySellTradeCount(1).buy  == 0);
        assert(rts.getBuySellTradeCount(5).buy  == 1);
        assert(rts.getBuySellTradeCount(M).buy  == 1);

        assert(rts.getBuySellTradeCount(1).sell == 1);
        assert(rts.getBuySellTradeCount(5).sell == 1);
        assert(rts.getBuySellTradeCount(M).sell == 1);
    }
}

void testGetHiLoPrice() {
    {
        RollingTradeStatistics rts;

        assert(rts.getHiLoPrice(1).h == 0.0);
        assert(rts.getHiLoPrice(5).h == 0.0);
        assert(rts.getHiLoPrice(M).h == 0.0);

        assert(rts.getHiLoPrice(1).l == std::numeric_limits<double>::infinity());
        assert(rts.getHiLoPrice(5).l == std::numeric_limits<double>::infinity());
        assert(rts.getHiLoPrice(M).l == std::numeric_limits<double>::infinity());
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);

        assert(rts.getHiLoPrice(1).h == 115100.0);
        assert(rts.getHiLoPrice(5).h == 115100.0);
        assert(rts.getHiLoPrice(M).h == 115100.0);

        assert(rts.getHiLoPrice(1).l == 115100.0);
        assert(rts.getHiLoPrice(5).l == 115100.0);
        assert(rts.getHiLoPrice(M).l == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);

        assert(rts.getHiLoPrice(1).h == 115118.0);
        assert(rts.getHiLoPrice(5).h == 115121.0);
        assert(rts.getHiLoPrice(M).h == 115121.0);

        assert(rts.getHiLoPrice(1).l == 115118.0);
        assert(rts.getHiLoPrice(5).l == 115103.0);
        assert(rts.getHiLoPrice(M).l == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);

        assert(rts.getHiLoPrice(1).h == 115131.0);
        assert(rts.getHiLoPrice(5).h == 115131.0);
        assert(rts.getHiLoPrice(M).h == 115131.0);

        assert(rts.getHiLoPrice(1).l == 115130.0);
        assert(rts.getHiLoPrice(5).l == 115118.0);
        assert(rts.getHiLoPrice(M).l == 115104.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        assert(rts.getHiLoPrice(1).h == 115133.11);
        assert(rts.getHiLoPrice(5).h == 115133.11);
        assert(rts.getHiLoPrice(M).h == 115133.11);

        assert(rts.getHiLoPrice(1).l == 115133.11);
        assert(rts.getHiLoPrice(5).l == 115131.0);
        assert(rts.getHiLoPrice(M).l == 115125.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);

        assert(rts.getHiLoPrice(1).h == 115199.97);
        assert(rts.getHiLoPrice(5).h == 115199.97);
        assert(rts.getHiLoPrice(M).h == 115199.97);

        assert(rts.getHiLoPrice(1).l == 115199.97);
        assert(rts.getHiLoPrice(5).l == 115133.11);
        assert(rts.getHiLoPrice(M).l == 115133.11);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);

        assert(rts.getHiLoPrice(1).h == 115777.03);
        assert(rts.getHiLoPrice(5).h == 115777.03);
        assert(rts.getHiLoPrice(M).h == 115777.03);

        assert(rts.getHiLoPrice(1).l == 115777.03);
        assert(rts.getHiLoPrice(5).l == 115133.11);
        assert(rts.getHiLoPrice(M).l == 115133.11);
    }
}

void testGetCumBuyAndSellQuantity()
{
    {
        RollingTradeStatistics rts;

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0000));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0000));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0010));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0000));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0020));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0045));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0075));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0028));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0028));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0020));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0065));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0090));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0018));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0045));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0010));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0010));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);

        assert(approxEqual(rts.getCumBuySellQuantity(1).buy, 0.0000));
        assert(approxEqual(rts.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).buy, 0.0010));

        assert(approxEqual(rts.getCumBuySellQuantity(1).sell, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(5).sell, 0.0010));
        assert(approxEqual(rts.getCumBuySellQuantity(M).sell, 0.0010));
    }
}

void testGetOldestPrice()
{
    {
       RollingTradeStatistics rts;
       assert(rts.getOldestPrice(1) == 0);
       assert(rts.getOldestPrice(5) == 0);
       assert(rts.getOldestPrice(M) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getOldestPrice(1) == 115100.0);
        assert(rts.getOldestPrice(5) == 0.0);
        assert(rts.getOldestPrice(M) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getOldestPrice(1) == 115118.0);
        assert(rts.getOldestPrice(5) == 115103.0);
        assert(rts.getOldestPrice(M) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getOldestPrice(1) == 115130.0);
        assert(rts.getOldestPrice(5) == 115118.0);
        assert(rts.getOldestPrice(M) == 115104.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getOldestPrice(1) == 115133.11);
        assert(rts.getOldestPrice(5) == 115131.00);
        assert(rts.getOldestPrice(M) == 115125.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(rts.getOldestPrice(1) == 115199.97);
        assert(rts.getOldestPrice(5) == 115133.11);
        assert(rts.getOldestPrice(M) == 115133.11);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(rts.getOldestPrice(1) == 115777.03);
        assert(rts.getOldestPrice(5) == 115133.11);
        assert(rts.getOldestPrice(M) == 115133.11);
    }
}

void testGetPriceDifference()
{
    {
        RollingTradeStatistics rts;
        assert(rts.getPriceDifference(1) == 0);
        assert(rts.getPriceDifference(5) == 0);
        assert(rts.getPriceDifference(M) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getPriceDifference(1) == 0.0);
        assert(rts.getPriceDifference(5) == 115100.0);
        assert(rts.getPriceDifference(M) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getPriceDifference(1) == 0.0);
        assert(rts.getPriceDifference(5) == 15.0);
        assert(rts.getPriceDifference(M) == 115118.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getPriceDifference(1) == 1.0);
        assert(rts.getPriceDifference(5) == 13.0);
        assert(rts.getPriceDifference(M) == 27.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getPriceDifference(1), 0.0));
        assert(approxEqual(rts.getPriceDifference(5), 2.11));
        assert(approxEqual(rts.getPriceDifference(M), 8.11));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(approxEqual(rts.getPriceDifference(1), 0.0));
        assert(approxEqual(rts.getPriceDifference(5), 66.86));
        assert(approxEqual(rts.getPriceDifference(M), 66.86));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(approxEqual(rts.getPriceDifference(1), 0.0));
        assert(approxEqual(rts.getPriceDifference(5), 643.92));
        assert(approxEqual(rts.getPriceDifference(M), 643.92));
    }
}

void testGetClosePrice()
{
    {
        RollingTradeStatistics rts;
        assert(rts.getClosePrice(1) == 0);
        assert(rts.getClosePrice(5) == 0);
        assert(rts.getClosePrice(M) == 0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getClosePrice(1) == 0.0);
        assert(rts.getClosePrice(5) == 0.0);
        assert(rts.getClosePrice(M) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getClosePrice(1) == 115121.0);
        assert(rts.getClosePrice(5) == 115102.0);
        assert(rts.getClosePrice(M) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getClosePrice(1) == 115125.0);
        assert(rts.getClosePrice(5) == 115121.0);
        assert(rts.getClosePrice(M) == 115104.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getClosePrice(1) == 115132.69);
        assert(rts.getClosePrice(5) == 115131.00);
        assert(rts.getClosePrice(M) == 115125.00);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(rts.getClosePrice(1) == 115133.11);
        assert(rts.getClosePrice(5) == 115133.11);
        assert(rts.getClosePrice(M) == 115133.11);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(rts.getClosePrice(1) == 115666.66);
        assert(rts.getClosePrice(5) == 115133.11);
        assert(rts.getClosePrice(M) == 115133.11);
    }
}

void testGetBiggestBuySell(){
    {
        RollingTradeStatistics rts;
        assert(rts.getBiggestBuySell(1).buy == 0.0);
        assert(rts.getBiggestBuySell(5).buy == 0.0);
        assert(rts.getBiggestBuySell(M).buy == 0.0);

        assert(rts.getBiggestBuySell(1).sell == 0.0);
        assert(rts.getBiggestBuySell(5).sell == 0.0);
        assert(rts.getBiggestBuySell(M).sell == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getBiggestBuySell(1).buy == 0.001);
        assert(rts.getBiggestBuySell(5).buy == 0.001);
        assert(rts.getBiggestBuySell(M).buy == 0.001);

        assert(rts.getBiggestBuySell(1).sell == 0.0);
        assert(rts.getBiggestBuySell(5).sell == 0.0);
        assert(rts.getBiggestBuySell(M).sell == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getBiggestBuySell(1).buy == 0.002);
        assert(rts.getBiggestBuySell(5).buy == 0.002);
        assert(rts.getBiggestBuySell(M).buy == 0.002);

        assert(rts.getBiggestBuySell(1).sell == 0.000);
        assert(rts.getBiggestBuySell(5).sell == 0.002);
        assert(rts.getBiggestBuySell(M).sell == 0.002);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getBiggestBuySell(1).buy == 0.001);
        assert(rts.getBiggestBuySell(5).buy == 0.002);
        assert(rts.getBiggestBuySell(M).buy == 0.002);

        assert(rts.getBiggestBuySell(1).sell == 0.000);
        assert(rts.getBiggestBuySell(5).sell == 0.001);
        assert(rts.getBiggestBuySell(M).sell == 0.001);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(rts.getBiggestBuySell(1).buy == 0.001);
        assert(rts.getBiggestBuySell(5).buy == 0.001);
        assert(rts.getBiggestBuySell(M).buy == 0.0015);

        assert(rts.getBiggestBuySell(1).sell == 0.0);
        assert(rts.getBiggestBuySell(5).sell == 0.001);
        assert(rts.getBiggestBuySell(M).sell == 0.001);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(rts.getBiggestBuySell(1).buy == 0.001);
        assert(rts.getBiggestBuySell(5).buy == 0.001);
        assert(rts.getBiggestBuySell(M).buy == 0.001);

        assert(rts.getBiggestBuySell(1).sell == 0.000);
        assert(rts.getBiggestBuySell(5).sell == 0.000);
        assert(rts.getBiggestBuySell(M).sell == 0.000);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(rts.getBiggestBuySell(1).buy == 0.000);
        assert(rts.getBiggestBuySell(5).buy == 0.001);
        assert(rts.getBiggestBuySell(M).buy == 0.001);

        assert(rts.getBiggestBuySell(1).sell == 0.001);
        assert(rts.getBiggestBuySell(5).sell == 0.001);
        assert(rts.getBiggestBuySell(M).sell == 0.001);
    }
}

void testGetSMA(){
    {
        RollingTradeStatistics rts;
        assert(rts.getSMA(1) == 0.0);
        assert(rts.getSMA(5) == 0.0);
        assert(rts.getSMA(M) == 0.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getSMA(1) == 115100.0);
        assert(rts.getSMA(5) == 23020.0);
        assert(rts.getSMA(M) == 14387.5);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getSMA(1) == 115118.0);
        assert(rts.getSMA(5) == 115113.2);
        assert(rts.getSMA(M) == 86333.5);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getSMA(1) == 115131.0);
        assert(rts.getSMA(5) == 115123.0);
        assert(rts.getSMA(M) == 115120.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getSMA(1), 115133.11));
        assert(approxEqual(rts.getSMA(5), 115132.436));
        assert(approxEqual(rts.getSMA(M), 115131.1474999999773));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(rts.getSMA(1) == 115199.97);
        assert(rts.getSMA(5) == 115146.482);
        assert(rts.getSMA(M) == 115141.4675);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(approxEqual(rts.getSMA(1), 115777.03));
        assert(approxEqual(rts.getSMA(5), 115582.02400000002));
        assert(approxEqual(rts.getSMA(M), 115413.68125));
    }
}

void testGetHLC3(){
    {
        RollingTradeStatistics rts;
        assert(rts.getHLC3(1) == std::numeric_limits<double>::infinity());
        assert(rts.getHLC3(5) == std::numeric_limits<double>::infinity());
        assert(rts.getHLC3(M) == std::numeric_limits<double>::infinity());
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getHLC3(1) == 115100.0);
        assert(rts.getHLC3(5) == 115100.0);
        assert(rts.getHLC3(M) == 115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getHLC3(1) == 115118.0);
        assert(rts.getHLC3(5) == 115114.0);
        assert(rts.getHLC3(M) == 115113.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getHLC3(1) == 115130.66666666667);
        assert(rts.getHLC3(5) == 115126.66666666667);
        assert(rts.getHLC3(M) == 115122.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getHLC3(1), 115133.11));
        assert(approxEqual(rts.getHLC3(5), 115132.40666666666));
        assert(approxEqual(rts.getHLC3(M), 115130.40666666666));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(approxEqual(rts.getHLC3(1), 115199.97));
        assert(approxEqual(rts.getHLC3(5), 115177.68333333335));
        assert(approxEqual(rts.getHLC3(M), 115177.68333333335));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(approxEqual(rts.getHLC3(1), 115777.03));
        assert(approxEqual(rts.getHLC3(5), 115562.39000000001));
        assert(approxEqual(rts.getHLC3(M), 115562.39000000001));
    }
}

void testGetPriceDistance(){
    {
        RollingTradeStatistics rts;
        assert(rts.getPriceDistance(1) == -std::numeric_limits<double>::infinity());
        assert(rts.getPriceDistance(3) == -std::numeric_limits<double>::infinity());
        assert(rts.getPriceDistance(M) == -std::numeric_limits<double>::infinity());
    }

    {
        RollingTradeStatistics rts;
        feed(rts, oneTradeArray);
        assert(rts.getPriceDistance(1) == 115100.0);
        assert(rts.getPriceDistance(3) == -115100.0);
        assert(rts.getPriceDistance(M) == -115100.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(rts.getPriceDistance(1) == 3.0);
        assert(rts.getPriceDistance(3) == 20.0);
        assert(rts.getPriceDistance(M) == -115076.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(rts.getPriceDistance(1) == 6.0);
        assert(rts.getPriceDistance(3) == 15.0);
        assert(rts.getPriceDistance(M) == 31.0);
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getPriceDistance(1), 0.419999999));
        assert(approxEqual(rts.getPriceDistance(3), 0.419999999));
        assert(approxEqual(rts.getPriceDistance(M), 8.110000000));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(approxEqual(rts.getPriceDistance(1), 66.860000000));
        assert(approxEqual(rts.getPriceDistance(3), 66.860000000));
        assert(approxEqual(rts.getPriceDistance(M), 66.860000000));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(approxEqual(rts.getPriceDistance(1), 110.369999999));
        assert(approxEqual(rts.getPriceDistance(3), 110.369999999));
        assert(approxEqual(rts.getPriceDistance(M), 643.919999999));
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
        assert(approxEqual(rts.getBollingerBandsDistance(5), 2.0));
        assert(approxEqual(rts.getBollingerBandsDistance(M), 2.645751311));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArray);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.601128187));
        assert(approxEqual(rts.getBollingerBandsDistance(M), 0.577484011));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, baseTradeArrayDuringRolling);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 1.705605702));
        assert(approxEqual(rts.getBollingerBandsDistance(M), 1.532812631));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, diffLargerThan1sArray);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.915512891));
        assert(approxEqual(rts.getBollingerBandsDistance(M), 0.794215743));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaEqualMaxBuckets);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 2.000000001));
        assert(approxEqual(rts.getBollingerBandsDistance(M), 2.645751314));
    }

    {
        RollingTradeStatistics rts;
        feed(rts, deltaLargerThanMaxBuckets);
        assert(approxEqual(rts.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(rts.getBollingerBandsDistance(5), 0.853451245));
        assert(approxEqual(rts.getBollingerBandsDistance(M), 1.285739280));
    }
}

int main() {
	// tests below workz only with MAX_BUCKETS = 8 set in RollingTradeStatistics.h
    // std::cout << std::fixed << std::setprecision(13);

    testBaseCase();
    testGetBuySellTradeCount();
    testGetHiLoPrice();
    testGetCumBuyAndSellQuantity();
    testGetOldestPrice();
    testGetPriceDifference();
    testGetClosePrice();
    testGetBiggestBuySell();
    testGetSMA();
    testGetHLC3();
    testGetPriceDistance();
    testGetBollingerBandsDistance();

    return 0;
}
