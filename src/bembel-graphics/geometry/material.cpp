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

Material::Material( unsigned renderer, GLint size )
	: _renderer( renderer )
{
	glGenBuffers( 1, &_uniformBufferObject );
	glBindBufferBase( GL_UNIFORM_BUFFER, 0, _uniformBufferObject );
	glBufferData( GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW );
}

Material::~Material()
{
	glDeleteBuffers( 1, &_uniformBufferObject );
}

unsigned Material::GetRenderer() const
{
	return _renderer;
}

GLuint Material::GetUniformBufferObject() const
{
	return _uniformBufferObject;
}

const std::string& Material::GetTypeName()
{
	const static std::string typeName = "Material";
	return typeName;
}

MaterialLoader::MaterialLoader(
	AssetManager* assetMgr,
	ContainerType* container,
	GraphicSystem* graphicSys )
	: _graphicSys( graphicSys )
	, _assetMgr( assetMgr )
	, _container( container )
{}

MaterialLoader::~MaterialLoader()
{}

AssetHandle MaterialLoader::RequestAsset(const std::string& fileName )
{
	AssetHandle handle = _container->GetAssetHandle( fileName );

	if( !_container->IsHandelValid( handle ) )
	{
		// we have to load the asset
		std::unique_ptr<Material> asset = nullptr;
			//Material::LoadAsset( _assetMgr, fileName );
		if( !asset )
			return AssetHandle();

		handle = _container->AddAsset( std::move( asset ) );
		_container->IncrementAssetRefCount( handle );
		_container->RegisterAssetAlias( handle, fileName );
	}

	_container->IncrementAssetRefCount( handle );
	return handle;
}

AssetHandle MaterialLoader::RequestAsset(const xml::Element* properties )
{
	std::string name = "";
	if( !xml::GetAttribute( properties, "name", name ) )
		return AssetHandle();

	AssetHandle handle = _container->GetAssetHandle( name );
	if( !_container->IsHandelValid( handle ) )
	{
		// we have to load the asset
		std::unique_ptr<Material> asset = nullptr;
			//Material::LoadAsset( _assetMgr, properties );
		if( !asset )
			return AssetHandle();

		handle = _container->AddAsset( std::move( asset ) );
		_container->IncrementAssetRefCount( handle );
		_container->RegisterAssetAlias( handle, name );
	}

	_container->IncrementAssetRefCount( handle );
	return handle;
}

bool MaterialLoader::ReleaseAsset( AssetHandle assetHandel )
{
	if( _container->IsHandelValid( assetHandel ) )
	{
		_container->DecrementAssetRefCount( assetHandel );
		if( _container->GetAssetRefCount( assetHandel ) == 0 )
		{
			_container->RemoveAsset( assetHandel );
			return true;
		}
	}
	return false;
}

/*
AssetHandle MaterialLoader::RequestAsset(
	const xml::Element* properties )
{
	std::string name = "", rendererName = "";
	if( !xml::GetAttribute( properties, "name", name ) ||
		_container->HasAsset( name ) )
	{
		// all assets must have a unique name
		return false;
	}

	xml::GetAttribute( properties, "renderer", rendererName );

	auto renderer = _graphicSys->GetRenderer( rendererName );
	if( !renderer )
		return false;

	auto mat = renderer->CreateMaterial( properties );

	AssetHandle handle = _container->AddAsset( std::move( mat ) );
	if( !_container->IsHandelValid( handle ) )
		return false;

	_container->RegisterAssetAlias( handle, name );
	return true;
}
*/

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
