#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>

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

    const std::array<TradeEntry, 90> cValExtendedOnly90Trades = {{
        // bullish
        {1760400000000000ULL, Symbol::BTCUSDT, 115100.0, 0.001, false, true, Market::SPOT},
        {1760400001000000ULL, Symbol::BTCUSDT, 115101.0, 0.001, true,  true, Market::SPOT},
        {1760400002000000ULL, Symbol::BTCUSDT, 115102.0, 0.001, false, true, Market::SPOT},
        {1760400003000000ULL, Symbol::BTCUSDT, 115103.0, 0.001, true,  true, Market::SPOT},
        {1760400004000000ULL, Symbol::BTCUSDT, 115104.0, 0.001, false, true, Market::SPOT},
        {1760400005000000ULL, Symbol::BTCUSDT, 115105.0, 0.001, true,  true, Market::SPOT},
        {1760400006000000ULL, Symbol::BTCUSDT, 115106.0, 0.001, false, true, Market::SPOT},
        {1760400007000000ULL, Symbol::BTCUSDT, 115107.0, 0.001, true,  true, Market::SPOT},
        {1760400008000000ULL, Symbol::BTCUSDT, 115108.0, 0.001, false, true, Market::SPOT},
        {1760400009000000ULL, Symbol::BTCUSDT, 115109.0, 0.001, true,  true, Market::SPOT},

        {1760400010000000ULL, Symbol::BTCUSDT, 115110.0, 0.001, false, true, Market::SPOT},
        {1760400011000000ULL, Symbol::BTCUSDT, 115111.0, 0.001, true,  true, Market::SPOT},
        {1760400012000000ULL, Symbol::BTCUSDT, 115112.0, 0.001, false, true, Market::SPOT},
        {1760400013000000ULL, Symbol::BTCUSDT, 115113.0, 0.001, true,  true, Market::SPOT},
        {1760400014000000ULL, Symbol::BTCUSDT, 115114.0, 0.001, false, true, Market::SPOT},
        {1760400015000000ULL, Symbol::BTCUSDT, 115115.0, 0.001, true,  true, Market::SPOT},
        {1760400016000000ULL, Symbol::BTCUSDT, 115116.0, 0.001, false, true, Market::SPOT},
        {1760400017000000ULL, Symbol::BTCUSDT, 115117.0, 0.001, true,  true, Market::SPOT},
        {1760400018000000ULL, Symbol::BTCUSDT, 115118.0, 0.001, false, true, Market::SPOT},
        {1760400019000000ULL, Symbol::BTCUSDT, 115119.0, 0.001, true,  true, Market::SPOT},

        {1760400020000000ULL, Symbol::BTCUSDT, 115120.0, 0.001, false, true, Market::SPOT},
        {1760400021000000ULL, Symbol::BTCUSDT, 115121.0, 0.001, true,  true, Market::SPOT},
        {1760400022000000ULL, Symbol::BTCUSDT, 115122.0, 0.001, false, true, Market::SPOT},
        {1760400023000000ULL, Symbol::BTCUSDT, 115123.0, 0.001, true,  true, Market::SPOT},
        {1760400024000000ULL, Symbol::BTCUSDT, 115124.0, 0.001, false, true, Market::SPOT},
        {1760400025000000ULL, Symbol::BTCUSDT, 115125.0, 0.001, true,  true, Market::SPOT},
        {1760400026000000ULL, Symbol::BTCUSDT, 115126.0, 0.001, false, true, Market::SPOT},
        {1760400027000000ULL, Symbol::BTCUSDT, 115127.0, 0.001, true,  true, Market::SPOT},
        {1760400028000000ULL, Symbol::BTCUSDT, 115128.0, 0.001, false, true, Market::SPOT},
        {1760400029000000ULL, Symbol::BTCUSDT, 115129.0, 0.001, true,  true, Market::SPOT},

        {1760400030000000ULL, Symbol::BTCUSDT, 115130.0, 0.001, false, true, Market::SPOT},
        {1760400031000000ULL, Symbol::BTCUSDT, 115131.0, 0.001, true,  true, Market::SPOT},
        {1760400032000000ULL, Symbol::BTCUSDT, 115132.0, 0.001, false, true, Market::SPOT},
        {1760400033000000ULL, Symbol::BTCUSDT, 115133.0, 0.001, true,  true, Market::SPOT},
        {1760400034000000ULL, Symbol::BTCUSDT, 115134.0, 0.001, false, true, Market::SPOT},
        {1760400035000000ULL, Symbol::BTCUSDT, 115135.0, 0.001, true,  true, Market::SPOT},
        {1760400036000000ULL, Symbol::BTCUSDT, 115136.0, 0.001, false, true, Market::SPOT},
        {1760400037000000ULL, Symbol::BTCUSDT, 115137.0, 0.001, true,  true, Market::SPOT},
        {1760400038000000ULL, Symbol::BTCUSDT, 115138.0, 0.001, false, true, Market::SPOT},
        {1760400039000000ULL, Symbol::BTCUSDT, 115139.0, 0.001, true,  true, Market::SPOT},

        {1760400040000000ULL, Symbol::BTCUSDT, 115140.0, 0.001, false, true, Market::SPOT},
        {1760400041000000ULL, Symbol::BTCUSDT, 115141.0, 0.001, true,  true, Market::SPOT},
        {1760400042000000ULL, Symbol::BTCUSDT, 115142.0, 0.001, false, true, Market::SPOT},
        {1760400043000000ULL, Symbol::BTCUSDT, 115143.0, 0.001, true,  true, Market::SPOT},
        {1760400044000000ULL, Symbol::BTCUSDT, 115144.0, 0.001, false, true, Market::SPOT},
        {1760400045000000ULL, Symbol::BTCUSDT, 115145.0, 0.001, true,  true, Market::SPOT},
        {1760400046000000ULL, Symbol::BTCUSDT, 115146.0, 0.001, false, true, Market::SPOT},
        {1760400047000000ULL, Symbol::BTCUSDT, 115147.0, 0.001, true,  true, Market::SPOT},
        {1760400048000000ULL, Symbol::BTCUSDT, 115148.0, 0.001, false, true, Market::SPOT},
        {1760400049000000ULL, Symbol::BTCUSDT, 115149.0, 0.001, true,  true, Market::SPOT},

        // --- sideways (51–70) ---
        {1760400050000000ULL, Symbol::BTCUSDT, 115149.5, 0.001, false, true, Market::SPOT},
        {1760400051000000ULL, Symbol::BTCUSDT, 115149.2, 0.001, true,  true, Market::SPOT},
        {1760400052000000ULL, Symbol::BTCUSDT, 115149.4, 0.001, false, true, Market::SPOT},
        {1760400053000000ULL, Symbol::BTCUSDT, 115149.1, 0.001, true,  true, Market::SPOT},
        {1760400054000000ULL, Symbol::BTCUSDT, 115149.3, 0.001, false, true, Market::SPOT},
        {1760400055000000ULL, Symbol::BTCUSDT, 115149.0, 0.001, true,  true, Market::SPOT},
        {1760400056000000ULL, Symbol::BTCUSDT, 115149.2, 0.001, false, true, Market::SPOT},
        {1760400057000000ULL, Symbol::BTCUSDT, 115149.4, 0.001, true,  true, Market::SPOT},
        {1760400058000000ULL, Symbol::BTCUSDT, 115149.1, 0.001, false, true, Market::SPOT},
        {1760400059000000ULL, Symbol::BTCUSDT, 115149.3, 0.001, true,  true, Market::SPOT},

        {1760400060000000ULL, Symbol::BTCUSDT, 115149.2, 0.001, false, true, Market::SPOT},
        {1760400061000000ULL, Symbol::BTCUSDT, 115149.4, 0.001, true,  true, Market::SPOT},
        {1760400062000000ULL, Symbol::BTCUSDT, 115149.1, 0.001, false, true, Market::SPOT},
        {1760400063000000ULL, Symbol::BTCUSDT, 115149.3, 0.001, true,  true, Market::SPOT},
        {1760400064000000ULL, Symbol::BTCUSDT, 115149.0, 0.001, false, true, Market::SPOT},
        {1760400065000000ULL, Symbol::BTCUSDT, 115149.2, 0.001, true,  true, Market::SPOT},
        {1760400066000000ULL, Symbol::BTCUSDT, 115149.4, 0.001, false, true, Market::SPOT},
        {1760400067000000ULL, Symbol::BTCUSDT, 115149.1, 0.001, true,  true, Market::SPOT},
        {1760400068000000ULL, Symbol::BTCUSDT, 115149.3, 0.001, false, true, Market::SPOT},
        {1760400069000000ULL, Symbol::BTCUSDT, 115149.2, 0.001, true,  true, Market::SPOT},

        // --- bearish (71–90) ---
        {1760400070000000ULL, Symbol::BTCUSDT, 115148.5, 0.001, false, true, Market::SPOT},
        {1760400071000000ULL, Symbol::BTCUSDT, 115147.8, 0.001, true,  true, Market::SPOT},
        {1760400072000000ULL, Symbol::BTCUSDT, 115147.0, 0.001, false, true, Market::SPOT},
        {1760400073000000ULL, Symbol::BTCUSDT, 115146.2, 0.001, true,  true, Market::SPOT},
        {1760400074000000ULL, Symbol::BTCUSDT, 115145.5, 0.001, false, true, Market::SPOT},
        {1760400075000000ULL, Symbol::BTCUSDT, 115144.8, 0.001, true,  true, Market::SPOT},
        {1760400076000000ULL, Symbol::BTCUSDT, 115144.0, 0.001, false, true, Market::SPOT},
        {1760400077000000ULL, Symbol::BTCUSDT, 115143.2, 0.001, true,  true, Market::SPOT},
        {1760400078000000ULL, Symbol::BTCUSDT, 115142.5, 0.001, false, true, Market::SPOT},
        {1760400079000000ULL, Symbol::BTCUSDT, 115141.8, 0.001, true,  true, Market::SPOT},

        {1760400080000000ULL, Symbol::BTCUSDT, 115141.0, 0.001, false, true, Market::SPOT},
        {1760400081000000ULL, Symbol::BTCUSDT, 115140.2, 0.001, true,  true, Market::SPOT},
        {1760400082000000ULL, Symbol::BTCUSDT, 115139.5, 0.001, false, true, Market::SPOT},
        {1760400083000000ULL, Symbol::BTCUSDT, 115138.8, 0.001, true,  true, Market::SPOT},
        {1760400084000000ULL, Symbol::BTCUSDT, 115138.0, 0.001, false, true, Market::SPOT},
        {1760400085000000ULL, Symbol::BTCUSDT, 115137.2, 0.001, true,  true, Market::SPOT},
        {1760400086000000ULL, Symbol::BTCUSDT, 115136.5, 0.001, false, true, Market::SPOT},
        {1760400087000000ULL, Symbol::BTCUSDT, 115135.8, 0.001, true,  true, Market::SPOT},
        {1760400088000000ULL, Symbol::BTCUSDT, 115135.0, 0.001, false, true, Market::SPOT},
        {1760400089000000ULL, Symbol::BTCUSDT, 115134.2, 0.001, true,  true, Market::SPOT},
    }};

    template <class Container>
    void feed(TradeRoll &tr, const Container& trades) {
        for (const auto& t : trades) tr.update(t);
    }
}

bool approxEqual(const double x, const double expected, const double eps = 1e-6) {
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
        assert(approxEqual(tr.getHLC3(1), 115130.66666666667));
        assert(approxEqual(tr.getHLC3(5), 115126.66666666667));
        assert(approxEqual(tr.getHLC3(M), 115122.0));
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
        assert(approxEqual(tr.getBollingerBandsDistance(5), 0.915514180));
        assert(approxEqual(tr.getBollingerBandsDistance(M), 0.794215743));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);
        assert(approxEqual(tr.getBollingerBandsDistance(1), 0.0));
        assert(approxEqual(tr.getBollingerBandsDistance(5), 2.000000003));
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

void testGetRFWMA(){
    {
        TradeRoll tr;

        assert(tr.getRFWMA(1, 1) == 0.0);
        assert(tr.getRFWMA(5, 1) == 0.0);
        assert(tr.getRFWMA(M, 1) == 0.0);

        assert(tr.getRFWMA(1, 2) == 0.0);
        assert(tr.getRFWMA(5, 2) == 0.0);
        assert(tr.getRFWMA(M, 2) == 0.0);

        assert(tr.getRFWMA(1, 7) == 0.0);
        assert(tr.getRFWMA(5, 7) == 0.0);
        assert(tr.getRFWMA(M, 7) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);

        assert(tr.getRFWMA(1, 1) == 115100.0);
        assert(tr.getRFWMA(5, 1) == 9591.6666666666661);
        assert(tr.getRFWMA(M, 1) == 5755.0);

        assert(tr.getRFWMA(1, 2) == 115100.00);
        assert(tr.getRFWMA(5, 2) == 28775.0);
        assert(tr.getRFWMA(M, 2) == 16442.8571428571413);

        assert(tr.getRFWMA(1, 7) == 115100.00);
        assert(tr.getRFWMA(5, 7) == 115100.00);
        assert(tr.getRFWMA(M, 7) == 57550.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        assert(tr.getRFWMA(1, 1) == 115118.0);
        assert(tr.getRFWMA(5, 1) == 115108.83333333333);
        assert(tr.getRFWMA(M, 1) == 115106.1);

        assert(tr.getRFWMA(1, 2) == 115118.0);
        assert(tr.getRFWMA(5, 2) == 115111.0);
        assert(tr.getRFWMA(M, 2) == 65777.71428571429);

        assert(tr.getRFWMA(1, 7) == 115118.0);
        assert(tr.getRFWMA(5, 7) == 115118.0);
        assert(tr.getRFWMA(M, 7) == 57559.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);
        assert(tr.getRFWMA(1, 1) == 115131.0);
        assert(tr.getRFWMA(5, 1) == 115120.83333333333);
        assert(tr.getRFWMA(8, 1) == 115120.9);

        assert(tr.getRFWMA(1, 2) == 115131.0);
        assert(tr.getRFWMA(5, 2) == 115121.5);
        assert(tr.getRFWMA(8, 2) == 115120.85714285714);

        assert(tr.getRFWMA(1, 7) == 115131.0);
        assert(tr.getRFWMA(5, 7) == 115131.0);
        assert(tr.getRFWMA(8, 7) == 115117.5);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);

        assert(tr.getRFWMA(1, 1) == 115133.11);
        assert(tr.getRFWMA(5, 1) == 115132.0208333333285);
        assert(tr.getRFWMA(M, 1) == 115131.6125);

        assert(tr.getRFWMA(1, 2) == 115133.11);
        assert(tr.getRFWMA(5, 2) == 115131.95);
        assert(tr.getRFWMA(M, 2) == 115131.54285714286);

        assert(tr.getRFWMA(1, 7) == 115133.11);
        assert(tr.getRFWMA(5, 7) == 115133.11);
        assert(tr.getRFWMA(M, 7) == 115129.055);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);

        assert(tr.getRFWMA(1, 1) == 115199.97);
        assert(tr.getRFWMA(5, 1) == 115138.6816666666564);
        assert(tr.getRFWMA(8, 1) == 115136.4530000000086);

        assert(tr.getRFWMA(1, 2) == 115199.97);
        assert(tr.getRFWMA(5, 2) == 115149.82500000001);
        assert(tr.getRFWMA(8, 2) == 115142.66142857143);

        assert(tr.getRFWMA(1, 7) == 115199.97);
        assert(tr.getRFWMA(5, 7) == 115199.97);
        assert(tr.getRFWMA(8, 7) == 115166.5400000000081);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);

        assert(tr.getRFWMA(1, 1) == 115777.03);
        assert(tr.getRFWMA(5, 1) == 115453.545);
        assert(tr.getRFWMA(8, 1) == 115325.371);

        assert(tr.getRFWMA(1, 2) == 115777.03);
        assert(tr.getRFWMA(5, 2) == 115427.47750000001);
        assert(tr.getRFWMA(8, 2) == 115301.3199999999924);

        assert(tr.getRFWMA(1, 7) == 115777.03);
        assert(tr.getRFWMA(5, 7) == 115777.03);
        assert(tr.getRFWMA(8, 7) == 115455.07);
    }
}

void testGetFWMA(){
    {
        TradeRoll tr;

        assert(tr.getFWMA(1, 1) == 0.0);
        assert(tr.getFWMA(5, 1) == 0.0);
        assert(tr.getFWMA(M, 1) == 0.0);

        assert(tr.getFWMA(1, 2) == 0.0);
        assert(tr.getFWMA(5, 2) == 0.0);
        assert(tr.getFWMA(M, 2) == 0.0);

        assert(tr.getFWMA(1, 7) == 0.0);
        assert(tr.getFWMA(5, 7) == 0.0);
        assert(tr.getFWMA(M, 7) == 0.0);
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);

        assert(tr.getFWMA(1, 1) == 115100.0);
        assert(tr.getFWMA(5, 1) == 47958.333333333336);
        assert(tr.getFWMA(M, 1) == 46040.0);

        assert(tr.getFWMA(1, 2) == 115100.00);
        assert(tr.getFWMA(5, 2) == 57550.0);
        assert(tr.getFWMA(M, 2) == 49328.57142857143);

        assert(tr.getFWMA(1, 7) == 115100.00);
        assert(tr.getFWMA(5, 7) == 115100.00);
        assert(tr.getFWMA(M, 7) == 57550.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);

        assert(tr.getFWMA(1, 1) == 115118.0);
        assert(tr.getFWMA(5, 1) == 115116.66666666667);
        assert(tr.getFWMA(M, 1) == 115116.1);

        assert(tr.getFWMA(1, 2) == 115118.0);
        assert(tr.getFWMA(5, 2) == 115114.75);
        assert(tr.getFWMA(M, 2) == 98671.0);

        assert(tr.getFWMA(1, 7) == 115118.0);
        assert(tr.getFWMA(5, 7) == 115118.0);
        assert(tr.getFWMA(M, 7) == 57559.0);
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);

        assert(tr.getFWMA(1, 1) == 115131.0);
        assert(tr.getFWMA(5, 1) == 115125.66666666667);
        assert(tr.getFWMA(M, 1) == 115125.65);

        assert(tr.getFWMA(1, 2) == 115131.0);
        assert(tr.getFWMA(5, 2) == 115124.75);
        assert(tr.getFWMA(M, 2) == 115124.14285714286);

        assert(tr.getFWMA(1, 7) == 115131.0);
        assert(tr.getFWMA(5, 7) == 115131.0);
        assert(tr.getFWMA(M, 7) == 115117.5);
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);

        assert(tr.getFWMA(1, 1) == 115133.11);
        assert(tr.getFWMA(5, 1) == 115132.72416666667);
        assert(tr.getFWMA(M, 1) == 115132.6890000000130);

        assert(tr.getFWMA(1, 2) == 115133.11);
        assert(tr.getFWMA(5, 2) == 115132.47750000001);
        assert(tr.getFWMA(M, 2) == 115132.38714285714);

        assert(tr.getFWMA(1, 7) == 115133.11);
        assert(tr.getFWMA(5, 7) == 115133.11);
        assert(tr.getFWMA(M, 7) == 115129.055);
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);

        assert(tr.getFWMA(1, 1) == 115199.97);
        assert(tr.getFWMA(5, 1) == 115160.96833333334);
        assert(tr.getFWMA(M, 1) == 115159.854);

        assert(tr.getFWMA(1, 2) == 115199.97);
        assert(tr.getFWMA(5, 2) == 115166.5399999999936);
        assert(tr.getFWMA(M, 2) == 115161.7642857142782);

        assert(tr.getFWMA(1, 7) == 115199.97);
        assert(tr.getFWMA(5, 7) == 115199.97);
        assert(tr.getFWMA(M, 7) == 115166.54000000001);
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);

        assert(tr.getFWMA(1, 1) == 115777.03);
        assert(tr.getFWMA(5, 1) == 115668.1849999999977);
        assert(tr.getFWMA(M, 1) == 115657.45300000001);

        assert(tr.getFWMA(1, 2) == 115777.03);
        assert(tr.getFWMA(5, 2) == 115588.4574999999895);
        assert(tr.getFWMA(M, 2) == 115561.5185714285617);

        assert(tr.getFWMA(1, 7) == 115777.03);
        assert(tr.getFWMA(5, 7) == 115777.03);
        assert(tr.getFWMA(M, 7) == 115455.07);
    }
}

void testGetTrendCoefAndTValue() {

    {
        TradeRoll tr;
        auto [tVal, coef] = tr.getTrendCoefAndTValue(0);

        assert(std::isnan(tVal));
        assert(std::isnan(coef));
    }

    {
        TradeRoll tr;
        feed(tr, oneTradeArray);
        auto [tVal, coef] = tr.getTrendCoefAndTValue(0);

        assert(std::isnan(tVal));
        assert(std::isnan(coef));
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArray);
        auto [tVal, coef] = tr.getTrendCoefAndTValue(0);

        assert(approxEqual(tVal, 2.601759));
        assert(approxEqual(coef, 4.7));
    }

    {
        TradeRoll tr;
        feed(tr, baseTradeArrayDuringRolling);

        auto [tVal5Seconds, coef5Seconds] = tr.getTrendCoefAndTValue(0);
        assert(approxEqual(tVal5Seconds, 3.121107));
        assert(approxEqual(coef5Seconds, 2.9));

        auto [tVal8Seconds, coef8Seconds] = tr.getTrendCoefAndTValue(1);
        assert(approxEqual(tVal8Seconds, 3.333180));
        assert(approxEqual(coef8Seconds, 2.523810));
    }

    {
        TradeRoll tr;
        feed(tr, diffLargerThan1sArray);

        auto [tVal5Seconds, coef5Seconds] = tr.getTrendCoefAndTValue(0);
        assert(approxEqual(tVal5Seconds, 2.397999));
        assert(approxEqual(coef5Seconds, 0.422000));

        auto [tVal8Seconds, coef8Seconds] = tr.getTrendCoefAndTValue(1);
        assert(approxEqual(tVal8Seconds, 3.205678));
        assert(approxEqual(coef8Seconds, 0.856904));
    }

    {
        TradeRoll tr;
        feed(tr, deltaEqualMaxBuckets);

        auto [tVal5Seconds, coef5Seconds] = tr.getTrendCoefAndTValue(0);
        assert(approxEqual(tVal5Seconds, 1.732050));
        assert(approxEqual(coef5Seconds, 13.372000));

        auto [tVal8Seconds, coef8Seconds] = tr.getTrendCoefAndTValue(1);
        assert(approxEqual(tVal8Seconds, 1.732050));
        assert(approxEqual(coef8Seconds, 5.571666));
    }

    {
        TradeRoll tr;
        feed(tr, deltaLargerThanMaxBuckets);

        auto [tVal5Seconds, coef5Seconds] = tr.getTrendCoefAndTValue(0);
        assert(approxEqual(tVal5Seconds, 2.286294));
        assert(approxEqual(coef5Seconds, 128.783999));

        auto [tVal8Seconds, coef8Seconds] = tr.getTrendCoefAndTValue(1);
        assert(approxEqual(tVal8Seconds, 5.015628));
        assert(approxEqual(coef8Seconds, 110.826071));
    }

    {
        TradeRoll tr;
        feed(tr, cValExtendedOnly90Trades);

        auto [tVal5Seconds, coef5Seconds] = tr.getTrendCoefAndTValue(0);
        assert(approxEqual(tVal5Seconds, -49.059045));
        assert(approxEqual(coef5Seconds, -0.75));

        auto [tVal8Seconds, coef8Seconds] = tr.getTrendCoefAndTValue(1);
        assert(approxEqual(tVal8Seconds, -125.957160));
        assert(approxEqual(coef8Seconds, -0.754761));

        auto [tVal13Seconds, coef13Seconds] = tr.getTrendCoefAndTValue(2);
        assert(approxEqual(tVal13Seconds, -255.109563));
        assert(approxEqual(coef13Seconds, -0.75));

        auto [tVal21Seconds, coef21Seconds] = tr.getTrendCoefAndTValue(3);
        assert(approxEqual(tVal21Seconds, -547.986961));
        assert(approxEqual(coef21Seconds, -0.750000));

        auto [tVal34Seconds, coef34Seconds] = tr.getTrendCoefAndTValue(4);
        assert(approxEqual(tVal34Seconds, -15.728994));
        assert(approxEqual(coef34Seconds, -0.488829));

        auto [tVal55Seconds, coef55Seconds] = tr.getTrendCoefAndTValue(5);
        assert(approxEqual(tVal55Seconds, -1.352947));
        assert(approxEqual(coef55Seconds, -0.057907));

        auto [tVal89Seconds, coef89Seconds] = tr.getTrendCoefAndTValue(6);
        assert(approxEqual(tVal89Seconds, 12.816324));
        assert(approxEqual(coef89Seconds, 0.470289));
    }
}

void testGetCVal() {
    {
        TradeRoll tr;
        auto [bestTVal, bestCoef, bestTrendLength] = tr.getCVal();

        assert(std::isnan(bestTVal));
        assert(std::isnan(bestCoef));
        assert(bestTrendLength == 0);
    }

    {
        TradeRoll tr;
        feed(tr, cValExtendedOnly90Trades);

        auto [bestTVal, bestCoef, bestTrendLength] = tr.getCVal();
        assert(approxEqual(bestTVal, -547.986962));
        assert(approxEqual(bestCoef, -0.75));
        assert(approxEqual(bestTrendLength, 21));

        tr.update(TradeEntry(1760400090000000ULL, Symbol::BTCUSDT, 115180.00, 0.001, false, true, Market::SPOT));
        auto [bestTVal2, bestCoef2, bestTrendLength2] = tr.getCVal();
        assert(approxEqual(bestTVal2, 12.666243));
        assert(approxEqual(bestCoef2, 0.479275));
        assert(approxEqual(bestTrendLength2, 89));

        tr.update(TradeEntry(1760400091000000ULL, Symbol::BTCUSDT, 114030.00, 0.001, false, true, Market::SPOT));
        auto [bestTVal3, bestCoef3, bestTrendLength3] = tr.getCVal();
        assert(approxEqual(bestTVal3, -1.820551));
        assert(approxEqual(bestCoef3, -5.890327));
        assert(approxEqual(bestTrendLength3, 34));

        tr.update(TradeEntry(1760400092000000ULL, Symbol::BTCUSDT, 114031.00, 0.001, false, true, Market::SPOT));
        auto [bestTVal4, bestCoef4, bestTrendLength4] = tr.getCVal();
        assert(approxEqual(bestTVal4, -2.841833));
        assert(approxEqual(bestCoef4, -331.220000));
        assert(approxEqual(bestTrendLength4, 5));

        tr.update(TradeEntry(1760400093000000ULL, Symbol::BTCUSDT, 114032.00, 0.001, false, true, Market::SPOT));
        auto [bestTVal5, bestCoef5, bestTrendLength5] = tr.getCVal();
        assert(approxEqual(bestTVal5, -3.732146));
        assert(approxEqual(bestCoef5, -196.722619));
        assert(approxEqual(bestTrendLength5, 8));
    }
}

int main() {
    /*
    * testBaseCase() ... testGetFWMA()
    *       TradeRoll::MAX_BUCKETS == 8
    *       TradeRoll::getFWMA::MAX_FWMA_SAMPLES == 6
    *
    * testGetTrendCoefAndTValue(), testGetCVal()
    *       MAX_BUCKETS == 8192
    *       TradeRoll::getCVal set consider secondsId to 0-15 with 0 with 15
    */

    // std::cout << std::fixed << std::setprecision(2);

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
    testGetRFWMA();
    testGetFWMA();
    // testGetTrendCoefAndTValue();
    // testGetCVal();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
