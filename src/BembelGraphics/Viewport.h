#ifndef BEMBEL_VIEW_PORT_H
#define BEMBEL_VIEW_PORT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>
#include <memory>

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

	const glm::ivec2& GetPosition() const;
	void SetPosition(const glm::ivec2& pos);

	const glm::ivec2& GetSize() const;
	void SetSize(const glm::ivec2& size);

	void SetView(std::shared_ptr<View>);
	std::shared_ptr<View> GetView() const;

	void Enable();
	void Disable();
	bool IsEnabled();

private:
	std::shared_ptr<View> _view;

	glm::ivec2 _position;
	glm::ivec2 _size;


	bool _enabled = true;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
