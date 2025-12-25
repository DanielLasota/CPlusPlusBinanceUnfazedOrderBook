#include <fstream>
#include <iostream>
#include <memory>
#include <functional>
#include <unordered_set>
#include <string_view>
#include <type_traits>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "MetricsStore.h"

namespace py = pybind11;

template <typename T>
static py::array_t<T> vector_to_numpy(const std::vector<T>& vec) {
    const size_t n = vec.size();
    T* data = static_cast<T*>(std::malloc(n * sizeof(T)));
    if (!data && n != 0) { throw std::bad_alloc(); }
    if (n) { std::memcpy(data, vec.data(), n * sizeof(T)); }

    py::capsule free_when_done(data, [](void* f) { std::free(f); });
    return py::array_t<T>(
        {n},
        {sizeof(T)},
        data,
        free_when_done
    );
}

namespace {
    struct ColumnBase {
        std::string name;
        virtual void push(const MetricsEntry& e) = 0;
        virtual py::object to_numpy() = 0;
        virtual ~ColumnBase() = default;
    };

    template <class MemberT, class OutT = MemberT>
    struct MemberColumn : ColumnBase {
        OutT (*convert_)(MemberT);
        MemberT MetricsEntry::* ptr;
        std::vector<OutT> data;

        MemberColumn(std::string nm,
                     MemberT MetricsEntry::* p,
                     size_t reserve,
                     OutT (*conv)(MemberT) = +[](MemberT x)-> OutT { return static_cast<OutT>(x); }) {
            name = std::move(nm);
            ptr = p;
            convert_ = conv;
            data.reserve(reserve);
        }

        void push(const MetricsEntry& e) override { data.push_back(convert_(e.*ptr)); }

        py::object to_numpy() override { return vector_to_numpy<OutT>(data); }
    };
}

py::dict MetricsStore::toNp() const {
    const size_t n = entries_.size();
    py::dict result;

    std::unordered_set<std::string_view> wanted;
    wanted.reserve(features_.size() * 2);
    for (auto const& v : features_) wanted.insert(v);

    std::vector<std::unique_ptr<ColumnBase>> cols;
    cols.reserve(features_.size());

    auto add_if = [&](std::string_view key, auto member_ptr) {
        using MemberT = std::remove_reference_t<decltype(std::declval<MetricsEntry>().*member_ptr)>;

        if (!wanted.count(key)) return;

        if constexpr (std::is_same_v<MemberT, bool>) {
            cols.emplace_back(std::make_unique<MemberColumn<bool, uint8_t>>(
                std::string(key), member_ptr, n,
                +[](bool b)-> uint8_t { return b ? 1u : 0u; }
            ));
        }
        else if constexpr (std::is_same_v<MemberT, uint8_t>) {
            cols.emplace_back(std::make_unique<MemberColumn<uint8_t>>(
                std::string(key), member_ptr, n
            ));
        }
        else if constexpr (std::is_same_v<MemberT, uint64_t>) {
            cols.emplace_back(std::make_unique<MemberColumn<uint64_t>>(
                std::string(key), member_ptr, n
            ));
        }
        else if constexpr (std::is_same_v<MemberT, double>) {
            cols.emplace_back(std::make_unique<MemberColumn<double>>(
                std::string(key), member_ptr, n
            ));
        }
        else if constexpr (std::is_same_v<MemberT, int>) {
            cols.emplace_back(std::make_unique<MemberColumn<int>>(
                std::string(key), member_ptr, n
            ));
        }
        else if constexpr (std::is_same_v<MemberT, float>) {
            cols.emplace_back(std::make_unique<MemberColumn<float>>(
                std::string(key), member_ptr, n
            ));
        }
        else { static_assert(sizeof(MemberT) == 0, "Unsupported field type in MetricsEntry"); }
    };

    #define METRIC(name, ctype) add_if(#name, &MetricsEntry::name);
    #include "detail/metrics_list.def"
    #undef METRIC

    for (const auto& e : entries_) { for (auto& c : cols) c->push(e); }

    for (auto& c : cols) { result[py::str(c->name.c_str())] = c->to_numpy(); }

    return result;
}

void MetricsStore::toCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << path << std::endl;
        return;
    }

    for (size_t i = 0; i < features_.size(); ++i) {
        file << features_[i];
        if (i + 1 < features_.size()) file << ",";
    }
    file << "\n";

    struct ColW {
        std::function<void(const MetricsEntry&, std::ostream&)> write;
    };
    std::vector<ColW> writers;
    writers.reserve(features_.size());

    auto add_writer = [&](std::string_view key, auto member_ptr) {
        using MemberT = std::remove_reference_t<decltype(std::declval<MetricsEntry>().*member_ptr)>;
        for (auto const& v : features_) {
            if (v == key) {
                if constexpr (std::is_same_v<MemberT, bool>) {
                    writers.push_back(ColW{
                        [member_ptr](auto const& e, std::ostream& os) { os << ((e.*member_ptr) ? "1" : "0"); }
                    });
                }
                else if constexpr (std::is_same_v<MemberT, uint8_t>) {
                    writers.push_back(ColW{
                        [member_ptr](auto const& e, std::ostream& os) { os << static_cast<int>(e.*member_ptr); }
                    });
                }
                else {
                    writers.push_back(ColW{
                        [member_ptr](auto const& e, std::ostream& os) { os << (e.*member_ptr); }
                    });
                }
                break;
            }
        }
    };

    #define METRIC(name, ctype) add_writer(#name, &MetricsEntry::name);
    #include "detail/metrics_list.def"
    #undef METRIC

    for (const auto &e : entries_) {
        for (size_t j = 0; j < writers.size(); ++j) {
            writers[j].write(e, file);
            if (j + 1 < writers.size()) file << ",";
        }
        file << "\n";
    }
}
