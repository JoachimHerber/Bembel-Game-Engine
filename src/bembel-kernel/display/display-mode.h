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

#ifndef BEMBEL_KERNEL_DISPLAY_DISPLAYMODE_H_
#define BEMBEL_KERNEL_DISPLAY_DISPLAYMODE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <bembel-base/xml.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/

struct GLFWwindow;

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API DisplayModeBase
{
public:
	DisplayModeBase() = default;
	virtual ~DisplayModeBase() = default;

	virtual GLFWwindow* CreatWindow(const std::string&, GLFWwindow*) = 0;
};

class BEMBEL_API WindowDisplayMode : public DisplayModeBase
{
public:
	WindowDisplayMode() = default;
	~WindowDisplayMode() = default;

	int  GetWidth() const;
	void SetWidth(int);

	int  GetHeight() const;
	void SetHeight(int);

	bool GetIsResizable() const;
	void SetIsResizable(bool);

	GLFWwindow* CreatWindow(const std::string&, GLFWwindow*) override;

	static std::unique_ptr<WindowDisplayMode>
		CreateInstance(const xml::Element*);

	static const std::string& GetTypeName();
private:
	int width_ = 800;
	int height_ = 600;

	bool limit_size_ = false;
	glm::uvec2 min_size_;
	glm::uvec2 max_size_;

	unsigned aspect_ratio_numer_ = 0;
	unsigned aspect_ratio_denom_ = 0;

	bool resizable_ = true;
};

class BEMBEL_API FullscreenDisplayMode : public DisplayModeBase
{
public:
	FullscreenDisplayMode();
	~FullscreenDisplayMode() = default;

	glm::ivec2 GetResolution() const;
	void SetResolution(glm::ivec2 val);

	int GetRefreshRate() const;
	void SetRefreshRate(int val);

	GLFWwindow* CreatWindow(const std::string&, GLFWwindow*) override;

	static std::unique_ptr<FullscreenDisplayMode>
		CreateInstance(const xml::Element*);

	static const std::string& GetTypeName();

private:
	glm::ivec2 resolution_;

	int refresh_rate_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
