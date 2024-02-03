module;
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
module bembel.kernel.i18n;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel::i18n {
using namespace bembel::base;

std::shared_ptr<Localisation> Localisation::DEFAULT = std::make_shared<Localisation>();

std::u8string_view Localisation::translate(TranslationKeyIndex key) const {
    auto index = std::to_underlying(key);
    if(index < m_translations.size() && m_translations[index]) { return *m_translations[index]; }

    if(index < getTranslationKeys().size()) {
        logWarning("Localisation doesn't provide translation for: {}", getTranslationKeys()[index]);
    } else {
        logWarning("Invalid TranslationKeyIndex: {}", index);
    }
    return u8"";
}

std::shared_ptr<const std::u8string> Localisation::getTranslation(TranslationKeyIndex key) const {
    auto index = std::to_underlying(key);
    if(index < m_translations.size()) { return m_translations[index]; }

    logWarning("Localisation doesn't provide translation for: {}", index);
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
void Localisation::format(
    In<std::u8string_view> fmt, In<TimePoint> value, InOut<std::u8string> str
) {}

Localisation::Error Localisation::load(std::filesystem::path file_path) {
    if(!std::filesystem::exists(file_path)) {
        logError("File {} doesn't exist", file_path.string());
        return Error::FileNotFound;
    }

    std::ifstream file{file_path, std::ios::in | std::ios::binary};

    if(!file) {
        logError("Couldn't open file {}", file_path.string());
        return Error::FileNotFound;
    }

    auto&                                        keys = getTranslationKeys();
    std::unordered_map<std::string, std::size_t> key_to_index;
    for(std::size_t i = 0; i < keys.size(); ++i) { key_to_index.emplace(keys[i], i); }

    try {
        nlohmann::json root;
        file >> root;

        if(!root.is_object()) return Error::FileNotFound;

        if(root.contains("local")) {
            auto& local = root["local"];
            if(local.is_object() && local.contains("number_format"))
                this->number_format = local["number_format"];
        }
        auto& translations = root["translations"];

        m_translations.clear();
        m_translations.resize(keys.size());
        [&](this const auto self, std::string path, nlohmann::json const& j) -> void {
            if(j.is_object()) {
                for(auto& [key, value] : j.items()) {
                    if(path.empty())
                        self(key, value);
                    else
                        self(path + "." + key, value);
                }
            } else {
                auto it   = key_to_index.find(path);
                auto text = std::make_shared<std::u8string>(j.get<std::u8string>());
                if(it != key_to_index.end()) {
                    m_translations[it->second] = std::move(text);

                } else {
                    m_translations.emplace_back(std::move(text));
                    key_to_index.emplace(path, keys.size());
                    keys.push_back(path);
                }
            }
        }("", translations);
    } catch(nlohmann::json::exception e) {
        logError("Error while parsing {}: {}", file_path.string(), e.what());
        return Error::FileNotFound;
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

void Localisation::init(std::span<std::string_view> args, std::filesystem::path base_dir) {
    if(auto it = std::find(args.begin(), args.end(), "-local");
       it != args.end() && ++it != args.end()) {
        DEFAULT->load(base_dir / std::format("{}.json", *it));
    } else {
        DEFAULT->load(base_dir / "en.json");
    }
}

std::vector<std::string_view>& Localisation::getTranslationKeys() {
    static std::vector<std::string_view> keys;
    return keys;
}

} // namespace bembel::kernel::i18n