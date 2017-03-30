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
