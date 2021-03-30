/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2020 Joachim Herber                                     * */
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
/* *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/
#ifndef BEMBEL_BASE_IO_JSON_H
#define BEMBEL_BASE_IO_JSON_H

#include <bembel/library.h>

#include <nlohmann/json.hpp>

namespace bembel::base {

using json = nlohmann::json;

} // namespace bembel::base

namespace glm {

BEMBEL_API void to_json(nlohmann::json& j, const vec2& v);
BEMBEL_API void to_json(nlohmann::json& j, const vec3& v);
BEMBEL_API void to_json(nlohmann::json& j, const vec4& v);
BEMBEL_API void to_json(nlohmann::json& j, const ivec2& v);
BEMBEL_API void to_json(nlohmann::json& j, const ivec3& v);
BEMBEL_API void to_json(nlohmann::json& j, const ivec4& v);

BEMBEL_API void from_json(const nlohmann::json& j, vec2& v);
BEMBEL_API void from_json(const nlohmann::json& j, vec3& v);
BEMBEL_API void from_json(const nlohmann::json& j, vec4& v);
BEMBEL_API void from_json(const nlohmann::json& j, ivec2& v);
BEMBEL_API void from_json(const nlohmann::json& j, ivec3& v);
BEMBEL_API void from_json(const nlohmann::json& j, ivec4& v);

} // namespace glm
#endif // include guard
