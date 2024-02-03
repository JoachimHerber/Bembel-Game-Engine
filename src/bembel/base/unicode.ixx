module;
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
export module bembel.base:Unicode;

import :Types;

namespace bembel::base {

export namespace utf8 {
    using CodeUnit  = char8_t;
    using CodePoint = char32_t;

    inline constexpr CodePoint INVALID_CODE_POINT = ~CodePoint(0);

    enum class Encoding : int {
        INVALID                         = -1,
        CONTINUATION_BYTE               = 0,
        SINGLE_BYTE_CHARACTER           = 1,
        LEADING_BYTE_OF_2_BYTE_SEQUENCE = 2,
        LEADING_BYTE_OF_3_BYTE_SEQUENCE = 3,
        LEADING_BYTE_OF_4_BYTE_SEQUENCE = 4
    };

    constexpr Encoding getEncoding(CodeUnit cu) noexcept;

    std::u8string_view encode(In<CodePoint> pc) noexcept;
    CodePoint          decode(In<std::u8string_view> sv) noexcept;

    bool  isValidString(In<std::u8string_view> str);
    usize getNumCodePoints(In<std::u8string_view> str);

    bool eraseCodePoint(InOut<std::u8string> str, size_t position);
    bool insertCodePoint(InOut<std::u8string> str, size_t position, utf8::CodePoint c);

    std::optional<std::string>   toLocaleEncoding(In<std::u8string_view> str);
    std::optional<std::u8string> fromLocaleEncoding(In<std::string_view> str);

    class Iterator {
      public:
        Iterator(In<std::u8string_view> str, In<size_t> pos = 0) noexcept : m_str{str}, m_pos{pos} {}

        utf8::CodePoint operator*();
        Iterator&       operator++();
        Iterator        operator++(int);

        bool operator!=(Iterator other) {
            return m_pos != other.m_pos || m_str.size() != other.m_str.size() || m_str.data() != other.m_str.data();
        }

        operator bool() const { return m_pos < m_str.size(); };

        size_t getPosition() const { return m_pos; }

      private:
        std::u8string_view m_str;
        size_t             m_pos;
    };

    struct Iterate {
        std::u8string_view str;

        Iterator begin() { return {str}; }
        Iterator end() { return {str, str.length()}; }
    };

} // namespace utf8

} // namespace bembel::base

export std::ostream& operator<<(std::ostream& out, std::u8string_view str) {
    return out << bembel::base::utf8::toLocaleEncoding(str).value_or("");
}