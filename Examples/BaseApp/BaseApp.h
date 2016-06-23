#ifndef FONTCONVERTER_H
#define FONTCONVERTER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <BembelKernel/Application.h>
#include <BembelKernel/Display/Window.h>
#include <BembelKernel/Events/DisplayEvents.h>
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class BaseApp : public bembel::Application
{
public:
	BaseApp();
	~BaseApp();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double timeDelta) override;

	void HandleEvent(const bembel::WindowShouldCloseEvent&);
	void HandleEvent(const bembel::WindowUpdateEvent&);
};

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
