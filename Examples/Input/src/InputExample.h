#ifndef BEMBEL_INPUTEXAMPLE_H
#define BEMBEL_INPUTEXAMPLE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <BembelKernel/Application.h>
#include <BembelKernel/Events/EventManager.h>
#include <BembelKernel/Events/InputEvents.h>
#include <BembelKernel/Events/DisplayEvents.h>


/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class InputExample : public Application
{
public:
	InputExample();
	~InputExample();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void HandleEvent(const WindowShouldCloseEvent&);


	void HandleEvent(const KeyPressEvent&);
	void HandleEvent(const KeyRepeatEvent&);
	void HandleEvent(const KeyReleaseEvent&);
	void HandleEvent(const TextInputEvent&);
	void HandleEvent(const MouseButtonPressEvent&);
	void HandleEvent(const MouseButtonRepeatEvent&);
	void HandleEvent(const MouseButtonReleaseEvent&);
	void HandleEvent(const CursorMovedEvent&);
	void HandleEvent(const CursorEnteredEvent&);
	void HandleEvent(const CursorLeftEvent&);
	void HandleEvent(const ScrollEvent&);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
