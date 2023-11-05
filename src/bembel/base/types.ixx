module;
#include <cstdint>
#include <filesystem>
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <source_location>
#include <span>
#include <stdexcept>
#include <string_view>
export module bembel.base:Types;

export namespace bembel::base {
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using u64   = uint64_t;
using uint  = unsigned int;
using usize = size_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using byte = std::byte;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::dvec2;
using glm::dvec3;
using glm::dvec4;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::uvec2;
using glm::uvec3;
using glm::uvec4;

using glm::tvec2;
using glm::tvec3;
using glm::tvec4;

using ColorRGB  = glm::tvec3<u8>;
using ColorRGBA = glm::tvec4<u8>;

using glm::mat3;
using glm::mat4;

using glm::operator+;
using glm::operator-;
using glm::operator*;
using glm::operator/;
using glm::operator==;

using glm::max;
using glm::min;

using glm::quat;

using nlohmann::json;

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
template <>           struct PASS_BY_VALUE<std::source_location>  : std::true_type{};
template <>           struct PASS_BY_VALUE<std::filesystem::path> : std::false_type{};
template <>           struct PASS_BY_VALUE<nlohmann::json>        : std::false_type{};

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

using bembel::base::In;
using bembel::base::InOut;
using bembel::base::Out;
export template <>
struct std::formatter<glm::vec2> : std::formatter<std::string> {
    auto format(In<glm::vec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::vec3> : std::formatter<std::string> {
    auto format(In<glm::vec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::vec4> : std::formatter<std::string> {
    auto format(In<glm::vec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<glm::dvec2> : std::formatter<std::string> {
    auto format(In<glm::dvec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::dvec3> : std::formatter<std::string> {
    auto format(In<glm::dvec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::dvec4> : std::formatter<std::string> {
    auto format(In<glm::dvec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<glm::ivec2> : std::formatter<std::string> {
    auto format(In<glm::ivec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::ivec3> : std::formatter<std::string> {
    auto format(In<glm::ivec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::ivec4> : std::formatter<std::string> {
    auto format(In<glm::ivec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<glm::uvec2> : std::formatter<std::string> {
    auto format(In<glm::uvec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::uvec3> : std::formatter<std::string> {
    auto format(In<glm::uvec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::uvec4> : std::formatter<std::string> {
    auto format(In<glm::uvec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<bembel::base::WindowId> : std::formatter<std::string> {
    auto format(In<bembel::base::WindowId> v, format_context& ctx) {
        return formatter<string>::format(std::format("{}", uint32_t(v)), ctx);
    }
};

// partial specializations for nlohmann::json
export template <>
struct nlohmann::adl_serializer<std::u8string> {
    static void to_json(json& j, std::u8string const& str) {
        j = std::string_view((char const*)str.data(), str.size());
    }

    static void from_json(json const& j, std::u8string& str) {
        std::string tmp = j.get<std::string>();
        str             = std::u8string_view((char8_t const*)tmp.data(), tmp.size());
    }
};
