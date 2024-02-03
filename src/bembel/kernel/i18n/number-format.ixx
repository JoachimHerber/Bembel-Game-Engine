module;
#include <locale>
#include <set>
#include <string>
export module bembel.kernel.i18n:NumberFormat;

import bembel.base;

namespace bembel::kernel::i18n {
using namespace bembel::base;

export class NumberFormat {
  public:
    ~NumberFormat() = default;

    void setDecimalSeparator(std::u8string_view separator) { m_decimal_separator = separator; }
    void setGroupSeparator(std::u8string_view separator) { m_group_separator = separator; }
    void setGroupSeparatorLocations(std::set<uint> locations) {
        m_group_separator_locations = locations;
    }

    void format(In<u64> value, InOut<std::u8string> str);
    void format(In<double> value, InOut<std::u8string> str);

    NumberFormat& operator=(In<json> j);

  private:
    std::u8string  m_decimal_separator         = u8".";
    std::u8string  m_group_separator           = u8"\u2009"; // half space
    std::set<uint> m_group_separator_locations = {3, 6, 9, 12, 15, 18};
};

} // namespace bembel::kernel::i18n