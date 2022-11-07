module;
#include <glbinding/gl/gl.h>

#include "bembel/pch.h"
export module bembel.kernel.rendering:Shader;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;
using namespace gl;

export class Shader final {
  public:
    enum class Type {
        VERTEX   = gl::GL_VERTEX_SHADER,
        FRAGMENT = gl::GL_FRAGMENT_SHADER,
        GEOMETRY = gl::GL_GEOMETRY_SHADER
    };

    Shader(Type type, uint handle);
    virtual ~Shader();

    bool init(std::string_view source);

    uint getHandle() const;

    static constexpr std::string_view ASSET_TYPE_NAME = "Shader";

    static std::unique_ptr<Shader> loadAsset(AssetManager& asset_mgr, std::filesystem::path path);
    static std::unique_ptr<Shader> createAsset(
        AssetManager& asset_mgr, xml::Element const* properties
    );

    static void deleteAsset(AssetManager&, std::unique_ptr<Shader>) {}

    static std::unique_ptr<Shader> createShader(Type type, std::string_view source);

    using DefaultLoaderType = SerialAssetLoader<Shader>;

  private:
    Type m_type;
    uint m_handle;
};

export class ShaderProgram final {
  public:
    ShaderProgram();
    ShaderProgram(ShaderProgram const&)            = delete;
    ShaderProgram& operator&(ShaderProgram const&) = delete;
    ~ShaderProgram();

    bool attachShader(AssetManager& asset_mgr, AssetHandle handle);

    void bindAttribLocation(std::string_view name, unsigned int index);
    void bindFragDataLocation(std::string_view name, unsigned int index);

    int  getUniformLocation(std::string_view name) const;
    uint getUniformBlockIndex(std::string_view name) const;
    int  getUniformBlockDataSize(uint block_index) const;
    int  getUniformBlockActiveUniforms(uint block_index) const;
    void getUniformBlockActiveUniformIndices(uint block_index, std::vector<int>* indices) const;

    int  getActiveUniformOffset(uint uniform_index) const;
    void getActiveUniform(uint uniform_index, int* size, GLenum* type, std::string* name) const;

    bool setUniform(std::string_view name, float value);
    bool setUniform(std::string_view name, int value);
    bool setUniform(std::string_view name, glm::mat4 const& value);

    bool link();

    bool use();

    static constexpr std::string_view ASSET_TYPE_NAME = "ShaderProgram";

    static std::unique_ptr<ShaderProgram> loadAsset(
        AssetManager& asset_mgr, std::filesystem::path path
    );
    static std::unique_ptr<ShaderProgram> createAsset(
        AssetManager& asset_mgr, xml::Element const* properties
    );

    static void deleteAsset(AssetManager&, std::unique_ptr<ShaderProgram>);

    using DefaultLoaderType = SerialAssetLoader<ShaderProgram>;

  private:
    uint                     m_program_handle;
    std::vector<AssetHandle> m_shader_handles;

    bool m_ready_to_use = false;

    mutable Dictionary<int> m_uniorm_locations;
};

} // namespace bembel::kernel
