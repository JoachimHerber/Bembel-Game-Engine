module;
#include "bembel/pch.h"
export module bembel.base:Types;

export import glm;

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

using ColorRGB  = glm::tvec3<u8>;
using ColorRGBA = glm::tvec4<u8>;

using glm::mat3;
using glm::mat4;

using glm::quat;

enum class WindowId : u32 { MAIN };

struct Exeption : public std::runtime_error {
    Exeption(std::string const& what, std::source_location loc = std::source_location::current());
    Exeption(char const* what, std::source_location loc = std::source_location::current());

    Exeption(Exeption const& other) noexcept = default;
};

// clang-format off
template <typename T>
inline constexpr bool PASS_BY_VALUE = std::is_trivially_copyable_v<T> && sizeof(T) < 4 * sizeof(int); // pass small trivially copyable Types by value

template <>           inline constexpr bool PASS_BY_VALUE<std::string_view>     = true;
template <>           inline constexpr bool PASS_BY_VALUE<std::u8string_view>   = true;
template <typename T> inline constexpr bool PASS_BY_VALUE<std::span<T>>         = true;
template <>           inline constexpr bool PASS_BY_VALUE<std::source_location> = true;

template <typename T> using In    = std::conditional_t<PASS_BY_VALUE<T>, T, T const&>;
template <typename T> using InOut = T&;
template <typename T> using Out   = T&;
template <typename T> using Move  = T&&;
template <typename T> using Copy  = T;
// clang-format on

template <typename T> using Dictionary = std::map<std::string, T, std::less<>>;
// std::less<> allowes us to use std::string_views to find element in the map/Dictionary

template <typename T, typename... TArgs>
concept AllowedTypes = (std::same_as<T, TArgs> || ...);
} // namespace bembel::base

using bembel::base::In;
using bembel::base::InOut;
using bembel::base::Out;
export template <> struct std::formatter<glm::vec2> : std::formatter<std::string> {
    auto format(In<glm::vec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <> struct std::formatter<glm::vec3> : std::formatter<std::string> {
    auto format(In<glm::vec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <> struct std::formatter<glm::vec4> : std::formatter<std::string> {
    auto format(In<glm::vec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <> struct std::formatter<glm::dvec2> : std::formatter<std::string> {
    auto format(In<glm::dvec2>& v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <> struct std::formatter<glm::dvec3> : std::formatter<std::string> {
    auto format(In<glm::dvec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <> struct std::formatter<glm::dvec4> : std::formatter<std::string> {
    auto format(In<glm::dvec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <> struct std::formatter<glm::ivec2> : std::formatter<std::string> {
    auto format(In<glm::ivec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <> struct std::formatter<glm::ivec3> : std::formatter<std::string> {
    auto format(In<glm::ivec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <> struct std::formatter<glm::ivec4> : std::formatter<std::string> {
    auto format(In<glm::ivec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <> struct std::formatter<glm::uvec2> : std::formatter<std::string> {
    auto format(In<glm::uvec2> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <> struct std::formatter<glm::uvec3> : std::formatter<std::string> {
    auto format(In<glm::uvec3> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <> struct std::formatter<glm::uvec4> : std::formatter<std::string> {
    auto format(In<glm::uvec4> v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <> struct std::formatter<bembel::base::WindowId> : std::formatter<std::string> {
    auto format(In<bembel::base::WindowId> v, format_context& ctx) {
        return formatter<string>::format(std::format("{}", uint32_t(v)), ctx);
    }
};
