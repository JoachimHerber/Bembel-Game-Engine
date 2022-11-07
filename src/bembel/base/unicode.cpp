module;
#include "bembel/pch.h"
module bembel.base;

namespace bembel::base {

char32_t Utf8Decoder::operator*() {
    unsigned numBytes = getNumBytes();
    if(numBytes == 0 || m_index + numBytes > m_text.length()) return 0;

    char32_t character = m_text[m_index];
    switch(numBytes) {
        case 1: character &= 0b0111'1111; break;
        case 2: character &= 0b0001'1111; break;
        case 3: character &= 0b0000'1111; break;
        case 4: character &= 0b0000'0111; break;
        case 5: character &= 0b0000'0011; break;
        case 6: character &= 0b0000'0001; break;
    }
    for(int i = 1; i < numBytes; ++i) {
        char c = m_text[m_index + i];
        if((c & 0b1100'0000) != 0b1000'000) return 0; // invalid byte

        character = character << 6 | (c & 0b0011'1111);
    }
    return character;
}

Utf8Decoder& Utf8Decoder::operator++() {
    unsigned numBytes = getNumBytes();
    if(numBytes != 0) {
        m_index += numBytes;
    } else {
        m_index = m_text.length();
    }

    return *this;
}

uint Utf8Decoder::getNumBytes() const {
    if(m_index >= m_text.size()) return 0;

    char8_t c = m_text[m_index];

    if((c & 0b1000'0000) == 0) return 1; // one byte character
    if((c & 0b0100'0000) == 0) return 0; // follow byte of multi byte character
    if((c & 0b0010'0000) == 0) return 2; // two byte character
    if((c & 0b0001'0000) == 0) return 3; // three byte character
    if((c & 0b0000'1000) == 0) return 4; // four byte character
    if((c & 0b0000'0100) == 0) return 5; // five byte character
    if((c & 0b0000'0010) == 0) return 6; // six byte character
    return 7;                            // seven byte character
}

} // namespace bembel::base
