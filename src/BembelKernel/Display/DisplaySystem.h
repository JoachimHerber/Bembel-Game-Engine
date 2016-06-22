#ifndef BEMBEL_INPUTSYSTEM_H
#define BEMBEL_INPUTSYSTEM_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include "../Engine/System.h"
#include <BembelBase/Factory.hpp>

#include "DisplayMode.h"

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWwindow;

namespace bembel{

class EventManager;
class Window;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API DisplaySystem : public System
{
public:
	DisplaySystem(std::shared_ptr<EventManager>);
	~DisplaySystem();

	Window* CreateWindow();
	Window* GetWindow(unsigned id) const;
	unsigned GetNumWindows() const;

	EventManager* GetEventManager() const;
	Factory<DisplayModeBase>& GetDisplayModeFactory();

	virtual bool Init() override;
	virtual bool Init(const xml::Element* properties) override;
	virtual void Shutdown() override;
	virtual void Update(double) override;

public:
	void OnWindowOpend(Window*);
	void OnWindowClosed(Window*);

private:
	std::vector<Window*> _windows;
	std::vector<Window*> _openWindows;

	Factory<DisplayModeBase> _displayModeFactory;

	friend class bembel::Window;
};

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
