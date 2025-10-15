module;
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
export module glm;

import std;

export namespace glm {
    using ::glm::tvec2;
    using ::glm::tvec3;
    using ::glm::tvec4;

    using ::glm::vec2;
    using ::glm::vec3;
    using ::glm::vec4;

    using ::glm::dvec2;
    using ::glm::dvec3;
    using ::glm::dvec4;

    using ::glm::ivec2;
    using ::glm::ivec3;
    using ::glm::ivec4;

    using ::glm::uvec2;
    using ::glm::uvec3;
    using ::glm::uvec4;

    using ::glm::quat;

    using ::glm::mat2;
    using ::glm::mat3;
    using ::glm::mat4;

    using ::glm::dmat2;
    using ::glm::dmat3;
    using ::glm::dmat4;

    using ::glm::operator+;
    using ::glm::operator-;
    using ::glm::operator*;
    using ::glm::operator/;
    using ::glm::operator==;

    using ::glm::mat3_cast;
    using ::glm::mat4_cast;

    using ::glm::lookAt;
    using ::glm::ortho;
    using ::glm::perspective;
    using ::glm::rotate;
    using ::glm::scale;
    using ::glm::translate;

    using ::glm::cross;
    using ::glm::dot;
    using ::glm::length;
    using ::glm::normalize;

    using ::glm::affineInverse;
    using ::glm::inverse;

    using ::glm::angleAxis;
    using ::glm::cos;
    using ::glm::radians;
    using ::glm::sin;

    using ::glm::clamp;
    using ::glm::max;
    using ::glm::min;

    using ::glm::mix;

    using ::glm::value_ptr;
} // namespace glm

export template <>
struct std::formatter<glm::vec2> : std::formatter<std::string> {
    auto format(glm::vec2 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::vec3> : std::formatter<std::string> {
    auto format(glm::vec3 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::vec4> : std::formatter<std::string> {
    auto format(glm::vec4 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<glm::dvec2> : std::formatter<std::string> {
    auto format(glm::dvec2 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::dvec3> : std::formatter<std::string> {
    auto format(glm::dvec3 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::dvec4> : std::formatter<std::string> {
    auto format(glm::dvec4 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<glm::ivec2> : std::formatter<std::string> {
    auto format(glm::ivec2 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::ivec3> : std::formatter<std::string> {
    auto format(glm::ivec3 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::ivec4> : std::formatter<std::string> {
    auto format(glm::ivec4 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};

export template <>
struct std::formatter<glm::uvec2> : std::formatter<std::string> {
    auto format(glm::uvec2 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {})", v.x, v.y), ctx);
    }
};
export template <>
struct std::formatter<glm::uvec3> : std::formatter<std::string> {
    auto format(glm::uvec3 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {})", v.x, v.y, v.z), ctx);
    }
};
export template <>
struct std::formatter<glm::uvec4> : std::formatter<std::string> {
    auto format(glm::uvec4 v, format_context& ctx) {
        return formatter<string>::format(std::format("({}; {}; {}; {})", v.x, v.y, v.z, v.w), ctx);
    }
};
