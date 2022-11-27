module;
#include "bembel/pch.h"
module bembel.base;

namespace bembel::base {

namespace utf8 {
    constexpr Encoding getEncoding(CodeUnit cu) noexcept {
        if((cu & 0b1000'0000) == 0) return Encoding::SINGLE_BYTE_CHARACTER;
        if((cu & 0b0100'0000) == 0) return Encoding::CONTINUATION_BYTE;
        if((cu & 0b0010'0000) == 0) return Encoding::LEADING_BYTE_OF_2_BYTE_SEQUENCE;
        if((cu & 0b0001'0000) == 0) return Encoding::LEADING_BYTE_OF_3_BYTE_SEQUENCE;
        if((cu & 0b0000'1000) == 0) return Encoding::LEADING_BYTE_OF_4_BYTE_SEQUENCE;
        return Encoding::INVALID;
    }

    std::u8string_view encode(CodePoint pc) noexcept {
        thread_local std::array<CodeUnit, 4> buffer;
        if(pc < 0x0000'0080) {
            buffer[0] = char8_t(0b0111'1111 & pc);
            return {buffer.data(), 1};
        } else if(pc < 0x0000'0800) {
            buffer[0] = char8_t(0b1100'0000 | (0b0001'1111 & (pc >> 6)));
            buffer[0] = char8_t(0b1000'0000 | (0b0011'1111 & (pc >> 0)));
            return {buffer.data(), 2};
        } else if(pc < 0x0001'0000) {
            buffer[0] = char8_t(0b1110'0000 | (0b0000'1111 & (pc >> 12)));
            buffer[0] = char8_t(0b1000'0000 | (0b0011'1111 & (pc >> 6)));
            buffer[0] = char8_t(0b1000'0000 | (0b0011'1111 & (pc >> 0)));
            return {buffer.data(), 3};
        } else if(pc < 0x0011'0000) {
            buffer[0] = char8_t(0b1111'0000 | (0b0000'0111 & (pc >> 18)));
            buffer[0] = char8_t(0b1000'0000 | (0b0011'1111 & (pc >> 12)));
            buffer[0] = char8_t(0b1000'0000 | (0b0011'1111 & (pc >> 6)));
            buffer[0] = char8_t(0b1000'0000 | (0b0011'1111 & (pc >> 0)));
            return {buffer.data(), 4};
        }
        return {};
    }

    CodePoint decode(std::u8string_view sv) noexcept {
        if(sv.empty()) return INVALID_CODE_POINT;

        auto num_bytes = std::to_underlying(getEncoding(sv[0]));
        if(0 < num_bytes && num_bytes <= sv.size()) {
            // clang-format off
            switch(num_bytes) {
                case 1: return (sv[0] & 0x7F);
                case 2: return (sv[0] & 0x1F) <<  6 | (sv[1] & 0x3f);
                case 3: return (sv[0] & 0x0F) << 12 | (sv[1] & 0x3f) <<  6 | (sv[2] & 0x3f);
                case 4: return (sv[0] & 0x07) << 18 | (sv[1] & 0x3f) << 12 | (sv[2] & 0x3f) << 6 | (sv[3] & 0x3f);
            }
            // clang-format on
        }
        return INVALID_CODE_POINT;
    }
} // namespace utf8

utf8::CodePoint String::Iterator::operator*() {
    return m_pos < m_str.size() ? utf8::decode(m_str.substr(m_pos)) : utf8::INVALID_CODE_POINT;
}

String::Iterator& String::Iterator::operator++() {
    if(m_pos < m_str.size()) {
        auto num_bytes = std::to_underlying(utf8::getEncoding(m_str[m_pos]));
        m_pos += std::max(num_bytes, 1);
    }
    return *this;
}

String::Iterator String::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool String::isValidUtf8String() const {
    int num_continuation_bytes = 0;
    for(auto it : this->data) {
        utf8::Encoding enc = utf8::getEncoding(it);
        if(num_continuation_bytes > 0) {
            if(enc != utf8::Encoding::CONTINUATION_BYTE) return false;
            --num_continuation_bytes;
        } else {
            if(std::to_underlying(enc) <= 0) return false;
            num_continuation_bytes = std::to_underlying(enc) - 1;
        }
    }
    return num_continuation_bytes == 0;
}

std::optional<std::string> String::toLocaleString() {
    std::mbstate_t state{};
    char           buffer[MB_LEN_MAX]{};

    std::string out;
    out.reserve(this->data.size());

    for(char32_t c : *this) {
        std::size_t rc = std::c32rtomb(buffer, c, &state);
        if(rc == static_cast<std::size_t>(-1)) return {};
        out += std::string_view{buffer, rc};
    }
    return out;
}

size_t String::size() const {
    size_t             n   = 0;
    std::u8string_view str = this->data;
    while(!str.empty()) {
        int i = std::max(1, std::to_underlying(utf8::getEncoding(str[0])));
        str   = (i < str.length()) ? str.substr(i) : std::u8string_view{};
        ++n;
    }
    return n;
}

bool String::eraseCodePoint(size_t position) {
    size_t   n  = 0;
    Iterator it = this->begin();
    while(it && n < position) { ++it, ++n; }
    if(!it) return false;

    auto num_bytes = std::max(1, std::to_underlying(utf8::getEncoding(this->data[it.m_pos])));
    this->data.erase(it.m_pos, num_bytes);
    return true;
}

bool String::insertCodePoint(size_t position, utf8::CodePoint c) {
    size_t   n  = 0;
    Iterator it = this->begin();
    while(it && n < position) { ++it, ++n; }
    if(!it) return false;

    this->data.insert(it.m_pos, utf8::encode(c));
    return true;
}

String::String(In<std::string_view> str) {
    *this = str;
}
String::String(In<std::string> str) {
    *this = std::string_view(str);
}

String& String::operator=(In<std::string_view> str) {
    std::mbstate_t state{};

    this->data.reserve(str.size());

    utf8::CodePoint cp;
    while(std::size_t rc = std::mbrtoc32(&cp, str.data(), str.size(), &state)) {
        auto encoding = utf8::encode(cp);
        if(encoding.empty()) return *this;

        this->data += encoding;

        if(rc >= str.size()) break;
        str = str.substr(rc);
    }
    return *this;
}

} // namespace bembel::base
