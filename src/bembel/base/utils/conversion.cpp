#include "./conversion.hpp"

namespace bembel::base::conversion {

bool fromString(const std::string& str, glm::vec2& value) {
    std::istringstream ss(str);
    char c;
    ss >> value.x >> c >> value.y;
    if(ss.fail() || ss.bad() || ss.get(c)) return false;
    return true;
}

bool fromString(const std::string& str, glm::vec3& value) {
    std::istringstream ss(str);
    char c;
    ss >> value.x >> c >> value.y >> c >> value.z;
    if(ss.fail() || ss.bad() || ss.get(c)) return false;
    return true;
}

bool fromString(const std::string& str, glm::vec4& value) {
    std::istringstream ss(str);
    char c;
    ss >> value.x >> c >> value.y >> c >> value.z >> c >> value.w;
    if(ss.fail() || ss.bad() || ss.get(c)) return false;
    return true;
}

} // namespace bembel::base::conversion
