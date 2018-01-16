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

#ifndef BEMBEL_KERNEL_DISPLAY_DISPLAYMANAGER_H_
#define BEMBEL_KERNEL_DISPLAY_DISPLAYMANAGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "display-mode.h"

#include <memory>
#include <vector>

#include <bembel-base/factory.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWwindow;

namespace bembel{

class Kernel;
class Window;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API DisplayManager
{
public:
	DisplayManager(Kernel*);
	~DisplayManager();

	Window* CreateWindow();
	Window* GetWindow(unsigned id) const;
	unsigned GetNumWindows() const;

	Kernel* GetKernel() const;
	Factory<DisplayModeBase>& GetDisplayModeFactory();

	bool CreateWindows(const xml::Element* properties);
	void CloseOpenWindows();
	void DeleteAllWindows();

	void UpdateWindows();

public:
	void OnWindowOpend(Window*);
	void OnWindowClosed(Window*);

private:
	Kernel* kernel_;

	std::vector<std::unique_ptr<Window>> windows_;
	std::vector<Window*> open_windows_;

	Factory<DisplayModeBase> display_mode_factory_;

	friend class bembel::Window;
};

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
