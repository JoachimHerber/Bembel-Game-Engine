/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
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

#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_CAMERA_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_CAMERA_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <array>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Camera final
{
public:
	Camera();
	~Camera();

	const glm::vec3& GetPosition() const;
	void SetPosition(const glm::vec3&);

	const glm::quat& GetOrientation() const;
	void SetOrientation(const glm::quat&);

	void SetUpProjection(float fildOfFiew, float aspectRation, float near, float far);

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;

	const glm::mat4& GetinverseViewMatrix() const;
	const glm::mat4& GetinverseProjectionMatrix() const;

	const std::array<glm::vec4, 6>& GetViewFrustum() const;


private:
	glm::vec3 position_;
	glm::quat oriantation_;

	glm::mat4 view_matrix_;
	glm::mat4 proj_matrix_;

	glm::mat4 inverse_view_matrix_;
	glm::mat4 inverse_proj_matrix_;

	// clipping planes in order: left, right, bottom, top, near, far
	std::array<glm::vec4, 6> view_frustum_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
