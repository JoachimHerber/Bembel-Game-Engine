#ifndef BEMBEL_DISPLAYMANAGER_H
#define BEMBEL_DISPLAYMANAGER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelBase/Factory.hpp>

#include "DisplayMode.h"

#include <memory>
#include <vector>

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
	Kernel* _kernel;

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
