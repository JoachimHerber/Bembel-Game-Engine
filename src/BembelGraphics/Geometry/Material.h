#ifndef BEMBEL_MATERIAL_H
#define BEMBEL_MATERIAL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Assets/AssetLoader.h>

#include <glm/glm.hpp>

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

using MaterialContainer    = AssetContainer<Material>;
using MaterialContainerPtr = std::shared_ptr<MaterialContainer>;

class BEMBEL_API MaterialLoader final : public AssetLoaderBase
{
public:
	using ContainerType = AssetContainer<Material>;
	using ContainerTypePtr = std::shared_ptr<ContainerType>;

	MaterialLoader( AssetManager* , ContainerTypePtr, GraphicSystem* );
	virtual ~MaterialLoader();

	virtual bool        CreateAsset(  const xml::Element* propertiey ) override;
	virtual AssetHandle RequestAsset( const std::string&  fileName   ) override;

	virtual void Update() override;

	virtual void  ResetProgess()    override;
	virtual float GetProgress()     override;
	virtual bool  LoadingFinished() override;

private:
	GraphicSystem*   _graphicSys;
	AssetManager*    _assetMgr;
	ContainerTypePtr _container;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
