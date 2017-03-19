#ifndef BEMBEL_KERNEL_DISPLAY_WINDOW_H_
#define BEMBEL_KERNEL_DISPLAY_WINDOW_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <map>

#include <glm/glm.hpp>

#include <bembel-base/xml.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWwindow;

namespace bembel {
class DisplayManager;
class DisplayModeBase;
} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Window
{
public:
	Window(DisplayManager*, unsigned);
	~Window();

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
	bool SetShouldClose(bool should_close);

	DisplayManager* GetDisplayManager() const;
	GLFWwindow*    GetGlfwWindow() const;

	void MakeContextCurent();
	void SwapBuffers();

private:
	DisplayManager* display_system_;
	GLFWwindow*  window_imp_;
	std::shared_ptr<DisplayModeBase> display_mode_;

	unsigned window_id_;

	friend class DisplayManager;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
