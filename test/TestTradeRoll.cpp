#include <cassert>
#include <array>

#include "../include/TradeRoll.h"

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
    void feed(TradeRoll &tr, const Container& trades) {
        for (const auto& t : trades) tr.update(t);
    }
}

bool approxEqual(const double x, const double expected, const double eps = 1e-9) {
    return std::abs(x - expected) < eps;
}


void testBaseCase() {
    {
        TradeRoll tr;
        assert(tr.getLastTradeTimestamp() == 0);
        assert(tr.getNewestPrice() == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getLastTradeTimestamp() == 1760400002000000ULL);
        assert(tr.getNewestPrice() == 115100.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getLastTradeTimestamp() == 1760400007000000ULL);
        assert(tr.getNewestPrice() == 115118.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getLastTradeTimestamp() == 1760400011000001ULL);
        assert(tr.getNewestPrice() == 115131.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(tr.getLastTradeTimestamp() == 1760400017000001ULL);
        assert(tr.getNewestPrice() == 115133.11);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(tr.getLastTradeTimestamp() == 1760400025000001ULL);
        assert(tr.getNewestPrice() == 115199.97);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(tr.getLastTradeTimestamp() == 1760400029000001ULL);
        assert(tr.getNewestPrice() == 115777.03);
    }

}

void testGetBuySellTradeCount() {
    {
        TradeRoll tr;

        assert(tr.getBuySellTradeCount(1).buy == 0);
        assert(tr.getBuySellTradeCount(5).buy == 0);
        assert(tr.getBuySellTradeCount(M).buy == 0);

        assert(tr.getBuySellTradeCount(1).sell == 0);
        assert(tr.getBuySellTradeCount(5).sell == 0);
        assert(tr.getBuySellTradeCount(M).sell == 0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);

        assert(tr.getBuySellTradeCount(1).buy == 1);
        assert(tr.getBuySellTradeCount(5).buy == 1);
        assert(tr.getBuySellTradeCount(M).buy == 1);

        assert(tr.getBuySellTradeCount(1).sell == 0);
        assert(tr.getBuySellTradeCount(5).sell == 0);
        assert(tr.getBuySellTradeCount(M).sell == 0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);

        assert(tr.getBuySellTradeCount(1).buy  == 1);
        assert(tr.getBuySellTradeCount(5).buy  == 3);
        assert(tr.getBuySellTradeCount(M).buy  == 6);

        assert(tr.getBuySellTradeCount(1).sell == 0);
        assert(tr.getBuySellTradeCount(5).sell == 2);
        assert(tr.getBuySellTradeCount(M).sell == 2);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);

        assert(tr.getBuySellTradeCount(1).buy == 2);
        assert(tr.getBuySellTradeCount(5).buy == 5);
        assert(tr.getBuySellTradeCount(M).buy == 7);

        assert(tr.getBuySellTradeCount(1).sell == 0);
        assert(tr.getBuySellTradeCount(5).sell == 1);
        assert(tr.getBuySellTradeCount(M).sell == 2);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);

        assert(tr.getBuySellTradeCount(1).buy  == 1);
        assert(tr.getBuySellTradeCount(5).buy  == 1);
        assert(tr.getBuySellTradeCount(M).buy  == 4);

        assert(tr.getBuySellTradeCount(1).sell == 0);
        assert(tr.getBuySellTradeCount(5).sell == 1);
        assert(tr.getBuySellTradeCount(M).sell == 1);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);

        assert(tr.getBuySellTradeCount(1).buy  == 1);
        assert(tr.getBuySellTradeCount(5).buy  == 1);
        assert(tr.getBuySellTradeCount(M).buy  == 1);

        assert(tr.getBuySellTradeCount(1).sell == 0);
        assert(tr.getBuySellTradeCount(5).sell == 0);
        assert(tr.getBuySellTradeCount(M).sell == 0);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);

        assert(tr.getBuySellTradeCount(1).buy  == 0);
        assert(tr.getBuySellTradeCount(5).buy  == 1);
        assert(tr.getBuySellTradeCount(M).buy  == 1);

        assert(tr.getBuySellTradeCount(1).sell == 1);
        assert(tr.getBuySellTradeCount(5).sell == 1);
        assert(tr.getBuySellTradeCount(M).sell == 1);
    }
}

void testGetHiLoPrice() {
    {
        TradeRoll tr;

        assert(tr.getHiLoPrice(1).h == 0.0);
        assert(tr.getHiLoPrice(5).h == 0.0);
        assert(tr.getHiLoPrice(M).h == 0.0);

        assert(tr.getHiLoPrice(1).l == std::numeric_limits<double>::infinity());
        assert(tr.getHiLoPrice(5).l == std::numeric_limits<double>::infinity());
        assert(tr.getHiLoPrice(M).l == std::numeric_limits<double>::infinity());
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);

        assert(tr.getHiLoPrice(1).h == 115100.0);
        assert(tr.getHiLoPrice(5).h == 115100.0);
        assert(tr.getHiLoPrice(M).h == 115100.0);

        assert(tr.getHiLoPrice(1).l == 115100.0);
        assert(tr.getHiLoPrice(5).l == 115100.0);
        assert(tr.getHiLoPrice(M).l == 115100.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);

        assert(tr.getHiLoPrice(1).h == 115118.0);
        assert(tr.getHiLoPrice(5).h == 115121.0);
        assert(tr.getHiLoPrice(M).h == 115121.0);

        assert(tr.getHiLoPrice(1).l == 115118.0);
        assert(tr.getHiLoPrice(5).l == 115103.0);
        assert(tr.getHiLoPrice(M).l == 115100.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);

        assert(tr.getHiLoPrice(1).h == 115131.0);
        assert(tr.getHiLoPrice(5).h == 115131.0);
        assert(tr.getHiLoPrice(M).h == 115131.0);

        assert(tr.getHiLoPrice(1).l == 115130.0);
        assert(tr.getHiLoPrice(5).l == 115118.0);
        assert(tr.getHiLoPrice(M).l == 115104.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);

        assert(tr.getHiLoPrice(1).h == 115133.11);
        assert(tr.getHiLoPrice(5).h == 115133.11);
        assert(tr.getHiLoPrice(M).h == 115133.11);

        assert(tr.getHiLoPrice(1).l == 115133.11);
        assert(tr.getHiLoPrice(5).l == 115131.0);
        assert(tr.getHiLoPrice(M).l == 115125.0);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);

        assert(tr.getHiLoPrice(1).h == 115199.97);
        assert(tr.getHiLoPrice(5).h == 115199.97);
        assert(tr.getHiLoPrice(M).h == 115199.97);

        assert(tr.getHiLoPrice(1).l == 115199.97);
        assert(tr.getHiLoPrice(5).l == 115133.11);
        assert(tr.getHiLoPrice(M).l == 115133.11);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);

        assert(tr.getHiLoPrice(1).h == 115777.03);
        assert(tr.getHiLoPrice(5).h == 115777.03);
        assert(tr.getHiLoPrice(M).h == 115777.03);

        assert(tr.getHiLoPrice(1).l == 115777.03);
        assert(tr.getHiLoPrice(5).l == 115133.11);
        assert(tr.getHiLoPrice(M).l == 115133.11);
    }
}

void testGetCumBuyAndSellQuantity()
{
    {
        TradeRoll tr;

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0000));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0000));
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0010));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0000));
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0020));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0045));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0075));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0028));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0028));
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0020));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0065));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0090));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0018));
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0045));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0010));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0010));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0));
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);

        assert(approxEqual(tr.getCumBuySellQuantity(1).buy, 0.0000));
        assert(approxEqual(tr.getCumBuySellQuantity(5).buy, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).buy, 0.0010));

        assert(approxEqual(tr.getCumBuySellQuantity(1).sell, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(5).sell, 0.0010));
        assert(approxEqual(tr.getCumBuySellQuantity(M).sell, 0.0010));
    }
}

void testGetOldestPrice()
{
    {
       TradeRoll tr;
       assert(tr.getOldestPrice(1) == 0);
       assert(tr.getOldestPrice(5) == 0);
       assert(tr.getOldestPrice(M) == 0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getOldestPrice(1) == 115100.0);
        assert(tr.getOldestPrice(5) == 0.0);
        assert(tr.getOldestPrice(M) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getOldestPrice(1) == 115118.0);
        assert(tr.getOldestPrice(5) == 115103.0);
        assert(tr.getOldestPrice(M) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getOldestPrice(1) == 115130.0);
        assert(tr.getOldestPrice(5) == 115118.0);
        assert(tr.getOldestPrice(M) == 115104.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(tr.getOldestPrice(1) == 115133.11);
        assert(tr.getOldestPrice(5) == 115131.00);
        assert(tr.getOldestPrice(M) == 115125.0);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(tr.getOldestPrice(1) == 115199.97);
        assert(tr.getOldestPrice(5) == 115133.11);
        assert(tr.getOldestPrice(M) == 115133.11);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(tr.getOldestPrice(1) == 115777.03);
        assert(tr.getOldestPrice(5) == 115133.11);
        assert(tr.getOldestPrice(M) == 115133.11);
    }
}

void testGetPriceDifference()
{
    {
        TradeRoll tr;
        assert(tr.getPriceDifference(1) == 0);
        assert(tr.getPriceDifference(5) == 0);
        assert(tr.getPriceDifference(M) == 0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getPriceDifference(1) == 0.0);
        assert(tr.getPriceDifference(5) == 115100.0);
        assert(tr.getPriceDifference(M) == 115100.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getPriceDifference(1) == 0.0);
        assert(tr.getPriceDifference(5) == 15.0);
        assert(tr.getPriceDifference(M) == 115118.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getPriceDifference(1) == 1.0);
        assert(tr.getPriceDifference(5) == 13.0);
        assert(tr.getPriceDifference(M) == 27.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(approxEqual(tr.getPriceDifference(1), 0.0));
        assert(approxEqual(tr.getPriceDifference(5), 2.11));
        assert(approxEqual(tr.getPriceDifference(M), 8.11));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(approxEqual(tr.getPriceDifference(1), 0.0));
        assert(approxEqual(tr.getPriceDifference(5), 66.86));
        assert(approxEqual(tr.getPriceDifference(M), 66.86));
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(approxEqual(tr.getPriceDifference(1), 0.0));
        assert(approxEqual(tr.getPriceDifference(5), 643.92));
        assert(approxEqual(tr.getPriceDifference(M), 643.92));
    }
}

void testGetClosePrice()
{
    {
        TradeRoll tr;
        assert(tr.getClosePrice(1) == 0);
        assert(tr.getClosePrice(5) == 0);
        assert(tr.getClosePrice(M) == 0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getClosePrice(1) == 0.0);
        assert(tr.getClosePrice(5) == 0.0);
        assert(tr.getClosePrice(M) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getClosePrice(1) == 115121.0);
        assert(tr.getClosePrice(5) == 115102.0);
        assert(tr.getClosePrice(M) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getClosePrice(1) == 115125.0);
        assert(tr.getClosePrice(5) == 115121.0);
        assert(tr.getClosePrice(M) == 115104.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(tr.getClosePrice(1) == 115132.69);
        assert(tr.getClosePrice(5) == 115131.00);
        assert(tr.getClosePrice(M) == 115125.00);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(tr.getClosePrice(1) == 115133.11);
        assert(tr.getClosePrice(5) == 115133.11);
        assert(tr.getClosePrice(M) == 115133.11);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(tr.getClosePrice(1) == 115666.66);
        assert(tr.getClosePrice(5) == 115133.11);
        assert(tr.getClosePrice(M) == 115133.11);
    }
}

void testGetBiggestBuySell(){
    {
        TradeRoll tr;
        assert(tr.getBiggestBuySell(1).buy == 0.0);
        assert(tr.getBiggestBuySell(5).buy == 0.0);
        assert(tr.getBiggestBuySell(M).buy == 0.0);

        assert(tr.getBiggestBuySell(1).sell == 0.0);
        assert(tr.getBiggestBuySell(5).sell == 0.0);
        assert(tr.getBiggestBuySell(M).sell == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getBiggestBuySell(1).buy == 0.001);
        assert(tr.getBiggestBuySell(5).buy == 0.001);
        assert(tr.getBiggestBuySell(M).buy == 0.001);

        assert(tr.getBiggestBuySell(1).sell == 0.0);
        assert(tr.getBiggestBuySell(5).sell == 0.0);
        assert(tr.getBiggestBuySell(M).sell == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getBiggestBuySell(1).buy == 0.002);
        assert(tr.getBiggestBuySell(5).buy == 0.002);
        assert(tr.getBiggestBuySell(M).buy == 0.002);

        assert(tr.getBiggestBuySell(1).sell == 0.000);
        assert(tr.getBiggestBuySell(5).sell == 0.002);
        assert(tr.getBiggestBuySell(M).sell == 0.002);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getBiggestBuySell(1).buy == 0.001);
        assert(tr.getBiggestBuySell(5).buy == 0.002);
        assert(tr.getBiggestBuySell(M).buy == 0.002);

        assert(tr.getBiggestBuySell(1).sell == 0.000);
        assert(tr.getBiggestBuySell(5).sell == 0.001);
        assert(tr.getBiggestBuySell(M).sell == 0.001);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(tr.getBiggestBuySell(1).buy == 0.001);
        assert(tr.getBiggestBuySell(5).buy == 0.001);
        assert(tr.getBiggestBuySell(M).buy == 0.0015);

        assert(tr.getBiggestBuySell(1).sell == 0.0);
        assert(tr.getBiggestBuySell(5).sell == 0.001);
        assert(tr.getBiggestBuySell(M).sell == 0.001);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(tr.getBiggestBuySell(1).buy == 0.001);
        assert(tr.getBiggestBuySell(5).buy == 0.001);
        assert(tr.getBiggestBuySell(M).buy == 0.001);

        assert(tr.getBiggestBuySell(1).sell == 0.000);
        assert(tr.getBiggestBuySell(5).sell == 0.000);
        assert(tr.getBiggestBuySell(M).sell == 0.000);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(tr.getBiggestBuySell(1).buy == 0.000);
        assert(tr.getBiggestBuySell(5).buy == 0.001);
        assert(tr.getBiggestBuySell(M).buy == 0.001);

        assert(tr.getBiggestBuySell(1).sell == 0.001);
        assert(tr.getBiggestBuySell(5).sell == 0.001);
        assert(tr.getBiggestBuySell(M).sell == 0.001);
    }
}

void testGetPriceDistance(){
    {
        TradeRoll tr;
        assert(tr.getPriceDistance(1) == -std::numeric_limits<double>::infinity());
        assert(tr.getPriceDistance(3) == -std::numeric_limits<double>::infinity());
        assert(tr.getPriceDistance(M) == -std::numeric_limits<double>::infinity());
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getPriceDistance(1) == 115100.0);
        assert(tr.getPriceDistance(3) == -115100.0);
        assert(tr.getPriceDistance(M) == -115100.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getPriceDistance(1) == 3.0);
        assert(tr.getPriceDistance(3) == 20.0);
        assert(tr.getPriceDistance(M) == -115076.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getPriceDistance(1) == 6.0);
        assert(tr.getPriceDistance(3) == 15.0);
        assert(tr.getPriceDistance(M) == 31.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(approxEqual(tr.getPriceDistance(1), 0.419999999));
        assert(approxEqual(tr.getPriceDistance(3), 0.419999999));
        assert(approxEqual(tr.getPriceDistance(M), 8.110000000));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(approxEqual(tr.getPriceDistance(1), 66.860000000));
        assert(approxEqual(tr.getPriceDistance(3), 66.860000000));
        assert(approxEqual(tr.getPriceDistance(M), 66.860000000));
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(approxEqual(tr.getPriceDistance(1), 110.369999999));
        assert(approxEqual(tr.getPriceDistance(3), 110.369999999));
        assert(approxEqual(tr.getPriceDistance(M), 643.919999999));
    }
}

void testGetSMA(){
    {
        TradeRoll tr;
        assert(tr.getSMA(1) == 0.0);
        assert(tr.getSMA(5) == 0.0);
        assert(tr.getSMA(M) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getSMA(1) == 115100.0);
        assert(tr.getSMA(5) == 23020.0);
        assert(tr.getSMA(M) == 14387.5);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getSMA(1) == 115118.0);
        assert(tr.getSMA(5) == 115113.2);
        assert(tr.getSMA(M) == 86333.5);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getSMA(1) == 115131.0);
        assert(tr.getSMA(5) == 115123.0);
        assert(tr.getSMA(M) == 115120.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(approxEqual(tr.getSMA(1), 115133.11));
        assert(approxEqual(tr.getSMA(5), 115132.436));
        assert(approxEqual(tr.getSMA(M), 115131.1474999999773));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(tr.getSMA(1) == 115199.97);
        assert(tr.getSMA(5) == 115146.482);
        assert(tr.getSMA(M) == 115141.4675);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(approxEqual(tr.getSMA(1), 115777.03));
        assert(approxEqual(tr.getSMA(5), 115582.02400000002));
        assert(approxEqual(tr.getSMA(M), 115413.68125));
    }
}

void testGetHLC3(){
    {
        TradeRoll tr;
        assert(tr.getHLC3(1) == std::numeric_limits<double>::infinity());
        assert(tr.getHLC3(5) == std::numeric_limits<double>::infinity());
        assert(tr.getHLC3(M) == std::numeric_limits<double>::infinity());
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(tr.getHLC3(1) == 115100.0);
        assert(tr.getHLC3(5) == 115100.0);
        assert(tr.getHLC3(M) == 115100.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getHLC3(1) == 115118.0);
        assert(tr.getHLC3(5) == 115114.0);
        assert(tr.getHLC3(M) == 115113.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getHLC3(1) == 115130.66666666667);
        assert(tr.getHLC3(5) == 115126.66666666667);
        assert(tr.getHLC3(M) == 115122.0);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(approxEqual(tr.getHLC3(1), 115133.11));
        assert(approxEqual(tr.getHLC3(5), 115132.40666666666));
        assert(approxEqual(tr.getHLC3(M), 115130.40666666666));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(approxEqual(tr.getHLC3(1), 115199.97));
        assert(approxEqual(tr.getHLC3(5), 115177.68333333335));
        assert(approxEqual(tr.getHLC3(M), 115177.68333333335));
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(approxEqual(tr.getHLC3(1), 115777.03));
        assert(approxEqual(tr.getHLC3(5), 115562.39000000001));
        assert(approxEqual(tr.getHLC3(M), 115562.39000000001));
    }
}

void testGetBollingerBandsDistance(){
    {
        TradeRoll tr;
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(10), 0.0));
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 2.0));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 2.645751311));
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 0.601128187));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 0.577484011));
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 1.705605702));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 1.532812631));
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 0.915512891));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 0.794215743));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 2.000000001));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 2.645751314));
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 0.853451245));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 1.285739280));
    }
}

int main() {
	// tests below workz only with MAX_BUCKETS = 8 set in TradeRoll.h
    // std::cout << std::fixed << std::setprecision(13);

    testBaseCase();
    testGetBuySellTradeCount();
    testGetHiLoPrice();
    testGetCumBuyAndSellQuantity();
    testGetOldestPrice();
    testGetPriceDifference();
    testGetClosePrice();
    testGetBiggestBuySell();
    testGetPriceDistance();
    testGetSMA();
    testGetHLC3();
    testGetBollingerBandsDistance();

    return 0;
}
