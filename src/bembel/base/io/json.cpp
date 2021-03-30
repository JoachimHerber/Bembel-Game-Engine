/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/
#include "json.hpp"

namespace glm {

void to_json(nlohmann::json& j, const vec2& v) {
  j = nlohmann::json{{"x", v.x}, {"y", v.y}};
}
void to_json(nlohmann::json& j, const vec3& v) {
  j = nlohmann::json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
void to_json(nlohmann::json& j, const vec4& v) {
  j = nlohmann::json{{"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w}};
}
void to_json(nlohmann::json& j, const ivec2& v) {
  j = nlohmann::json{{"x", v.x}, {"y", v.y}};
}
void to_json(nlohmann::json& j, const ivec3& v) {
  j = nlohmann::json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
void to_json(nlohmann::json& j, const ivec4& v) {
  j = nlohmann::json{{"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w}};
}

void from_json(const nlohmann::json& j, vec2& v) {
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
}
void from_json(const nlohmann::json& j, vec3& v) {
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
}
void from_json(const nlohmann::json& j, vec4& v) {
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
  j.at("w").get_to(v.w);
}
void from_json(const nlohmann::json& j, ivec2& v) {
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
}
void from_json(const nlohmann::json& j, ivec3& v) {
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
}
void from_json(const nlohmann::json& j, ivec4& v) {
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
  j.at("w").get_to(v.w);
}

} // namespace bembel::base
