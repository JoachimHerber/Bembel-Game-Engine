#ifndef BEMBEL_LIGHTSOURCE_H
#define BEMBEL_LIGHTSOURCE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API LightSource
{
public:
	enum TYPE
	{
		POINT_LIGHT,
		DIRECTIONAL_LIGHT,
		SPOT_LIGHT
	};

public:
	LightSource();
	~LightSource();

private:
	glm::vec3 _pos;
	glm::vec3 _dir;

	glm::vec3 _color;
	float _intensety;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

