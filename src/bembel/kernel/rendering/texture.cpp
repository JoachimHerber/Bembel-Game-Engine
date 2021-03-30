#include "./texture.hpp"

#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/assets/asset-locator.hpp>

namespace bembel::kernel {

Texture::Texture(GLenum target, GLenum format)
: target(target)
, format(format)
, handle(0) {
}

Texture::~Texture() {
    cleanup();
}

void Texture::init(GLenum min_filter, GLenum mag_filter, GLenum warp_s, GLenum warp_t) {
    glGenTextures(1, &(this->handle));
    glBindTexture(this->target, this->handle);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));
    glBindTexture(this->target, 0);
}

void Texture::init(
    const glm::uvec2& size, GLenum min_filter, GLenum mag_filter, GLenum warp_s, GLenum warp_t) {
    glGenTextures(1, &(this->handle));
    glBindTexture(this->target, this->handle);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));

    if(this->format == GL_DEPTH_COMPONENT || this->format == GL_DEPTH_COMPONENT16
       || this->format == GL_DEPTH_COMPONENT24 || this->format == GL_DEPTH_COMPONENT32) {
        glTexImage2D(
            this->target,
            0,
            static_cast<GLint>(this->format),
            size.x,
            size.y,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            nullptr);
    } else {
        glTexImage2D(
            this->target,
            0,
            static_cast<GLint>(this->format),
            size.x,
            size.y,
            0,
            GL_RGBA,
            GL_FLOAT,
            nullptr);
    }
    glGenerateMipmap(this->target);
    glBindTexture(this->target, 0);
}
void Texture::init(
    const base::Image& image, GLenum min_filter, GLenum mag_filter, GLenum warp_s, GLenum warp_t) {
    glGenTextures(1, &(this->handle));
    glBindTexture(this->target, this->handle);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));
    glTexImage2D(
        this->target,
        0,
        static_cast<GLint>(this->format),
        image.getWidth(),
        image.getHeight(),
        0,
        image.getChannels() == 4 ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        image.getData());
    glGenerateMipmap(this->target);
    glBindTexture(this->target, 0);
}

void Texture::cleanup() {
    if(this->handle != 0) glDeleteTextures(1, &(this->handle));
    this->handle = 0;
}
void Texture::bind() const {
    glBindTexture(this->target, this->handle);
}
void Texture::release() const {
    glBindTexture(this->target, 0);
}

bool Texture::setSize(const glm::ivec2& size) {
    if(this->handle == 0) return false;

    glBindTexture(this->target, this->handle);
    glTexImage2D(
        this->target,
        0,
        static_cast<GLint>(this->format),
        size.x,
        size.y,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr);
    glBindTexture(this->target, 0);
    return true;
}

bool Texture::setData(const base::Image& image, GLint mipMapLevel) {
    if(this->handle == 0) return false;

    glBindTexture(this->target, this->handle);
    glTexImage2D(
        this->target,
        mipMapLevel,
        static_cast<GLint>(this->format),
        image.getWidth(),
        image.getHeight(),
        0,
        image.getChannels() == 4 ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        image.getData());
    glBindTexture(this->target, 0);
    return true;
}

GLuint Texture::getTextureHandle() const {
    return this->handle;
}

GLenum Texture::getTextureFormat() const {
    return this->format;
}

GLenum Texture::getTextureTarget() const {
    return this->target;
}

const std::string& Texture::getTypeName() {
    const static std::string type_name = "Texture";
    return type_name;
}

std::unique_ptr<Texture> Texture::loadAsset(
    kernel::AssetManager& asset_mgr, const std::string& file_name) {
    base::Image image;

    if(image.load(file_name)) {
        auto texture = std::make_unique<Texture>();
        texture->init(image);
        return texture;
    }
    return nullptr;
}

std::unique_ptr<Texture> Texture::createAsset(
    kernel::AssetManager& asset_mgr, const base::xml::Element* properties) {
    std::string file;
    if(base::xml::getAttribute(properties, "file", file)) {
        return Texture::loadAsset(asset_mgr, file);
    }
    return nullptr;
}

void Texture::deleteAsset(kernel::AssetManager& asset_mgr, std::unique_ptr<Texture> texture) {
}

} // namespace bembel::kernel
