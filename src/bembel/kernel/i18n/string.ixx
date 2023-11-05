module;
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>
export module bembel.kernel.i18n:String;

import bembel.base;
import :Localisation;

namespace bembel::kernel::i18n {
using namespace bembel::base;

export template <typename T>
concept ValidStringArg = std::is_arithmetic_v<T> || std::is_nothrow_convertible_v<T, std::u8string_view>
                      || std::is_nothrow_convertible_v<T, std::chrono::time_point<std::chrono::utc_clock>>;

export template <ValidStringArg... TArgs>
    requires(sizeof...(TArgs) <= 32)
class String {
  private:
    using Params = std::tuple<In<TArgs>...>;

    template <std::size_t N>
    using Param = std::tuple_element_t<N, Params>;

  public:
    String() = default;
    String(TranslationKeyIndex key, std::shared_ptr<Localisation> local = Localisation::DEFAULT)
      : m_local(local), m_fmt_str(local ? local->getTranslation(key) : nullptr) {
        parseFormatString();
    }
    String(std::u8string_view str, std::shared_ptr<Localisation> local = Localisation::DEFAULT)
      : m_local(local), m_fmt_str(std::make_shared<std::u8string>(str)) {
        parseFormatString();
    }
    String(String const& other)            = default;
    String(String&& other)                 = default;
    String& operator=(String const& other) = default;
    String& operator=(String&& other)      = default;

    std::u8string operator()(In<TArgs>... args) const {
        if(!m_fmt_str) return u8"";
        Params        params{args...};
        std::u8string str;
        str.reserve(m_fmt_str->size());
        for(auto& fmt : m_formater) { fmt->format(params, str); }
        return str;
    }

  private:
    class Formater {
      public:
        virtual ~Formater() = default;

        virtual void format(In<Params> parms, InOut<std::u8string> str) = 0;
    };

    template <std::size_t N>
    class ValueFormater : public Formater {
      public:
        ValueFormater(std::u8string_view fmt, Localisation* local) : m_fmt(fmt), m_local(local) {}

        virtual void format(In<Params> parms, InOut<std::u8string> str) override {
            if(m_local) {
                if constexpr(std::is_integral_v<Param<N>>)
                    m_local->format(m_fmt, i64(std::get<N>(parms)), str);
                else
                    m_local->format(m_fmt, std::get<N>(parms), str);
            }
        }

      private:
        std::u8string_view m_fmt;
        Localisation*      m_local;
    };

    template <std::size_t N>
    class PluralFormFormater : public Formater {
      public:
        PluralFormFormater(In<std::u8string_view> singular, In<std::u8string_view> plural)
          : m_singular(singular), m_plural(plural) {}

        virtual void format(In<Params> parms, InOut<std::u8string> str) override {
            str += (std::get<N>(parms) != 1 ? m_plural : m_singular);
        }

      private:
        std::u8string_view m_singular;
        std::u8string_view m_plural;
    };

    class SubStringFormater : public Formater {
      public:
        SubStringFormater(In<std::u8string_view> str) : m_str(str) {}

        virtual void format(In<Params>, InOut<std::u8string> str) override { str += m_str; }

      private:
        std::u8string_view m_str;
    };

    void parseFormatString() {
        m_formater.clear();
        if(!m_fmt_str) return;
        std::u8string_view str             = *m_fmt_str;
        auto               opening_bracket = str.find(u8"{");
        while(opening_bracket != std::u8string_view::npos) {
            auto formater = std::make_shared<SubStringFormater>(str.substr(0, opening_bracket));
            if(formater) {
                m_formater.push_back(std::move(formater));
                str = str.substr(opening_bracket + 1);
            } else {
                logError("Invalid placeholder for formater in");
                m_formater.clear();
                m_formater.push_back(std::make_shared<SubStringFormater>(*m_fmt_str));
                return;
            }

            if(auto closing_bracket = str.find(u8"}"); closing_bracket != std::u8string_view::npos) {
                m_formater.push_back(createFormater(str.substr(0, closing_bracket)));
                str = str.substr(closing_bracket + 1);
            } else {
                logError("Format String is missing closing '}'");
                m_formater.clear();
                m_formater.push_back(std::make_shared<SubStringFormater>(*m_fmt_str));
                return;
            }
            opening_bracket = str.find(u8"{");
        }
        if(!str.empty()) m_formater.push_back(std::make_shared<SubStringFormater>(str));
    }
    std::shared_ptr<Formater> createFormater(std::u8string_view placeholder) {
        std::size_t index = 0;
        while(!placeholder.empty() && '0' <= placeholder[0] && placeholder[0] <= '9') {
            index *= 10;
            index += placeholder[0] - '0';
            placeholder = placeholder.substr(1);
        }
        switch(index) {
            case 1: return createFormaterForParam<0>(placeholder);
            case 2: return createFormaterForParam<1>(placeholder);
            case 3: return createFormaterForParam<2>(placeholder);
            case 4: return createFormaterForParam<3>(placeholder);
            case 5: return createFormaterForParam<4>(placeholder);
            case 6: return createFormaterForParam<5>(placeholder);
            case 7: return createFormaterForParam<6>(placeholder);
            case 8: return createFormaterForParam<7>(placeholder);
            case 9: return createFormaterForParam<8>(placeholder);
            case 10: return createFormaterForParam<9>(placeholder);
            case 11: return createFormaterForParam<10>(placeholder);
            case 12: return createFormaterForParam<11>(placeholder);
            case 13: return createFormaterForParam<12>(placeholder);
            case 14: return createFormaterForParam<13>(placeholder);
            case 15: return createFormaterForParam<14>(placeholder);
            case 16: return createFormaterForParam<15>(placeholder);
            case 17: return createFormaterForParam<16>(placeholder);
            case 18: return createFormaterForParam<17>(placeholder);
            case 19: return createFormaterForParam<18>(placeholder);
            case 20: return createFormaterForParam<19>(placeholder);
            case 21: return createFormaterForParam<20>(placeholder);
            case 22: return createFormaterForParam<21>(placeholder);
            case 23: return createFormaterForParam<22>(placeholder);
            case 24: return createFormaterForParam<23>(placeholder);
            case 25: return createFormaterForParam<24>(placeholder);
            case 26: return createFormaterForParam<25>(placeholder);
            case 27: return createFormaterForParam<26>(placeholder);
            case 28: return createFormaterForParam<27>(placeholder);
            case 29: return createFormaterForParam<28>(placeholder);
            case 30: return createFormaterForParam<29>(placeholder);
            case 31: return createFormaterForParam<30>(placeholder);
            case 32: return createFormaterForParam<31>(placeholder);
        }
        return nullptr;
    }

    template <std::size_t N>
    std::shared_ptr<Formater> createFormaterForParam(std::u8string_view placeholder) {
        if constexpr(N < sizeof...(TArgs)) {
            if(placeholder.empty() || placeholder[0] == ':') {
                return std::make_shared<ValueFormater<N>>(
                    placeholder.empty() ? placeholder : placeholder.substr(1), m_local.get()
                );
            }
            if(placeholder[0] == '|') {
                if(placeholder.size() <= 1) return nullptr;
                if constexpr(!std::is_integral_v<Param<N>>) {
                    logError(
                        "Argument '{}' of i18n::String<> is not an integer and therefor can't be used for "
                        "selecting "
                        "plural form",
                        N + 1
                    );
                    return nullptr;
                }

                auto pos = placeholder.find(u8"|", 1);

                std::u8string_view singular = placeholder.substr(1, pos - 1);
                std::u8string_view plural   = placeholder.substr(pos + 1);
                return std::make_shared<PluralFormFormater<N>>(singular, plural);
            }
        }
        return nullptr;
    }

  private:
    std::shared_ptr<Localisation>          m_local;
    std::shared_ptr<const std::u8string>   m_fmt_str;
    std::vector<std::shared_ptr<Formater>> m_formater;
};

} // namespace bembel::text::i18n
