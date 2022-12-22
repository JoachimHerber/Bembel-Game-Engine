module;
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

    std::u8string_view encode(CodePoint pc) noexcept;
    CodePoint          decode(std::u8string_view sv) noexcept;
} // namespace utf8

export class String {
  public:
    String() = default;
    template <typename T>
        requires std::is_convertible_v<T, std::u8string>
    String(T&& str) : data{std::forward<T>(str)} {}
    String(In<std::u8string_view> str) : data{str} {}
    String(In<std::string_view> str);
    String(In<std::string> str);

    template <std::convertible_to<std::u8string> T>
    String& operator=(T&& str) {
        this->data = std::forward<T>(str);
        return *this;
    }
    String& operator=(std::string_view str);

//    operator std::u8string const&() { return this->data; }
//    operator std::u8string_view() { return this->data; }

    class Iterator {
        friend class ::bembel::base::String;

      public:
        Iterator(In<std::u8string_view> str, size_t pos = 0) noexcept : m_str{str}, m_pos{pos} {}

        utf8::CodePoint operator*();
        Iterator&       operator++();
        Iterator        operator++(int);

        bool operator!=(Iterator other) {
            return m_pos != other.m_pos || m_str.size() != other.m_str.size()
                || m_str.data() != other.m_str.data();
        }

        operator bool() const { return m_pos < m_str.size(); };

      private:
        std::u8string_view m_str;
        size_t             m_pos;
    };
    Iterator begin() const noexcept { return Iterator{this->data}; }
    Iterator end() const noexcept { return {this->data, this->data.size()}; }

    bool isValidUtf8String() const;

    std::optional<std::string> toLocaleString();

    size_t size() const;

    bool eraseCodePoint(size_t position);
    bool insertCodePoint(size_t position, utf8::CodePoint c);

  public:
    std::u8string data = u8"";
};

export inline std::strong_ordering operator<=>(String const& a, String const& b) noexcept {
    return a.data <=> b.data;
}

export inline bool operator==(String const& a, String const& b) noexcept {
    return a.data == b.data;
}

} // namespace bembel::base
