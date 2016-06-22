#ifndef BEMBEL_WINDOW_H
#define BEMBEL_WINDOW_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelBase/XML.h>
#include <BembelBase/Events/EventManager.h>

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWwindow; 

namespace bembel{
	class DisplayManager;
	class DisplayModeBase;
} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Window
{
	Window(DisplayManager*, unsigned );
	~Window();
public:

 	std::shared_ptr<DisplayModeBase> GetDisplayMode() const;
 	void SetDisplayMode(std::shared_ptr<DisplayModeBase> val);

	void Init(const xml::Element* properties);

	void Open(const std::string& titel);
	void Close();

	unsigned GetWindowID() const;

	bool IsOpend();

	glm::ivec2 GetWindowSize() const;
	glm::ivec2 GetFrameBufferSize() const;

	bool SetIconify(bool b);
	bool SetVisible(bool b);

	bool GetShouldClose() const;
	bool SetShouldClose(bool shouldClose);

	DisplayManager* GetDisplaySystem() const;
	GLFWwindow*    GetGlfwWindow() const;

	void MakeContextCurent();
	void SwapBuffers();

private:
	DisplayManager* _displaySys;
	GLFWwindow*  _windowImp;
	std::shared_ptr<DisplayModeBase> _displayMode;

	unsigned _windowID;

	friend class DisplayManager;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
