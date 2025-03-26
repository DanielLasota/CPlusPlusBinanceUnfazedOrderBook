#include <cstdint>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <string>

// namespace py = pybind11;

struct OrderBookEntry {
    int64_t TimestampOfReceive;
    std::string Stream;
    std::string EventType;
    int64_t EventTime;
    std::string Symbol;
    int64_t FirstUpdateId;
    int64_t FinalUpdateId;
    bool IsAsk;
    double Price;
    double Quantity;
};

std::vector<OrderBookEntry> get_sample_data() {
    return {
        {1741046400062959, "trxusdt@depth@100ms", "depthUpdate", 1741046400062507, "TRXUSDT", 5434434562, 5434434566, false, 0.2311, 804627.7},
        {1741046400062959, "trxusdt@depth@100ms", "depthUpdate", 1741046400062507, "TRXUSDT", 5434434562, 5434434566, true,  0.2313, 260867.4},
        {1741046400062959, "trxusdt@depth@100ms", "depthUpdate", 1741046400062507, "TRXUSDT", 5434434562, 5434434566, true,  0.2314, 557393.7},
        {1741046400062959, "trxusdt@depth@100ms", "depthUpdate", 1741046400062507, "TRXUSDT", 5434434562, 5434434566, true,  0.2332, 15137.1},
        {1741046400163097, "trxusdt@depth@100ms", "depthUpdate", 1741046400162664, "TRXUSDT", 5434434567, 5434434567, true,  0.2314, 556957.2}
    };
}

std::string get_first_symbol() {
    std::vector<OrderBookEntry> data = get_sample_data();
    if (!data.empty()) {
        return data[0].Symbol;
    }
    return "";
}

PYBIND11_MODULE(orderbook, m) {
    py::class_<OrderBookEntry>(m, "OrderBookEntry")
        .def_readonly("TimestampOfReceive", &OrderBookEntry::TimestampOfReceive)
        .def_readonly("Stream", &OrderBookEntry::Stream)
        .def_readonly("EventType", &OrderBookEntry::EventType)
        .def_readonly("EventTime", &OrderBookEntry::EventTime)
        .def_readonly("Symbol", &OrderBookEntry::Symbol)
        .def_readonly("FirstUpdateId", &OrderBookEntry::FirstUpdateId)
        .def_readonly("FinalUpdateId", &OrderBookEntry::FinalUpdateId)
        .def_readonly("IsAsk", &OrderBookEntry::IsAsk)
        .def_readonly("Price", &OrderBookEntry::Price)
        .def_readonly("Quantity", &OrderBookEntry::Quantity);

    m.def("get_sample_data", &get_sample_data, "Returns sample order book data");
    m.def("get_first_symbol", &get_first_symbol, "Returns the symbol of the first order book entry");
}

int main() {
    std::string firstSymbol = get_first_symbol();
    std::cout << "Pierwszy symbol: " << firstSymbol << std::endl;

    auto data = get_sample_data();
    for (const auto& entry : data) {
        std::cout << "Timestamp: " << entry.TimestampOfReceive
                  << ", Symbol: " << entry.Symbol
                  << ", Price: " << entry.Price
                  << ", Quantity: " << entry.Quantity << std::endl;
    }

    return 0;
}
