#ifndef BEMBEL_MATERIAL_H
#define BEMBEL_MATERIAL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "../OpenGL/Texture.h"

#include <BembelKernel/Assets/AssetLoader.h>
#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Assets/AssetContainer.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Material final
{
public:
	using Color = glm::vec3;

public:
	Material();
	~Material();

	const Color& GetEmission() const;
	void SetEmission(const Color& value);

	const Color& GetAlbedo() const;
	void SetAlbedo(const Color& value);

	const Color& GetReflectivity() const;
	void SetReflectivity(const Color& value);

	float GetRoughness() const;
	void SetRoughness(float value);

	AssetHandle GetEmissionTexture() const;
	void SetEmissionTexture(AssetHandle handle);

	AssetHandle GetAlbedoTexture() const;
	void SetAlbedoTexture(AssetHandle handle);

	AssetHandle GetReflectivityTexture() const;
	void SetReflectivityTexture(AssetHandle handle);

	AssetHandle GetNormalTexture() const;
	void SetNormalTexture(AssetHandle handle);

	bool GetNormalMapHasRoughness() const;
	void SetNormalMapHasRoughness(bool value);

	const static std::string& GetTypeName();

private:
	Color _emission     = Color(0.0f, 0.0f, 0.0f);
	Color _albedo       = Color(1.0f, 0.0f, 1.0f);
	Color _reflectivity = Color(0.1f, 0.1f, 0.1f);
	float _roughness    = 1.0f;

	AssetHandle _emissionTexture     = AssetHandle{~0U,~0U};
	AssetHandle _albedoTexture       = AssetHandle{~0U,~0U};
	AssetHandle _reflectivityTexture = AssetHandle{~0U,~0U};
	AssetHandle _normalTexture       = AssetHandle{~0U,~0U};
	bool _normalMapHasRoughness      = false;

	friend class MaterialLoader;
}; 

using MaterialContainer    = AssetContainer<Material>;
using MaterialContainerPtr = std::shared_ptr<MaterialContainer>;


class BEMBEL_API MaterialLoader : public AssetLoaderBase
{
public:
	MaterialLoader(MaterialContainerPtr, TextureContainerPtr);
	~MaterialLoader();

	void CreateDummyMaterial();

	virtual bool LoadeAsset(const AssetDescription& asset) override;
	virtual bool UnloadeAsset(const std::string& name, bool force = false) override;

	static std::shared_ptr<MaterialLoader> CreateDefaultLoader(AssetManager*);

private:
	MaterialContainerPtr _materials;
	TextureContainerPtr  _textures;

	std::map<std::string, AssetHandle> _loadedMaterials;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards