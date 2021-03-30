#ifndef BEMBEL_BASE_UTILS_CONVERSION_HPP
#define BEMBEL_BASE_UTILS_CONVERSION_HPP

#include <bembel/library.hpp>

namespace bembel::base::conversion {

BEMBEL_API bool fromString(const std::string&, glm::vec2&);
BEMBEL_API bool fromString(const std::string&, glm::vec3&);
BEMBEL_API bool fromString(const std::string&, glm::vec4&);

template <typename T>
bool fromString(const std::string&, T&);

template <typename T>
bool fromString(const std::string& str, T& value) {
    std::istringstream ss(str);
    ss >> value;
    char c;
    if(ss.fail() || ss.bad() || ss.get(c)) return false;
    return true;
}

} // namespace bembel::base::conversion
#endif // include guards
