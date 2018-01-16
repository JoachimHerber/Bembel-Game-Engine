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

#ifndef BEMBEL_RENDERING_EXAMPLE_H
#define BEMBEL_RENDERING_EXAMPLE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "camera-controle.h"

#include <bembel-kernel/application.h>
#include <bembel-kernel/events/display-events.h>
#include <bembel-kernel/scene/scene.h>
#include <bembel-graphics/graphic-system.h>
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class RenderingExample : public bembel::Application
{
public:
	RenderingExample();
	~RenderingExample();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void HandleEvent(const bembel::WindowShouldCloseEvent&);

private:
	bembel::GraphicSystem* graphic_system_;

	std::unique_ptr<CameraControle> cam_;

	std::vector<std::shared_ptr<bembel::Scene>> scenes_;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
