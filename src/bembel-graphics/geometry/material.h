#ifndef BEMBEL_GRAPHICS_GEOMETRY_MATERIAL_H_
#define BEMBEL_GRAPHICS_GEOMETRY_MATERIAL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-open-gl.h"
#include "bembel-config.h"

#include <glm/glm.hpp>

#include <bembel-kernel/assets/asset-manager.h>
#include <bembel-kernel/assets/asset-loader.h>
#include <bembel-kernel/assets/asset-handle.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class MaterialLoader;
class GraphicSystem;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Material final
{
public:
	using Color             = glm::vec3;
	using DefaultLoaderType = MaterialLoader;

public:
	Material( unsigned renderer, GLint );
	~Material();

	unsigned GetRenderer() const;
	GLuint GetUniformBufferObject() const;

	const static std::string& GetTypeName();

private:
	unsigned _renderer = 0;

	GLuint _uniformBufferObject;
}; 

class BEMBEL_API MaterialLoader final : public AssetLoaderBase
{
public:
	using ContainerType = AssetContainer<Material>;

	MaterialLoader( AssetManager* , ContainerType*, GraphicSystem* );
	virtual ~MaterialLoader();

	virtual AssetHandle RequestAsset( const std::string& filename ) override;
	virtual AssetHandle RequestAsset( const xml::Element* properties ) override;
	virtual bool ReleaseAsset( AssetHandle assetHandel ) override;

	virtual void Update() override;

	virtual void  ResetProgess()    override;
	virtual float GetProgress()     override;
	virtual bool  LoadingFinished() override;

private:
	GraphicSystem*   _graphicSys;
	AssetManager*    _assetMgr;
	ContainerType*   _container;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards