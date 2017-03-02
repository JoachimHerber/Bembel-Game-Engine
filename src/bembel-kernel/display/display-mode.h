#ifndef BEMBEL_DISPLAYMODE_H
#define BEMBEL_DISPLAYMODE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelBase/XML.h>

#include <glm/glm.hpp>

#include <string>
#include <memory>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/

struct GLFWwindow;

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

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
	int _width  = 800;
	int _height = 600;

	bool _limitSize = false;
	glm::uvec2 _minSize;
	glm::uvec2 _maxSize;

	unsigned _aspectRatioNumer = 0;
	unsigned _aspectRatioDenom = 0;

	bool _resizable = true;
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
	glm::ivec2 _resolution;

	int _refreshRate;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
