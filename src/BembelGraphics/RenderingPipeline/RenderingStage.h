#ifndef BEMBEL_RENDERINGSTAGE_H
#define BEMBEL_RENDERINGSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <memory>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class RenderingPipeline;
class EntityManager;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API RenderingStage
{
public:
	RenderingStage(RenderingPipeline* pipline)
		: _pipline(pipline)
	{}
	virtual ~RenderingStage()
	{}

	using EntityManagerPtr = std::shared_ptr<EntityManager>;
	virtual void SetEntityManager(EntityManagerPtr)
	{}

	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void DoRendering() = 0;

protected:
	RenderingPipeline* _pipline;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards