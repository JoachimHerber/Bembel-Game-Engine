/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "material.h"
#include "../graphic-system.h"

#include <bembel-base/conversion.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Material::Material(unsigned renderer, GLint size)
	: renderer_(renderer)
{
	glGenBuffers(1, &uniform_buffer_object_);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer_object_);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

Material::~Material()
{
	glDeleteBuffers(1, &uniform_buffer_object_);
}

unsigned Material::GetRenderer() const
{
	return renderer_;
}

GLuint Material::GetUniformBufferObject() const
{
	return uniform_buffer_object_;
}

const std::vector<AssetHandle>& Material::GetTextures() const
{
	return textures_;
}

void Material::SetTextures(const std::vector<AssetHandle>& textures)
{
	textures_ = textures;
}

const std::string& Material::GetTypeName()
{
	const static std::string type_name = "Material";
	return type_name;
}

MaterialLoader::MaterialLoader(
	AssetManager* asset_manager,
	ContainerType* container,
	GraphicSystem* graphic_system)
	: graphic_system_(graphic_system)
	, asset_manager_(asset_manager)
	, container_(container)
{}

MaterialLoader::~MaterialLoader()
{}

AssetHandle MaterialLoader::RequestAsset(const std::string& fileName)
{
	AssetHandle handle = container_->GetAssetHandle(fileName);

	if( !container_->IsHandelValid(handle) )
	{
		// we have to load the asset
		std::unique_ptr<Material> asset = nullptr;
		//Material::LoadAsset( _assetMgr, fileName );
		if( !asset )
			return AssetHandle();

		handle = container_->AddAsset(std::move(asset));
		container_->IncrementAssetRefCount(handle);
		container_->RegisterAssetAlias(handle, fileName);
	}

	container_->IncrementAssetRefCount(handle);
	return handle;
}

AssetHandle MaterialLoader::RequestAsset(const xml::Element* properties)
{
	std::string name = "";
	if( !xml::GetAttribute(properties, "name", name) )
		return AssetHandle();

	AssetHandle handle = container_->GetAssetHandle(name);
	if( !container_->IsHandelValid(handle) )
	{
		std::string renderer_name;
		xml::GetAttribute(properties, "renderer", renderer_name);
		auto renderer = graphic_system_->GetRenderer(renderer_name);
		if( !renderer )
			return AssetHandle();

		// we have to load the asset
		std::unique_ptr<Material> asset = 
			renderer->CreateMaterial(asset_manager_, properties);
		if( !asset )
			return AssetHandle();

		handle = container_->AddAsset(std::move(asset));
		container_->RegisterAssetAlias(handle, name);
	}

	container_->IncrementAssetRefCount(handle);
	return handle;
}

bool MaterialLoader::ReleaseAsset(AssetHandle asset_handel)
{
	if( container_->IsHandelValid(asset_handel) )
	{
		container_->DecrementAssetRefCount(asset_handel);
		if( container_->GetAssetRefCount(asset_handel) == 0 )
		{
			auto mat = container_->RemoveAsset(asset_handel);
			for( auto& it : mat->GetTextures() )
				asset_manager_->ReleaseAsset(it);

			return true;
		}
	}
	return false;
}

void MaterialLoader::Update()
{}

void MaterialLoader::ResetProgess()
{}

float MaterialLoader::GetProgress()
{
	return 1.0f;
}

bool MaterialLoader::LoadingFinished()
{
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
