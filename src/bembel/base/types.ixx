export module bembel.base:Types;

import std;
export import glm;
export import nlohmann.json;

export namespace bembel::base {
using u8    = std::uint8_t;
using u16   = std::uint16_t;
using u32   = std::uint32_t;
using u64   = std::uint64_t;
using uint  = unsigned int;
using usize = std::size_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using byte = std::byte;

using Seconds = std::chrono::duration<double>;

using namespace ::glm;
using namespace ::nlohmann;

using ColorRGB  = ::glm::tvec3<u8>;
using ColorRGBA = ::glm::tvec4<u8>;


enum class WindowId : u32 { MAIN };

struct Fraction {
    int  numerator;
    uint denominator;

    explicit operator double() const { return double(numerator) / double(denominator); }
};

struct Exeption : public std::runtime_error {
    Exeption(std::string const& what);
    Exeption(char const* what);

    Exeption(Exeption const& other) noexcept = default;
};

template <usize N>
struct StringLiteral {
    consteval StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

    char value[N];

    constexpr operator std::string_view() const { return {value, N - 1}; }
};

// clang-format off
template <typename T>
struct PASS_BY_VALUE : std::bool_constant<std::is_trivially_copyable_v<T> && sizeof(T) < 4 * sizeof(int)>{}; // pass small trivially copyable Types by value

template <>           struct PASS_BY_VALUE<std::string_view>      : std::true_type{};
template <>           struct PASS_BY_VALUE<std::u8string_view>    : std::true_type{};
template <typename T> struct PASS_BY_VALUE<std::span<T>>          : std::true_type{};

template <typename T> using In    = std::conditional_t<PASS_BY_VALUE<T>::value, T const, T const &>;
template <typename T> using InOut = T&;
template <typename T> using Out   = T&;
template <typename T> using Move  = T&&;
template <typename T> using Copy  = T;

static_assert(std::is_same_v<In<int>,         int         const   >);
static_assert(std::is_same_v<In<int*>,        int*        const   >);
static_assert(std::is_same_v<In<std::string>, std::string const & >);
// clang-format on

template <typename T>
using Dictionary = std::map<std::string, T, std::less<>>;

template <typename T>
using not_null_ptr = T*;

// std::less<> allowes us to use std::string_views to find element in the map/Dictionary
template <typename T, typename... TArgs>
concept AllowedTypes = (std::same_as<T, TArgs> || ...);
} // namespace bembel::base

export template <>
struct std::formatter<bembel::base::WindowId> : std::formatter<std::string> {
    auto format(bembel::base::WindowId v, format_context& ctx) {
        return formatter<string>::format(std::format("{}", uint32_t(v)), ctx);
    }
};
