module;
#include <string>
#include <string_view>
#include <charconv>
#include <format>
export module bembel.base:Conversion;

import :Types;
import :Logger;
import :Unicode;

export namespace bembel::base::conversion {

bool fromString(In<std::string_view> str, Out<bool> value) {
    if(str.compare("true") == 0) {
        value = true;
        return true;
    }
    if(str.compare("false") == 0) {
        value = false;
        return true;
    }
    log().error("Can't convert '{}' to bool", str);
    return false;
}

bool fromString(In<std::string_view> str, Out<std::string> value) {
    value = str;
    return true;
}

bool fromString(In<std::string_view> str, Out<String> value) {
    value = str;
    return true;
}

template <typename T>
requires std::integral<T>
bool fromString(In<std::string_view> str, Out<T> value) {
    auto result = std::from_chars(str.data(), str.data() + str.size(), value);
    
    if(result.ec == std::errc::invalid_argument) {
        log().error("Can't parse string '{}'", str);
        return false;
    } else if(result.ec == std::errc::result_out_of_range) {
        log().error("This number '{}'  is to large", str);
        return false;
    }

    return true;
}

template <typename T>
requires std::floating_point<T>
bool fromString(In<std::string_view> str, T& value) {
    try{
        char* end;
        value = std::strtof(str.data(), &end);
    }catch(...){
        log().error("Can't parse string '{}'", str);
    }
    //std::string tmp = {str.begin(), str.end()};
    //auto [ptr, ec] = std::from_chars(tmp.data(), tmp.data() + tmp.size(), value);
    //log().info("fromString '{}' -> {}", tmp, value);
    //
    //if(ec == std::errc::invalid_argument) {
    //    log().error("Can't parse string '{}'", str);
    //    return false;
    //} else if(ec == std::errc::result_out_of_range) {
    //    log().error("This number '{}'  is to large", str);
    //    return false;
    //}

    return true;
}

template <typename T>
requires std::integral<T> || std::floating_point<T>
bool fromString(In<std::string_view> str, Out<tvec2<T>> value) {
    auto delim_pos = str.find(' ');
    if(delim_pos == str.npos) {
        log().error("Can't parse string '{}'", str);
        return false;
    }

    return fromString(str.substr(/*********/ 0, delim_pos), value.x)
        && fromString(str.substr(delim_pos + 1 /********/), value.y);
}

template <typename T>
requires std::integral<T> || std::floating_point<T>
bool fromString(In<std::string_view> str, Out<tvec3<T>> value) {
    auto delim_pos_0 = str.find(' ');
    if(delim_pos_0 == str.npos) {
        log().error("Can't parse string '{}'", str);
        return false;
    }
    auto delim_pos_1 = str.find(' ', delim_pos_0 + 1);
    if(delim_pos_1 == str.npos) {
        log().error("Can't parse string '{}'", str);
        return false;
    }

    return fromString(str.substr(/***********/ 0, delim_pos_0), value.x)
        && fromString(str.substr(delim_pos_0 + 1, delim_pos_1), value.y)
        && fromString(str.substr(delim_pos_1 + 1 /**********/), value.z);
}

template <typename T>
requires std::integral<T> || std::floating_point<T>
bool fromString(In<std::string_view> str, Out<tvec4<T>> value) {
    auto delim_pos_0 = str.find(' ');
    if(delim_pos_0 == str.npos) {
        log().error("Can't parse string '{}'", str);
        return false;
    }
    auto delim_pos_1 = str.find(' ', delim_pos_0 + 1);
    if(delim_pos_1 == str.npos) {
        log().error("Can't parse string '{}'", str);
        return false;
    }

    auto delim_pos_2 = str.find(' ', delim_pos_1 + 1);
    if(delim_pos_2 == str.npos) {
        log().error("Can't parse string '{}'", str);
        return false;
    }

    return fromString(str.substr(/***********/ 0, delim_pos_0), value.x)
        && fromString(str.substr(delim_pos_0 + 1, delim_pos_1), value.y)
        && fromString(str.substr(delim_pos_1 + 1, delim_pos_2), value.z)
        && fromString(str.substr(delim_pos_2 + 1 /**********/), value.w);
}

template <typename T>
std::string toString(In<T> value) {
    return std::format("{}", value);
}

std::string toString(In<vec2> value) {
    return std::format("{} {}", value.x, value.y);
}

std::string toString(In<vec3> value) {
    return std::format("{} {} {}", value.x, value.y, value.z);
}

std::string toString(In<vec4> value) {
    return std::format("{} {} {} {}", value.x, value.y, value.z, value.w);
}

} // namespace bembel::base::conversion
