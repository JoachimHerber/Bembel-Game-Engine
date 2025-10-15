module;
#include <nlohmann/json.hpp>
export module nlohmann.json;

import std;

namespace nlohmann {
inline namespace NLOHMANN_JSON_NAMESPACE_CONCAT(
    NLOHMANN_JSON_ABI_TAGS, NLOHMANN_JSON_NAMESPACE_VERSION
) {

namespace detail {
export using NLOHMANN_JSON_NAMESPACE::detail::json_sax_dom_callback_parser;
export using NLOHMANN_JSON_NAMESPACE::detail::unknown_size;
} // namespace detail

export using NLOHMANN_JSON_NAMESPACE::adl_serializer;
export using NLOHMANN_JSON_NAMESPACE::basic_json;
export using NLOHMANN_JSON_NAMESPACE::json;
export using NLOHMANN_JSON_NAMESPACE::json_pointer;
export using NLOHMANN_JSON_NAMESPACE::ordered_json;
export using NLOHMANN_JSON_NAMESPACE::ordered_map;
export using NLOHMANN_JSON_NAMESPACE::to_string;

} // namespace NLOHMANN_JSON_NAMESPACE_CONCAT(NLOHMANN_JSON_ABI_TAGS,NLOHMANN_JSON_NAMESPACE_VERSION)
}  // namespace nlohmann