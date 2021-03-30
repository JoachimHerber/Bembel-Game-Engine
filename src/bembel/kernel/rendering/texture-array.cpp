#include "./texture-array.hpp"

#include <bembel/base/logging/logger.hpp>

namespace bembel::kernel {

TextureArray::TextureArray(GLenum format)
: _format{format}
, _handle{0} {
}

TextureArray::~TextureArray() {
  this->cleanup();
}

void TextureArray::init(
  const glm::uvec3& size,
  GLenum min_filter,
  GLenum mag_filter,
  GLenum warp_s,
  GLenum warp_t) {
  glGenTextures(1, &_handle);
  glBindTexture(GL_TEXTURE_2D_ARRAY, _handle);
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));

  glTexImage3D(
    GL_TEXTURE_2D_ARRAY,
    0,
    _format,
    size.x,
    size.y,
    size.z,
    0,
    GL_RGBA,
    GL_FLOAT,
    0);
  _size = size;
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
bool TextureArray::init(
  const std::vector<base::Image>& images,
  GLenum min_filter,
  GLenum mag_filter,
  GLenum warp_s,
  GLenum warp_t) {
  if(images.empty()) { return false; }

  const glm::ivec3 size{
    images[0].getWidth(), images[1].getHeight(), images.size()};

  for(auto& it : images) {
    if(it.getWidth() != size.x || it.getHeight() != size.y) {
      BEMBEL_LOG_WARNING() << "all images must be the same size";
      return false;
    }
  }

  glGenTextures(1, &_handle);
  glBindTexture(GL_TEXTURE_2D_ARRAY, _handle);
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));
  glTexImage3D(
    GL_TEXTURE_2D_ARRAY,
    0,
    _format,
    size.x,
    size.y,
    size.z,
    0,
    GL_RGBA,
    GL_FLOAT,
    0);
  _size = size;

  for(size_t n = 0; n < images.size(); ++n) {
    GLenum format;
    switch(images[n].getChannels()) {
      case 1: format = GL_RED; break;
      case 2: format = GL_RG; break;
      case 3: format = GL_RGB; break;
      case 4: format = GL_RGBA; break;
    }
    glTexSubImage3D(
      GL_TEXTURE_2D_ARRAY,
      0,
      0,
      0,
      n,
      size.z,
      size.y,
      1,
      format,
      GL_UNSIGNED_BYTE,
      images[n].getData());
  }
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

bool TextureArray::init(
  const std::vector<base::Image>& images,
  const std::unordered_map<std::string, int>& textures_names,
  GLenum min_filter,
  GLenum mag_filter,
  GLenum warp_s,
  GLenum warp_t) {
  if(!this->init(images, min_filter, mag_filter, warp_s, warp_t)) {
    return false;
  }
  _textures_names = textures_names;
  return true;
}

void TextureArray::cleanup() {
  if(_handle != 0) glDeleteTextures(1, &_handle);
  _handle = 0;
}
void TextureArray::use() const {
  glBindTexture(GL_TEXTURE_2D_ARRAY, _handle);
}

int TextureArray::getTextureIndex(std::string& name) {
  auto it = _textures_names.find(name);
  if(it != _textures_names.end()) return it->second;

  return -1;
}

GLuint TextureArray::getTextureHandle() const {
  return _handle;
}

GLenum TextureArray::getTextureFormat() const {
  return _format;
}

GLenum TextureArray::getTextureTarget() const {
  return GL_TEXTURE_2D_ARRAY;
}

const std::string& TextureArray::getTypeName() {
  const static std::string type_name = "TextureArray";
  return type_name;
}

std::unique_ptr<TextureArray> TextureArray::loadAsset(
  AssetManager* asset_manager, const std::string& file_name) {
  xml::Document doc;
  if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
    BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                       << doc.ErrorName() << std::endl;
    return nullptr;
  }

  const xml::Element* root = doc.FirstChildElement(getTypeName().c_str());
  if(!root) {
    BEMBEL_LOG_ERROR() << "File '" << file_name
                       << "' has no root element 'TextureArray'" << std::endl;
    return nullptr;
  }
  return createAsset(asset_manager, root);
}

std::unique_ptr<TextureArray> TextureArray::createAsset(
  AssetManager* asset_manager, const xml::Element* properties) {
  std::vector<base::Image> images;
  std::unordered_map<std::string, int> texture_names;
  for(auto it : base::xml::IterateChildElements(properties, "Entry")) {
    std::string file, name, path;
    if(!xml::getAttribute(it, "file", file)) { return nullptr; }
    if(xml::getAttribute(it, "name", name)) {
      texture_names[name] = images.size();
    }
    auto& locator = asset_manager->getAssetLocator();
    if(!locator.findAssetLocation("Image", file, &path)) { return nullptr; }
    images.push_back({});
    if(!images.back().load(path)) { return nullptr; }
  }
  if(images.empty()) { return nullptr; }

  auto asset = std::make_unique<TextureArray>(GL_RGBA8);
  if(!asset->init(images, texture_names)) { return nullptr; }

  return asset;
}

void TextureArray::deleteAsset(
  AssetManager* asset_manager, std::unique_ptr<TextureArray> texture) {
}

} // namespace bembel::kernel
