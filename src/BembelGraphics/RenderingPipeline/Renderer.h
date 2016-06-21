#ifndef BEMBEL_RENDERER_H
#define BEMBEL_RENDERER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>
#include <array>
/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class EntityManager;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Renderer
{
public:
	Renderer(){}
	virtual ~Renderer(){}

	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	using EntityManagerPtr = std::shared_ptr<EntityManager>;
	virtual void SetEntityManager(EntityManagerPtr)
	{}

	using ViewFrustum = std::array<glm::vec4,6>;
	virtual void DoGeometryPass(
		const glm::mat4& view, 
		const glm::mat4& proj, 
		const ViewFrustum& frustum) = 0;
	virtual void DoShadowPass(
		const glm::mat4& view,
		const glm::mat4& proj,
		const ViewFrustum& frustum) = 0;
private:
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
