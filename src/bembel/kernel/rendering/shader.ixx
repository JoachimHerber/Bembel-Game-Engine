module;
#include <filesystem>
#include <memory>
#include <string_view>
export module bembel.kernel.rendering:Shader;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

export class Shader final {
  public:
    enum class Type { VERTEX, FRAGMENT, GEOMETRY };

    Shader(Type type, uint handle);
    virtual ~Shader();

    bool init(std::string_view source);

    uint getHandle() const;

    static constexpr std::string_view ASSET_TYPE_NAME = "Shader";

    static std::unique_ptr<Shader> loadAsset(std::filesystem::path path);
    static std::unique_ptr<Shader> createAsset(xml::Element const* properties);

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

    bool attachShader(Asset<Shader> handle);

    void bindAttribLocation(std::string_view name, unsigned int index);
    void bindFragDataLocation(std::string_view name, unsigned int index);

    int  getUniformLocation(std::string_view name) const;
    uint getUniformBlockIndex(std::string_view name) const;
    int  getUniformBlockDataSize(uint block_index) const;
    int  getUniformBlockActiveUniforms(uint block_index) const;
    void getUniformBlockActiveUniformIndices(uint block_index, std::vector<int>* indices) const;

    int  getActiveUniformOffset(uint uniform_index) const;
    void getActiveUniform(uint uniform_index, int* size, uint* type, std::string* name) const;

    bool setUniform(In<std::string_view> name, In<float> value);
    bool setUniform(In<std::string_view> name, In<int> value);
    bool setUniform(In<std::string_view> name, In<mat4> value);

    bool link();

    bool use();

    static constexpr std::string_view ASSET_TYPE_NAME = "ShaderProgram";

    static std::unique_ptr<ShaderProgram> loadAsset(std::filesystem::path path);
    static std::unique_ptr<ShaderProgram> createAsset(xml::Element const* properties);

    using DefaultLoaderType = SerialAssetLoader<ShaderProgram>;

  private:
    uint                       m_program_handle;
    std::vector<Asset<Shader>> m_shader_handles;

    bool m_ready_to_use = false;

    mutable Dictionary<int> m_uniorm_locations;
};

} // namespace bembel::kernel
