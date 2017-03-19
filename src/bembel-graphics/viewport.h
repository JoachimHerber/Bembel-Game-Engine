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
		View()
		{}
		virtual ~View()
		{}

		virtual void Draw() = 0;
	};

	Viewport();
	Viewport(std::shared_ptr<View>);
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
	const glm::uvec2& GetSize() const;

	void SetView(std::shared_ptr<View>);
	std::shared_ptr<View> GetView() const;

	void UpdatePosition(const glm::vec2& frame_buffer_size);
	void UpdateSize(const glm::vec2& frame_buffer_size);

	void Enable();
	void Disable();
	bool IsEnabled();

private:
	std::shared_ptr<View> view_;

	glm::ivec2 position_;
	glm::uvec2 size_;

	glm::vec2 relativ_position_;
	glm::vec2 relativ_size_;

	glm::vec2 position_offset_;
	glm::vec2 size_offset_;

	bool enabled_ = true;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
