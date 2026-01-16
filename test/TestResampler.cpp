#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include "Resampler.h"
#include "enums/XEvent.h"

namespace {
    constexpr std::array<MetricsEntry, 1> oneEntryArr = {{
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002000000ULL, .midPrice=115100.88 , .queueDiff=-5, .isAggressorAsk=true },
    }};

    constexpr std::array<MetricsEntry, 5> unfinishedWithinOneSecondArr = {{
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'000'000ULL, .midPrice=115100.88, .queueDiff= 0, .isAggressorAsk=true },
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115101.18, .queueDiff= 1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115102.22, .queueDiff= 2, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'600'000ULL, .midPrice=115120.10, .queueDiff=-1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'999'999ULL, .midPrice=115118.29, .queueDiff=-5, .isAggressorAsk=true },
    }};

    constexpr std::array<MetricsEntry, 6> finishedWithinOneSecondArr = {{
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'000'000ULL, .midPrice=115100.88, .queueDiff= 0, .isAggressorAsk=true },
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115101.18, .queueDiff= 1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115102.22, .queueDiff= 2, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'600'000ULL, .midPrice=115120.10, .queueDiff=-1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'999'999ULL, .midPrice=115118.29, .queueDiff=-5, .isAggressorAsk=true }, //1
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'000'000ULL, .midPrice=115118.99, .queueDiff=-5, .isAggressorAsk=true },
    }};

    constexpr std::array<MetricsEntry, 10> fewConsecutiveSeconds = {{
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'000'000ULL, .midPrice=115100.88, .queueDiff= 0, .isAggressorAsk=true },
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115101.18, .queueDiff= 1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115102.22, .queueDiff= 2, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'600'000ULL, .midPrice=115120.10, .queueDiff=-1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'999'999ULL, .midPrice=115118.29, .queueDiff=-5, .isAggressorAsk=true }, //1
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'000'000ULL, .midPrice=115200.00, .queueDiff=10, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'123'000ULL, .midPrice=115200.11, .queueDiff=11, .isAggressorAsk=true },
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'999'999ULL, .midPrice=115211.22, .queueDiff=12, .isAggressorAsk=false}, //2
        MetricsEntry{ .market=1, .timestampOfReceive=1760400004'000'000ULL, .midPrice=115311.33, .queueDiff=21, .isAggressorAsk=false}, //3
        MetricsEntry{ .market=1, .timestampOfReceive=1760400005'000'000ULL, .midPrice=115418.88, .queueDiff=31, .isAggressorAsk=true }, //4
    }};

    constexpr std::array<MetricsEntry, 15> fewNonConsSeconds = {{
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'000'000ULL, .midPrice=115100.88, .queueDiff= 0, .isAggressorAsk=true },
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115101.18, .queueDiff= 1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'100'999ULL, .midPrice=115102.22, .queueDiff= 2, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'600'000ULL, .midPrice=115120.10, .queueDiff=-1, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400002'999'999ULL, .midPrice=115118.29, .queueDiff=-5, .isAggressorAsk=true }, //0
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'000'000ULL, .midPrice=115200.00, .queueDiff=10, .isAggressorAsk=false},
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'123'000ULL, .midPrice=115200.11, .queueDiff=11, .isAggressorAsk=true },
        MetricsEntry{ .market=1, .timestampOfReceive=1760400003'999'999ULL, .midPrice=115211.22, .queueDiff=12, .isAggressorAsk=false}, //1
        MetricsEntry{ .market=1, .timestampOfReceive=1760400004'000'000ULL, .midPrice=115311.33, .queueDiff=21, .isAggressorAsk=false}, //2
        MetricsEntry{ .market=1, .timestampOfReceive=1760400005'000'000ULL, .midPrice=115418.88, .queueDiff=31, .isAggressorAsk=true }, //3
        MetricsEntry{ .market=1, .timestampOfReceive=1760400007'000'000ULL, .midPrice=115518.99, .queueDiff=44, .isAggressorAsk=false}, //4
        MetricsEntry{ .market=1, .timestampOfReceive=1760400007'999'999ULL, .midPrice=115518.98, .queueDiff=44, .isAggressorAsk=false}, //4
        MetricsEntry{ .market=1, .timestampOfReceive=1760400007'999'999ULL, .midPrice=115518.90, .queueDiff=1 , .isAggressorAsk=true }, //4
        MetricsEntry{ .market=1, .timestampOfReceive=1760400008'000'000ULL, .midPrice=115618.01, .queueDiff=55, .isAggressorAsk=true }, //5
        MetricsEntry{ .market=1, .timestampOfReceive=1760400010'000'000ULL, .midPrice=115818.01, .queueDiff=77, .isAggressorAsk=false},
    }};

    template <class Container>
    std::vector<MetricsEntry> collect(Resampler& r, const Container &events) {
        std::vector<MetricsEntry> out;
        for (const auto& e : events) {
            if (auto maybe = r.update(e)) { out.push_back(*maybe); }
        }
        return out;
    }
}

void testLast500ms() {
    {
        Resampler r{ResampleConfig{500'000, ResampleMethod::LAST}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{500'000, ResampleMethod::LAST}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'100'999ULL);
        assert(outs[0].midPrice == 115102.22);
        assert(outs[0].queueDiff == 2);
        assert(outs[0].isAggressorAsk == 0.0);
    }

    {
        Resampler r{ResampleConfig{500'000, ResampleMethod::LAST}};
        const auto outs = collect(r, finishedWithinOneSecondArr);

        assert(outs.size() == 2);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'100'999ULL);
        assert(outs[0].midPrice == 115102.22);
        assert(outs[0].queueDiff == 2);
        assert(outs[0].isAggressorAsk == 0.0);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400002'999'999ULL);
        assert(outs.back().midPrice == 115118.29);
        assert(outs.back().queueDiff == -5);
        assert(outs.back().isAggressorAsk == 1.0);
    }

    {
        Resampler r{ResampleConfig{500'000, ResampleMethod::LAST}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 5);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'100'999ULL);
        assert(outs[0].midPrice == 115102.22);
        assert(outs[0].queueDiff == 2);
        assert(outs[0].isAggressorAsk == 0.0);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[1].midPrice == 115118.29);
        assert(outs[1].queueDiff == -5);
        assert(outs[1].isAggressorAsk == 1.0);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400003'123'000ULL);
        assert(outs[2].midPrice == 115200.11);
        assert(outs[2].queueDiff == 11);
        assert(outs[2].isAggressorAsk == 1.);

        assert(outs[3].market == 1);
        assert(outs[3].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[3].midPrice == 115211.22);
        assert(outs[3].queueDiff == 12);
        assert(outs[3].isAggressorAsk == 0.);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400004'000'000ULL);
        assert(outs.back().midPrice == 115311.33);
        assert(outs.back().queueDiff == 21);
        assert(outs.back().isAggressorAsk == 0.);
    }

    {
        Resampler r{ResampleConfig{500'000, ResampleMethod::LAST}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 9);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'100'999ULL);
        assert(outs[0].midPrice == 115102.22);
        assert(outs[0].queueDiff == 2);
        assert(outs[0].isAggressorAsk == 0.0);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[1].midPrice == 115118.29);
        assert(outs[1].queueDiff == -5);
        assert(outs[1].isAggressorAsk == 1.0);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400003'123'000ULL);
        assert(outs[2].midPrice == 115200.11);
        assert(outs[2].queueDiff == 11);
        assert(outs[2].isAggressorAsk == 1.);

        assert(outs[3].market == 1);
        assert(outs[3].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[3].midPrice == 115211.22);
        assert(outs[3].queueDiff == 12);
        assert(outs[3].isAggressorAsk == 0.);

        assert(outs[4].market == 1);
        assert(outs[4].timestampOfReceive == 1760400004'000'000ULL);
        assert(outs[4].midPrice == 115311.33);
        assert(outs[4].queueDiff == 21);
        assert(outs[4].isAggressorAsk == 0.);

        assert(outs[5].market == 1);
        assert(outs[5].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[5].midPrice == 115418.88);
        assert(outs[5].queueDiff == 31);
        assert(outs[5].isAggressorAsk == 1.0);

        assert(outs[6].market == 1);
        assert(outs[6].timestampOfReceive == 1760400007'000'000ULL);
        assert(outs[6].midPrice == 115518.99);
        assert(outs[6].queueDiff == 44);
        assert(outs[6].isAggressorAsk == 0.0);

        assert(outs[7].market == 1);
        assert(outs[7].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[7].midPrice == 115518.90);
        assert(outs[7].queueDiff == 1);
        assert(outs[7].isAggressorAsk == 1.0);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.01);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}

void testLast1s() {
    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, finishedWithinOneSecondArr);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115118.29);
        assert(outs[0].queueDiff == -5);
        assert(outs[0].isAggressorAsk == 1.0);
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 3);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115118.29);
        assert(outs[0].queueDiff == -5);
        assert(outs[0].isAggressorAsk == 1.0);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[1].midPrice == 115211.22);
        assert(outs[1].queueDiff == 12);
        assert(outs[1].isAggressorAsk == 0.);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400004'000'000ULL);
        assert(outs.back().midPrice == 115311.33);
        assert(outs.back().queueDiff == 21);
        assert(outs.back().isAggressorAsk == 0.);
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 6);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115118.29);
        assert(outs[0].queueDiff == -5);
        assert(outs[0].isAggressorAsk == 1.0);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[1].midPrice == 115211.22);
        assert(outs[1].queueDiff == 12);
        assert(outs[1].isAggressorAsk == 0.);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400004'000'000ULL);
        assert(outs[2].midPrice == 115311.33);
        assert(outs[2].queueDiff == 21);
        assert(outs[2].isAggressorAsk == 0.);

        assert(outs[3].market == 1);
        assert(outs[3].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[3].midPrice == 115418.88);
        assert(outs[3].queueDiff == 31);
        assert(outs[3].isAggressorAsk == 1.0);

        assert(outs[4].market == 1);
        assert(outs[4].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[4].midPrice == 115518.90);
        assert(outs[4].queueDiff == 1);
        assert(outs[4].isAggressorAsk == 1.0);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.01);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}

void testLast2s() {
    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, finishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[0].midPrice == 115211.22);
        assert(outs[0].queueDiff == 12);
        assert(outs[0].isAggressorAsk == 0.);
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::LAST}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 4);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[0].midPrice == 115211.22);
        assert(outs[0].queueDiff == 12);
        assert(outs[0].isAggressorAsk == 0.);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[1].midPrice == 115418.88);
        assert(outs[1].queueDiff == 31);
        assert(outs[1].isAggressorAsk == 1.);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[2].midPrice == 115518.90);
        assert(outs[2].queueDiff == 1);
        assert(outs[2].isAggressorAsk == 1.0);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.01);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}


void testMean500ms(){}

void testMean1s() {
    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, finishedWithinOneSecondArr);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115108.5340000000142026692629);
        assert(outs[0].queueDiff == -0.6000000000000000888178);
        assert(outs[0].isAggressorAsk == 0.4);
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 3);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115108.5340000000142026692629);
        assert(outs[0].queueDiff == -0.6000000000000000888178);
        assert(outs[0].isAggressorAsk == 0.4);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[1].midPrice == 115203.77666666664);
        assert(outs[1].queueDiff == 11.0);
        assert(outs[1].isAggressorAsk == 1./3.);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400004'000'000ULL);
        assert(outs.back().midPrice == 115311.33);
        assert(outs.back().queueDiff == 21);
        assert(outs.back().isAggressorAsk == 0.);
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 6);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115108.5340000000142026692629);
        assert(outs[0].queueDiff == -0.6000000000000000888178);
        assert(outs[0].isAggressorAsk == 0.4);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[1].midPrice == 115203.77666666664);
        assert(outs[1].queueDiff == 11.0);
        assert(outs[1].isAggressorAsk == 1./3.);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400004'000'000ULL);
        assert(outs[2].midPrice == 115311.33);
        assert(outs[2].queueDiff == 21);
        assert(outs[2].isAggressorAsk == 0.);

        assert(outs[3].market == 1);
        assert(outs[3].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[3].midPrice == 115418.88);
        assert(outs[3].queueDiff == 31);
        assert(outs[3].isAggressorAsk == 1.0);

        assert(outs[4].market == 1);
        assert(outs[4].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[4].midPrice == 115518.95666666667);
        assert(outs[4].queueDiff == 29.666666666666664);
        assert(outs[4].isAggressorAsk == 1./3.);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.01);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}

void testMean2s() {
    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, finishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[0].midPrice == 115144.25);
        assert(outs[0].queueDiff == 3.75);
        assert(outs[0].isAggressorAsk == 0.375);
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 4);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[0].midPrice == 115144.25);
        assert(outs[0].queueDiff == 3.75);
        assert(outs[0].isAggressorAsk == 0.375);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[1].midPrice == 115365.10500000001);
        assert(outs[1].queueDiff == 26.0);
        assert(outs[1].isAggressorAsk == 0.5);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[2].midPrice == 115518.95666666667);
        assert(outs[2].queueDiff == 29.666666666666664);
        assert(outs[2].isAggressorAsk == 1./3.);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.01);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}


void testHLC3_500ms(){}

void testHLC3_1s() {
    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, finishedWithinOneSecondArr);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115113.09);
        assert(outs[0].queueDiff == -2.6666666666666665);
        assert(outs[0].isAggressorAsk == 2./3.);
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 3);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115113.09);
        assert(outs[0].queueDiff == -2.6666666666666665);
        assert(outs[0].isAggressorAsk == 2./3.);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[1].midPrice == 115207.48);
        assert(outs[1].queueDiff == 11.333333333333332);
        assert(outs[1].isAggressorAsk == 0.3333333333333333);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400004'000'000ULL);
        assert(outs.back().midPrice == 115311.32999999999);
        assert(outs.back().queueDiff == 21);
        assert(outs.back().isAggressorAsk == 0.);
    }

    {
        Resampler r{ResampleConfig{1'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 6);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400002'999'999ULL);
        assert(outs[0].midPrice == 115113.09);
        assert(outs[0].queueDiff == -2.6666666666666665);
        assert(outs[0].isAggressorAsk == 2./3.);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[1].midPrice == 115207.48);
        assert(outs[1].queueDiff == 11.333333333333332);
        assert(outs[1].isAggressorAsk == 0.3333333333333333);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400004'000'000ULL);
        assert(outs[2].midPrice == 115311.32999999999);
        assert(outs[2].queueDiff == 21);
        assert(outs[2].isAggressorAsk == 0.);

        assert(outs[3].market == 1);
        assert(outs[3].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[3].midPrice == 115418.88);
        assert(outs[3].queueDiff == 31);
        assert(outs[3].isAggressorAsk == 1.0);

        assert(outs[4].market == 1);
        assert(outs[4].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[4].midPrice == 115518.93000000001);
        assert(outs[4].queueDiff == 15.333333333333332);
        assert(outs[4].isAggressorAsk == 2./3);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.00999999998);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}

void testHLC3_2s() {
    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::MEAN}};
        const auto outs = collect(r, oneEntryArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, unfinishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, finishedWithinOneSecondArr);
        assert(outs.empty());
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, fewConsecutiveSeconds);

        assert(outs.size() == 1);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[0].midPrice == 115174.44);
        assert(outs[0].queueDiff == 6.333333333333333);
        assert(outs[0].isAggressorAsk == 1./3.);
    }

    {
        Resampler r{ResampleConfig{2'000'000, ResampleMethod::HLC3}};
        const auto outs = collect(r, fewNonConsSeconds);

        assert(outs.size() == 4);

        assert(outs[0].market == 1);
        assert(outs[0].timestampOfReceive == 1760400003'999'999ULL);
        assert(outs[0].midPrice == 115174.44);
        assert(outs[0].queueDiff == 6.333333333333333);
        assert(outs[0].isAggressorAsk == 1./3.);

        assert(outs[1].market == 1);
        assert(outs[1].timestampOfReceive == 1760400005'000'000ULL);
        assert(outs[1].midPrice == 115383.03);
        assert(outs[1].queueDiff == 27.666666666666664);
        assert(outs[1].isAggressorAsk == 2./3.);

        assert(outs[2].market == 1);
        assert(outs[2].timestampOfReceive == 1760400007'999'999ULL);
        assert(outs[2].midPrice == 115518.93000000001);
        assert(outs[2].queueDiff == 15.333333333333332);
        assert(outs[2].isAggressorAsk == 2./3.);

        assert(outs.back().market == 1);
        assert(outs.back().timestampOfReceive == 1760400008'000'000ULL);
        assert(outs.back().midPrice == 115618.00999999998);
        assert(outs.back().queueDiff == 55);
        assert(outs.back().isAggressorAsk == 1.0);
    }
}



int main() {
    std::cout << std::fixed << std::setprecision(22);

    testLast500ms();
    testLast1s();
    testLast2s();

    // testMean500ms(); // no time, not covered
    testMean1s();
    testMean2s();

    // testHLC3_500ms(); // no time, not covered
    testHLC3_1s();
    testHLC3_2s();
}
