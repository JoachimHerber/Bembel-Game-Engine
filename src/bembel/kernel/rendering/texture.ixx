module;
#include <glbinding/gl/gl.h>

#include <memory>
#include <optional>
#include <string_view>
export module bembel.kernel.rendering:Texture;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

export class Texture final {
  public:
    using TexturePtr = std::unique_ptr<Texture>;

    enum class Target : uint {
        TEXTURE_1D       = gl::GL_TEXTURE_1D,
        TEXTURE_2D       = gl::GL_TEXTURE_2D,
        TEXTURE_3D       = gl::GL_TEXTURE_3D,
        TEXTURE_2D_ARRAY = gl::GL_TEXTURE_2D_ARRAY,
        CUBE_MAP         = gl::GL_TEXTURE_CUBE_MAP
    };
    enum class Format : uint {
        DEPTH_COMPONENT   = gl::GL_DEPTH_COMPONENT,
        DEPTH_COMPONENT16 = gl::GL_DEPTH_COMPONENT16,
        DEPTH_COMPONENT24 = gl::GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT32 = gl::GL_DEPTH_COMPONENT32,
        R8                = gl::GL_R8,
        RGB               = gl::GL_RGB,
        RGB8              = gl::GL_RGB8,
        RGB16             = gl::GL_RGB16,
        RGB16F            = gl::GL_RGB16F,
        RGB32F            = gl::GL_RGB32F,
        RGBA              = gl::GL_RGBA,
        RGBA8             = gl::GL_RGBA8,
        RGBA16            = gl::GL_RGBA16,
        RGBA16F           = gl::GL_RGBA16F,
        RGBA32F           = gl::GL_RGBA32F,
        SRGB8             = gl::GL_SRGB8,
        SRGB8_ALPHA8      = gl::GL_SRGB8_ALPHA8,
    };
    enum class MinFilter : uint {
        NEAREST         = gl::GL_NEAREST,
        NEAREST_NEAREST = gl::GL_NEAREST_MIPMAP_NEAREST,
        NEAREST_LINEAR  = gl::GL_NEAREST_MIPMAP_LINEAR,
        LINEAR          = gl::GL_LINEAR,
        LINEAR_NEAREST  = gl::GL_LINEAR_MIPMAP_NEAREST,
        LINEAR_LINEAR   = gl::GL_LINEAR_MIPMAP_LINEAR
    };
    enum class MagFilter : uint { NEAREST = gl::GL_NEAREST, LINEAR = gl::GL_LINEAR };
    enum class Wrap : uint {
        CLAMP_TO_EDGE   = gl::GL_CLAMP_TO_EDGE,
        MIRRORED_REPEAT = gl::GL_MIRRORED_REPEAT,
        REPEAT          = gl::GL_REPEAT
    };

    Texture(Target target = Target::TEXTURE_2D, Format format = Format::RGBA8);
    ~Texture();

    void init(
        In<MinFilter> min_filter = MinFilter::LINEAR_LINEAR,
        In<MagFilter> mag_filter = MagFilter::LINEAR,
        In<Wrap>      warp_s     = Wrap::CLAMP_TO_EDGE,
        In<Wrap>      warp_t     = Wrap::CLAMP_TO_EDGE
    );
    void init(
        In<uvec2>     size,
        In<MinFilter> min_filter = MinFilter::LINEAR_LINEAR,
        In<MagFilter> mag_filter = MagFilter::LINEAR,
        In<Wrap>      warp_s     = Wrap::CLAMP_TO_EDGE,
        In<Wrap>      warp_t     = Wrap::CLAMP_TO_EDGE
    );
    void init(
        In<uvec3>     size,
        In<MinFilter> min_filter = MinFilter::LINEAR_LINEAR,
        In<MagFilter> mag_filter = MagFilter::LINEAR,
        In<Wrap>      warp_s     = Wrap::CLAMP_TO_EDGE,
        In<Wrap>      warp_t     = Wrap::CLAMP_TO_EDGE
    );
    void init(
        In<Image>     _data,
        In<MinFilter> min_filter = MinFilter::LINEAR_LINEAR,
        In<MagFilter> mag_filter = MagFilter::LINEAR,
        In<Wrap>      warp_s     = Wrap::CLAMP_TO_EDGE,
        In<Wrap>      warp_t     = Wrap::CLAMP_TO_EDGE
    );
    void cleanup();

    void bind() const;
    void release() const;

    bool setSize(In<ivec2> size);
    bool setData(In<Image>, In<int> mip_map_level = 0);

    uint   getTextureHandle() const { return m_handle; }
    Format getTextureFormat() const { return m_format; }
    Target getTextureTarget() const { return m_target; }

    static constexpr std::string_view ASSET_TYPE_NAME = "Texture";

    static TexturePtr loadAsset(std::filesystem::path file);
    static TexturePtr createAsset(xml::Element const* properties);

    using DefaultLoaderType = SerialAssetLoader<Texture>;

    static std::optional<Format> stringToTextureFormat(std::string_view);

  private:
    Target m_target;
    Format m_format;
    uint   m_handle;
};

} // namespace bembel::kernel
