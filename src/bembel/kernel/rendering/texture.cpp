module;
#include <glbinding/gl/gl.h>

#include <memory>
#include <source_location>
module bembel.kernel.rendering;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;
using namespace ::gl;

namespace gl {

    inline void bindTexture(Texture::Target target, uint texture) {
        glBindTexture(static_cast<GLenum>(target), texture);
    }
    template <typename T>
    inline void setTexParam(Texture::Target target, GLenum param, T value)
        requires std::is_enum_v<T> || std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        if constexpr(std::is_enum_v<T> || std::is_integral_v<T>) {
            glTexParameteri(static_cast<GLenum>(target), param, static_cast<GLint>(value));
        } else {
            glTexParameterf(static_cast<GLenum>(target), param, static_cast<GLfloat>(value));
        }
    }
    inline void setTexImage2D(
        Texture::Target target,
        int             level,
        Texture::Format internalformat,
        uint            width,
        uint            height,
        Texture::Format format = Texture::Format::RGBA,
        GLenum          type   = GL_FLOAT,
        void const*     data   = nullptr
    ) {
        glTexImage2D(
            static_cast<GLenum>(target),
            level,
            static_cast<GLint>(internalformat),
            width,
            height,
            0,
            static_cast<GLenum>(format),
            type,
            data
        );
    }
    inline void generateMipmap(Texture::Target target) {
        glGenerateMipmap(static_cast<GLenum>(target));
    }

} // namespace gl

Texture::Texture(Target target, Format format) : m_target(target), m_format(format), m_handle(0) {}

Texture::~Texture() {
    cleanup();
}

void Texture::init(
    In<MinFilter> min_filter, In<MagFilter> mag_filter, In<Wrap> warp_s, In<Wrap> warp_t
) {
    glGenTextures(1, &(m_handle));
    gl::bindTexture(m_target, m_handle);
    gl::setTexParam(m_target, GL_TEXTURE_MIN_FILTER, min_filter);
    gl::setTexParam(m_target, GL_TEXTURE_MAG_FILTER, mag_filter);
    gl::setTexParam(m_target, GL_TEXTURE_WRAP_S, warp_s);
    gl::setTexParam(m_target, GL_TEXTURE_WRAP_T, warp_t);
    gl::bindTexture(m_target, 0);
}

void Texture::init(
    In<uvec2>     size,
    In<MinFilter> min_filter,
    In<MagFilter> mag_filter,
    In<Wrap>      warp_s,
    In<Wrap>      warp_t
) {
    glGenTextures(1, &(m_handle));
    gl::bindTexture(m_target, m_handle);
    gl::setTexParam(m_target, GL_TEXTURE_MIN_FILTER, min_filter);
    gl::setTexParam(m_target, GL_TEXTURE_MAG_FILTER, mag_filter);
    gl::setTexParam(m_target, GL_TEXTURE_WRAP_S, warp_s);
    gl::setTexParam(m_target, GL_TEXTURE_WRAP_T, warp_t);

    if(m_format == Format::DEPTH_COMPONENT || m_format == Format::DEPTH_COMPONENT16
       || m_format == Format::DEPTH_COMPONENT24 || m_format == Format::DEPTH_COMPONENT32) {
        gl::setTexImage2D(m_target, 0, m_format, size.x, size.y, Format::DEPTH_COMPONENT);
    } else {
        gl::setTexImage2D(m_target, 0, m_format, size.x, size.y);
    }
    gl::generateMipmap(m_target);
    gl::bindTexture(m_target, 0);
}
void Texture::init(
    In<Image>     image,
    In<MinFilter> min_filter,
    In<MagFilter> mag_filter,
    In<Wrap>      warp_s,
    In<Wrap>      warp_t
) {
    glGenTextures(1, &(m_handle));
    gl::bindTexture(m_target, m_handle);
    gl::setTexParam(m_target, GL_TEXTURE_MIN_FILTER, min_filter);
    gl::setTexParam(m_target, GL_TEXTURE_MAG_FILTER, mag_filter);
    gl::setTexParam(m_target, GL_TEXTURE_WRAP_S, warp_s);
    gl::setTexParam(m_target, GL_TEXTURE_WRAP_T, warp_t);
    gl::setTexImage2D(
        m_target,
        0,
        m_format,
        image.getWidth(),
        image.getHeight(),
        image.getChannels() == 4 ? Format::RGBA : Format::RGB,
        GL_UNSIGNED_BYTE,
        image.getData()
    );
    gl::generateMipmap(m_target);
    gl::bindTexture(m_target, 0);
}

void Texture::cleanup() {
    if(m_handle != 0) glDeleteTextures(1, &(m_handle));
    m_handle = 0;
}
void Texture::bind() const {
    gl::bindTexture(m_target, m_handle);
}
void Texture::release() const {
    gl::bindTexture(m_target, 0);
}

bool Texture::setSize(In<ivec2> size) {
    if(m_handle == 0) return false;

    gl::bindTexture(m_target, m_handle);
    gl::setTexImage2D(m_target, 0, m_format, size.x, size.y);
    gl::bindTexture(m_target, 0);
    return true;
}

bool Texture::setData(In<Image> image, In<int> mipMapLevel) {
    if(m_handle == 0) return false;

    gl::bindTexture(m_target, m_handle);
    gl::setTexImage2D(
        m_target,
        mipMapLevel,
        m_format,
        image.getWidth(),
        image.getHeight(),
        image.getChannels() == 4 ? Format::RGBA : Format::RGB,
        GL_UNSIGNED_BYTE,
        image.getData()
    );
    gl::bindTexture(m_target, 0);
    return true;
}

std::unique_ptr<Texture> Texture::loadAsset(
    kernel::AssetManager& asset_mgr, std::filesystem::path file
) {
    base::Image image;

    if(image.load(file)) {
        auto texture = std::make_unique<Texture>();
        texture->init(image);
        return texture;
    }
    return nullptr;
}

std::unique_ptr<Texture> Texture::createAsset(
    kernel::AssetManager& asset_mgr, xml::Element const* properties
) {
    std::string file;
    if(xml::getAttribute(properties, "file", file)) { return Texture::loadAsset(asset_mgr, file); }
    return nullptr;
}

void Texture::deleteAsset(kernel::AssetManager& asset_mgr, std::unique_ptr<Texture> texture) {}

std::optional<Texture::Format> Texture::stringToTextureFormat(std::string_view format) {
    static Dictionary<Format> const mapping{
        {"GL_DEPTH_COMPONENT32", Format::DEPTH_COMPONENT32},
        {"GL_DEPTH_COMPONENT24", Format::DEPTH_COMPONENT24},
        {"GL_DEPTH_COMPONENT16", Format::DEPTH_COMPONENT16},
        {"GL_DEPTH_COMPONENT", Format::DEPTH_COMPONENT},
        {"GL_RGBA", Format::RGBA},
        {"GL_RGBA8", Format::RGBA8},
        {"GL_RGBA16", Format::RGBA16},
        {"GL_RGBA16F", Format::RGBA16F},
        {"GL_RGBA32F", Format::RGBA32F},
        {"GL_RGB", Format::RGB},
        {"GL_RGB8", Format::RGB8},
        {"GL_RGB16", Format::RGB16},
        {"GL_RGB16F", Format::RGB16F},
        {"GL_RGB32F", Format::RGB32F},

        {"GL_SRGB8", Format::SRGB8},
        {"GL_SRGB8_ALPHA8", Format::SRGB8_ALPHA8},
    };
    auto it = mapping.find(format);
    if(it != mapping.end()) return it->second;

    return {};
}
} // namespace bembel::kernel
