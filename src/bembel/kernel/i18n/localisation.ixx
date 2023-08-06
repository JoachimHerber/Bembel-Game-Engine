module;
#include <filesystem>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
export module bembel.kernel.i18n:Localisation;

import bembel.base;
import :NumberFormat;

namespace bembel::kernel::i18n {
using namespace bembel::base;

export enum class TranslationKeyIndex : uint { INVALID = std::numeric_limits<uint>::max() };

export class Localisation {
  public:
    std::u8string_view                   translate(TranslationKeyIndex key) const;
    std::shared_ptr<const std::u8string> getTranslation(TranslationKeyIndex key) const;

    using TimePoint = std::chrono::time_point<std::chrono::utc_clock>;

    NumberFormat number_format;

    void format(In<std::u8string_view> fmt, In<i64> value, InOut<std::u8string> str);
    void format(In<std::u8string_view> fmt, In<u64> value, InOut<std::u8string> str);
    void format(In<std::u8string_view> fmt, In<double> value, InOut<std::u8string> str);
    void format(In<std::u8string_view> fmt, In<TimePoint> value, InOut<std::u8string> str);

    enum class Error {
        Ok = 0,
        FileNotFound,
        InvalidFileFormat,
        MissingTranslation,
    };
    Error load(std::filesystem::path);

    static std::vector<std::string_view> const& getUsedTranslationKeys();
    static TranslationKeyIndex                  getTranslationKeyIndex(std::string_view key);

    static std::shared_ptr<Localisation> DEFAULT;

  private:
    static std::vector<std::string_view>& getTranslationKeys();

  private:
    std::vector<std::shared_ptr<const std::u8string>> m_translations;
};

template <StringLiteral TKey>
struct TranslationKey {
    static const TranslationKeyIndex INDEX;

    operator TranslationKeyIndex() const { return INDEX; }
    operator std::u8string_view() const { return Localisation::DEFAULT->translate(INDEX); }
};
template <StringLiteral TKey>
TranslationKeyIndex const TranslationKey<TKey>::INDEX = Localisation::getTranslationKeyIndex(TKey);

namespace literals {
    export template <StringLiteral Str>
    TranslationKey<Str> operator""_i18n() {
        return {};
    }
} // namespace literals

} // namespace bembel::kernel::i18n

export bool operator!(bembel::kernel::i18n::Localisation::Error e) {
    return e != bembel::kernel::i18n::Localisation::Error::Ok;
}