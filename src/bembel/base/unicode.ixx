module;
#include "bembel/pch.h"
export module bembel.base:Unicode;

import :Types;

namespace bembel::base {

//@ToDo switch to std::u8string for strings once it's supported by std::format
export class Utf8Decoder {
  public:
    Utf8Decoder(In<std::string_view> text, In<usize> index = 0) : m_text{text}, m_index{index} {}
    Utf8Decoder(Utf8Decoder&&)      = default;
    Utf8Decoder(Utf8Decoder const&) = default;
    ~Utf8Decoder()                  = default;

    Utf8Decoder begin() { return {m_text}; }
    Utf8Decoder end() { return {m_text, m_text.length()}; }

    char32_t     operator*();
    Utf8Decoder& operator++();

    bool operator!=(Utf8Decoder const& other) { return m_index != other.m_index; }

    operator bool() const { return m_index < m_text.size(); };

  private:
    uint getNumBytes() const;

  private:
    std::string_view m_text;
    usize            m_index;
};

export namespace utf8 {

    bool toLocaleString(In<std::u8string_view> in, Out<std::string> out) {
        std::mbstate_t state{};
        char           buffer[MB_LEN_MAX]{};

        out.clear();
        out.reserve(in.size());

        /* std::c8rtomb is not yet supported by VS2022
        for(char8_t c : in) {
            std::size_t rc = std::c8rtomb(out, c, &state);
            if(rc == static_cast<std::size_t>(-1)) return false;
            out += std::string_view{buffer, rc};
        }
        /*/
        for(char32_t c : Utf8Decoder(std::string_view{(char*)in.data(), in.size()})) {
            std::size_t rc = std::c32rtomb(buffer, c, &state);
            if(rc == static_cast<std::size_t>(-1)) return false;
            out += std::string_view{buffer, rc};
        }
        //*/
        return true;
    }

    bool fromLocaleString(In<std::string_view> in, Out<std::u8string> out) {
        std::mbstate_t state{};

        out.clear();
        out.reserve(in.size());

        char32_t c32;
        while(std::size_t rc = std::mbrtoc32(&c32, in.data(), in.size(), &state)) {
            if(rc == (std::size_t)-1) return false;
            if(rc == (std::size_t)-2) return false;

            in = in.substr(rc);

            if(c32 < 0x0000'0080) {
                out += char8_t(0b0111'1111 & c32);
            } else if(c32 < 0x0000'0800) {
                out += char8_t(0b110'0000 | (0b0001'1111 & (c32 >> 6)));
                out += char8_t(0b100'0000 | (0b0011'1111 & (c32 >> 0)));
            } else if(c32 < 0x0001'0000) {
                out += char8_t(0b111'0000 | (0b0000'1111 & (c32 >> 12)));
                out += char8_t(0b100'0000 | (0b0011'1111 & (c32 >> 6)));
                out += char8_t(0b100'0000 | (0b0011'1111 & (c32 >> 0)));
            } else if(c32 < 0x0011'0000) {
                out += char8_t(0b111'1000 | (0b0000'0111 & (c32 >> 18)));
                out += char8_t(0b100'0000 | (0b0011'1111 & (c32 >> 12)));
                out += char8_t(0b100'0000 | (0b0011'1111 & (c32 >> 6)));
                out += char8_t(0b100'0000 | (0b0011'1111 & (c32 >> 0)));
            } else {
                return false;
            }
        }
        return true;
    }

} // namespace utf8
} // namespace bembel::base