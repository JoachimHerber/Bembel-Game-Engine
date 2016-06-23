#ifndef BEMBEL_GEOMETRYRENDERER_H
#define BEMBEL_GEOMETRYRENDERER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>

#include <array>
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GeometryRenderer
{
public:
	GeometryRenderer()
	{}
	virtual ~GeometryRenderer()
	{}

	virtual bool Init() = 0;
	virtual bool Cleanup() = 0;

	using ViewFrustum = std::array<glm::vec4, 6>;

	virtual void DoGeometryPass(
		const glm::mat4& view,
		const glm::mat4& projection, 
		const ViewFrustum& frustum
	) = 0;
	virtual void DoDeapthPass(
		const glm::mat4& view, 
		const glm::mat4& projection, 
		const ViewFrustum& frustum
	) = 0;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
