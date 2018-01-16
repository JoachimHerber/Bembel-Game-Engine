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

#ifndef BEMBEL_GRAPHICS_VIEWPORT_H_
#define BEMBEL_GRAPHICS_VIEWPORT_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Viewport final
{
public:
	class BEMBEL_API View
	{
	public:
		View()
		{}
		virtual ~View()
		{}

		virtual void Draw(
			const glm::ivec2& viewport_position,
			const glm::uvec2& viewport_size
		) = 0;
	};

	Viewport();
	Viewport(std::shared_ptr<View>);
	~Viewport();

	const glm::vec2& GetRelativPosition() const;
	void SetRelativPosition(const glm::vec2& val);
	const glm::vec2& GetRelativSize() const;
	void SetRelativSize(const glm::vec2& val);

	const glm::vec2& GetPositionOffset() const;
	void SetPositionOffset(const glm::vec2& val);
	const glm::vec2& GetSizeOffset() const;
	void SetSizeOffset(const glm::vec2& val);

	const glm::ivec2& GetPosition() const;
	const glm::uvec2& GetSize() const;

	void SetView(std::shared_ptr<View>);
	std::shared_ptr<View> GetView() const;

	void UpdatePosition(const glm::vec2& frame_buffer_size);
	void UpdateSize(const glm::vec2& frame_buffer_size);

	void Enable();
	void Disable();
	bool IsEnabled();

private:
	std::shared_ptr<View> view_;

	glm::ivec2 position_;
	glm::uvec2 size_;

	glm::vec2 relativ_position_;
	glm::vec2 relativ_size_;

	glm::vec2 position_offset_;
	glm::vec2 size_offset_;

	bool enabled_ = true;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
