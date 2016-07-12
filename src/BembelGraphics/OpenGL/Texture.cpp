/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Texture.h"

#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Texture::Texture(GLenum target, GLenum format )
	: _target(target)
	, _format(format)
	, _handle(0)
{
}

Texture::~Texture()
{
	Cleanup();
}

void Texture::Init(
	GLenum minFilter,
	GLenum magFilter,
	GLenum warpS,
	GLenum warpT)
{
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warpS));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warpT));
	glBindTexture(_target, 0);
}
void Texture::Init(
	const glm::uvec2& size,
	GLenum minFilter,
	GLenum magFilter,
	GLenum warpS,
	GLenum warpT)
{
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warpS));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warpT));
	
	if (_format == GL_DEPTH_COMPONENT ||
		_format == GL_DEPTH_COMPONENT16||
		_format == GL_DEPTH_COMPONENT24||
		_format == GL_DEPTH_COMPONENT32)
	{
		glTexImage2D(
			_target, 0, static_cast<GLint>(_format),
			size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	else
	{
		glTexImage2D(
			_target, 0, static_cast<GLint>(_format),
			size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	glBindTexture(_target, 0);
}
void Texture::Init(
	const Image& image,
	GLenum minFilter, 
	GLenum magFilter,
	GLenum warpS,
	GLenum warpT)
{
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warpS));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warpT));
	glTexImage2D(
		_target, 0, static_cast<GLint>(_format),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData()
		);
	glBindTexture(_target, 0);
}

void Texture::Cleanup()
{
	if(_handle != 0)
		glDeleteTextures(1, &_handle);
	_handle = 0;
}
void Texture::Bind() const
{
	glBindTexture(_target, _handle);
}
void Texture::Release() const
{
	glBindTexture(_target, 0);
}

bool Texture::SetSize(const glm::ivec2& size)
{
	if (_handle == 0)
		return false;

	glBindTexture(_target, _handle);
	glTexImage2D(
		_target, 0, static_cast<GLint>(_format),
		size.x, size.y, 0,
		GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(_target, 0);
	return true;
}

bool Texture::SetData(const Image& image, GLint mipMapLevel)
{
	if (_handle == 0)
		return false;

	glBindTexture(_target, _handle);
	glTexImage2D(
		_target, mipMapLevel, static_cast<GLint>(_format),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData());
	glBindTexture(_target, 0);
	return true;
}

GLuint Texture::GetTextureHandle() const
{
	return _handle;
}

GLenum Texture::GetTextureFormat() const
{
	return _format;
}

GLenum Texture::GetTextureTarget() const
{
	return _target;
}

const std::string& Texture::GetTypeName()
{
	const static std::string typeName = "Texture";
	return typeName;
}

TextureLoader::TextureLoader(TextureContainerPtr container)
	: _container(container)
{}

TextureLoader::~TextureLoader()
{
	//delete all loaded assets
	for (auto it: _loadedTextures)
	{
		Texture* texture = _container->RemoveAsset(it.second, true);
		if(texture)
			delete texture;
	}
}

void TextureLoader::CreateDummyTexture()
{
	if (_container->IsHandelValid(_container->GetAssetHandle("dummy")))
		return;

	Image image(2,2,4);
	image.GetData()[0] = 255;
	image.GetData()[1] = 0;
	image.GetData()[2] = 255;
	image.GetData()[3] = 255;

	image.GetData()[4] = 200;
	image.GetData()[5] = 0;
	image.GetData()[6] = 200;
	image.GetData()[7] = 255;

	image.GetData()[8] = 200;
	image.GetData()[9] = 0;
	image.GetData()[10] = 200;
	image.GetData()[11] = 255;

	image.GetData()[12] = 255;
	image.GetData()[13] = 0;
	image.GetData()[14] = 255;
	image.GetData()[15] = 255;

	Texture* texture = new Texture();
	texture->Init(image);

	AssetHandle handle = _container->AddAsset(texture, "dummy");
	_container->SetDummyAsset(handle);
	_loadedTextures.emplace("dummy", handle);
}

bool TextureLoader::LoadeAsset(const AssetDescription& asset)
{
	if (_container->IsHandelValid(_container->GetAssetHandle(asset.GetName())))
		return false; // there already is an asset with the specified name

	Image image;
	if (!image.Load(asset.GetFilePath()))
		return false; // field to load texture image

	Texture* texture = new Texture();
	texture->Init(image);

	AssetHandle handle = _container->AddAsset(texture, asset.GetName());
	_loadedTextures.emplace(asset.GetName(), handle);
}

bool TextureLoader::UnloadeAsset(const std::string& name, bool force)
{
	auto it = _loadedTextures.find(name);
	
	if (it != _loadedTextures.end())
	{
		Texture* texture = _container->RemoveAsset(it->second, force);
		if (texture)
		{
			delete texture;
			_loadedTextures.erase(it);
			return true;
		}
	}

	return false;
}

std::shared_ptr<TextureLoader> TextureLoader::CreateDefaultLoader(
	AssetManager* assetMgr)
{
	auto container = assetMgr->RequestAssetContainer<Texture>();
	auto loader = std::make_shared<TextureLoader>(container);
	loader->CreateDummyTexture();
	return loader;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/