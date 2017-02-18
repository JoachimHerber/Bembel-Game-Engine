/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Material.h"

#include "../GraphicSystem.h"

#include <BembelBase/Conversion.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Material::Material(unsigned renderer, GLint size)
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
	ContainerTypePtr container, 
	GraphicSystem* graphicSys)
	: _graphicSys(graphicSys)
	, _assetMgr( assetMgr )
	, _container( container )
{}

MaterialLoader::~MaterialLoader()
{}

bool MaterialLoader::CreateAsset(
	const xml::Element* properties)
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

AssetHandle MaterialLoader::RequestAsset(const std::string& fileName)
{
	AssetHandle handle = _container->GetAssetHandle( fileName );
	if( _container->IsHandelValid( handle ) )
		return handle;// asset already loaded

	return AssetHandle();
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
