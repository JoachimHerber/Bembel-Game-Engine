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

#ifndef BEMBEL_INPUTEXAMPLE_H
#define BEMBEL_INPUTEXAMPLE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <bembel-kernel/application.h>
#include <bembel-kernel/events/event-manager.h>
#include <bembel-kernel/events/input-events.h>
#include <bembel-kernel/events/display-events.h>
#include <bembel-interaction/input/button.h>
#include <bembel-interaction/interaction-system.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class InputExample : public bembel::Application
{
public:
	InputExample();
	~InputExample();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void HandleEvent(const bembel::WindowShouldCloseEvent&);


	void HandleEvent(const bembel::KeyPressEvent&);
	void HandleEvent(const bembel::KeyRepeatEvent&);
	void HandleEvent(const bembel::KeyReleaseEvent&);
	void HandleEvent(const bembel::TextInputEvent&);
	void HandleEvent(const bembel::MouseButtonPressEvent&);
	void HandleEvent(const bembel::MouseButtonRepeatEvent&);
	void HandleEvent(const bembel::MouseButtonReleaseEvent&);
	void HandleEvent(const bembel::CursorMovedEvent&);
	void HandleEvent(const bembel::CursorEnteredEvent&);
	void HandleEvent(const bembel::CursorLeftEvent&);
	void HandleEvent(const bembel::ScrollEvent&);

	void HandleEvent(const bembel::ButtonPressEvent&);
	void HandleEvent(const bembel::ButtonReleaseEvent&);

private:
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
