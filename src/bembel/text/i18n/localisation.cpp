module;
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
module bembel.text.i18n;

import bembel.base;

namespace bembel::text::i18n {
using namespace bembel::base;

std::shared_ptr<Localisation> Localisation::DEFAULT = std::make_shared<Localisation>();

std::u8string_view Localisation::translate(TranslationKeyIndex key) const {
    auto index = std::to_underlying(key);
    if(index < m_translations.size() && m_translations[index]) { return *m_translations[index]; }

    log().warning("Localisation doesn't provide translation for: {}", index);
    return u8"";
}

std::shared_ptr<const std::u8string> Localisation::getTranslation(TranslationKeyIndex key) const {
    auto index = std::to_underlying(key);
    if(index < m_translations.size()) { return m_translations[index]; }

    log().warning("Localisation doesn't provide translation for: {}", index);
    return nullptr;
}

void Localisation::format(In<std::u8string_view> fmt, In<i64> value, InOut<std::u8string> str) {
    if(value < 0) {
        str += u8'-';
        format(fmt, u64(-value), str);
    } else {
        format(fmt, u64(value), str);
    }
}
void Localisation::format(In<std::u8string_view> fmt, In<u64> value, InOut<std::u8string> str) {
    number_format.format(value, str);
}
void Localisation::format(In<std::u8string_view> fmt, In<double> value, InOut<std::u8string> str) {
    number_format.format(value, str);
}
void Localisation::format(In<std::u8string_view> fmt, In<TimePoint> value, InOut<std::u8string> str) {}

Localisation::Error Localisation::load(std::filesystem::path file_path) {
    if(!std::filesystem::exists(file_path)) return Error::FileNotFound;

    using u8ifstream = std::basic_ifstream<char8_t, std::char_traits<char8_t>>;

    u8ifstream file{file_path, std::ios::in | std::ios::binary};

    if(!file) return Error::FileNotFound;

    auto& keys = getTranslationKeys();
    m_translations.resize(keys.size());

    std::unordered_map<std::string, std::size_t> key_to_index;
    for(std::size_t i = 0; i < keys.size(); ++i) { key_to_index.emplace(keys[i], i); }

    std::u8string line;
    while(std::getline(file, line)) {

        auto n = line.find(':');
        if(n == std::u8string::npos) continue;

        auto key   = utf8::toLocaleEncoding(std::u8string_view(line).substr(0, n));
        auto value = std::u8string_view(line).substr(n + 1);

        if(!key) { return Error::InvalidFileFormat; }
        while(value.starts_with(' ')) value = value.substr(1);

        if(value.ends_with(u8"\r")) value = value.substr(0, value.size() - 1);

        if(key == "number_format.decimal_separator") {
            number_format.setDecimalSeparator(value);
            continue;
        }
        if(key == "number_format.group_separator") {
            number_format.setGroupSeparator(value);
            continue;
        }
        if(key == "number_format.group_separator_locations") {
            //number_format.setGroupSeparator(value); // @ToDo
            continue;
        }

        auto it = key_to_index.find(key.value());
        if(it != key_to_index.end()) {
            m_translations[it->second] = std::make_shared<std::u8string>(value);
        } else {
            m_translations.emplace_back(std::make_shared<std::u8string>(value));
            key_to_index.emplace(key.value(), keys.size());
            keys.push_back(key.value());
        }
    }
    return Error::Ok;
}

std::vector<std::string_view> const& Localisation::getUsedTranslationKeys() {
    return getTranslationKeys();
}

TranslationKeyIndex Localisation::getTranslationKeyIndex(std::string_view key) {
    auto& keys = getTranslationKeys();
    for(unsigned i = 0; i < keys.size(); ++i) {
        if(keys[i] == key) return TranslationKeyIndex(i);
    }
    keys.push_back(key);
    return TranslationKeyIndex(keys.size() - 1);
}

std::vector<std::string_view>& Localisation::getTranslationKeys() {
    static std::vector<std::string_view> keys;
    return keys;
}

} // namespace bembel::text::i18n