#ifndef BEMBEL_GRAPHICS_VIEWPORT_H_
#define BEMBEL_GRAPHICS_VIEWPORT_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Viewport final
{
public:
	class BEMBEL_API View
	{
	public:
		View(){}
		virtual ~View(){}

		virtual void Draw() = 0;
	};

	Viewport();
	Viewport(std::shared_ptr<View>);
	Viewport(std::shared_ptr<View>, const glm::ivec2& pos, const glm::ivec2& size);
	~Viewport();

	const glm::vec2& GetRelativPosition() const;
	void SetRelativPosition(const glm::vec2& val);
	const glm::vec2& GetRelativSize() const;
	void SetRelativSize(const glm::vec2& val);

	const glm::vec2& GetPositionOffset() const;
	void SetPositionOffset(const glm::vec2& val);
	const glm::vec2& GetSizeOffset() const;
	void SetSizeOffset(const glm::vec2& val);

	const glm::ivec2& GetPosition() const;
	const glm::ivec2& GetSize() const;

	void SetView(std::shared_ptr<View>);
	std::shared_ptr<View> GetView() const;

	void UpdatePosition(const glm::vec2& frameBufferSize);
	void UpdateSize(const glm::vec2& frameBufferSize);

	void Enable();
	void Disable();
	bool IsEnabled();

private:
	std::shared_ptr<View> _view;

	glm::ivec2 _position;
	glm::ivec2 _size;

	glm::vec2 _relativPosition;
	glm::vec2 _relativSize;

	glm::ivec2 _positionOffset;
	glm::ivec2 _sizeOffset;

	bool _enabled = true;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
