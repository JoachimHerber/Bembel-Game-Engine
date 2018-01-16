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

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "interaction-system.h"
#include "input/mouse.h"
#include "input/keyboard.h"

#include <bembel-base/xml.h>
#include <bembel-base/logging/logger.h>
#include <bembel-kernel/kernel.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

InteractionSystem::InteractionSystem(Kernel* kernel)
	: System(kernel, "Interaction")
{
	_mouse    = std::make_unique<Mouse>(kernel->GetEventManager());
	_keyboard = std::make_unique<Keyboard>(kernel->GetEventManager());
}
InteractionSystem::~InteractionSystem()
{
}

bool InteractionSystem::Configure(const xml::Element* properties)
{
	if (!properties)
		return true;

	return true;
}

bool InteractionSystem::Init()
{
	return true;
}

void InteractionSystem::Shutdown()
{
}

void InteractionSystem::Update(double)
{
}

Mouse* InteractionSystem::GetMouse() const
{
	return _mouse.get();
}

Keyboard* InteractionSystem::GetKeyboard() const
{
	return _keyboard.get();
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
