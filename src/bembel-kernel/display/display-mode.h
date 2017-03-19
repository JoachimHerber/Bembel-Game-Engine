#ifndef BEMBEL_KERNEL_DISPLAY_DISPLAYMODE_H_
#define BEMBEL_KERNEL_DISPLAY_DISPLAYMODE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <bembel-base/xml.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/

struct GLFWwindow;

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API DisplayModeBase
{
public:
	DisplayModeBase() = default;
	virtual ~DisplayModeBase() = default;

	virtual GLFWwindow* CreatWindow(const std::string&, GLFWwindow*) = 0;
};

class BEMBEL_API WindowDisplayMode : public DisplayModeBase
{
public:
	WindowDisplayMode() = default;
	~WindowDisplayMode() = default;

	int  GetWidth() const;
	void SetWidth(int);

	int  GetHeight() const;
	void SetHeight(int);

	bool GetIsResizable() const;
	void SetIsResizable(bool);

	GLFWwindow* CreatWindow(const std::string&, GLFWwindow*) override;

	static std::unique_ptr<WindowDisplayMode>
		CreateInstance(const xml::Element*);

	static const std::string& GetTypeName();
private:
	int width_ = 800;
	int height_ = 600;

	bool limit_size_ = false;
	glm::uvec2 min_size_;
	glm::uvec2 max_size_;

	unsigned aspect_ratio_numer_ = 0;
	unsigned aspect_ratio_denom_ = 0;

	bool resizable_ = true;
};

class BEMBEL_API FullscreenDisplayMode : public DisplayModeBase
{
public:
	FullscreenDisplayMode();
	~FullscreenDisplayMode() = default;

	glm::ivec2 GetResolution() const;
	void SetResolution(glm::ivec2 val);

	int GetRefreshRate() const;
	void SetRefreshRate(int val);

	GLFWwindow* CreatWindow(const std::string&, GLFWwindow*) override;

	static std::unique_ptr<FullscreenDisplayMode>
		CreateInstance(const xml::Element*);

	static const std::string& GetTypeName();

private:
	glm::ivec2 resolution_;

	int refresh_rate_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
