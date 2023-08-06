module;
#include <array>
#include <set>
#include <string>
#include <nlohmann/json.hpp>
module bembel.kernel.i18n;

import bembel.base;

namespace bembel::kernel::i18n {
using namespace bembel::base;

NumberFormat& NumberFormat::operator=(In<json> j) {
    if(j.is_object() && j.contains("decimal_separator")) {
        m_decimal_separator = j["decimal_separator"].get<std::u8string>();
    }
    if(j.is_object() && j.contains("group_separator")) {
         m_group_separator = j["group_separator"].get<std::u8string>();
    }
    return *this;
}

void NumberFormat::format(In<u64> value, InOut<std::u8string> str) {
    // clang-format off
    static constexpr std::array<u64, 20> POWERS_OF_10 = {
                                 1,
                                10,
                               100,
                             1'000,
                            10'000,
                           100'000,
                         1'000'000,
                        10'000'000,
                       100'000'000,
                     1'000'000'000,
                    10'000'000'000,
                   100'000'000'000,
                 1'000'000'000'000,
                10'000'000'000'000,
               100'000'000'000'000,
             1'000'000'000'000'000,
            10'000'000'000'000'000,
           100'000'000'000'000'000,
         1'000'000'000'000'000'000,
        10'000'000'000'000'000'000,
    };
    // clang-format on
    if(value == 0) {
        str += u8"0";
        return;
    }

    for(uint i = 19; i-- > 0;) {
        if(value >= POWERS_OF_10[i]) {
            str += u8'0' + ((value / POWERS_OF_10[i]) % 10);
            if(m_group_separator_locations.contains(i)) str += m_group_separator;
        }
    }
}

void NumberFormat::format(In<double> value, InOut<std::u8string> str) {}

} // namespace bembel::text::i18n