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
