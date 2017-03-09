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
	std::shared_ptr<bembel::InteractionSystem>  _interactionSys;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
