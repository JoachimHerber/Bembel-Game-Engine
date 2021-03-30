#ifndef BEMBEL_KERNEL_RENDERING_SHADER_HPP
#define BEMBEL_KERNEL_RENDERING_SHADER_HPP

#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/kernel/open-gl.hpp>

namespace bembel::kernel {

class BEMBEL_API Shader {
  public:
    using DefaultLoaderType = SerialAssetLoader<Shader>;

  public:
    Shader(GLuint handle);
    virtual ~Shader();

    bool init(const std::string& source);

    GLuint getHandle() const;

    const static std::string& getTypeName();

    static std::unique_ptr<Shader> loadAsset(AssetManager& asset_mgr, const std::string& path);
    static std::unique_ptr<Shader> createAsset(
        AssetManager& asset_mgr, const xml::Element* properties);

    static void deleteAsset(AssetManager&, std::unique_ptr<Shader>) {}

    static std::unique_ptr<Shader> createShader(GLenum type, const std::string& source);

  private:
    GLuint handle;
};

class BEMBEL_API ShaderProgram final {
  public:
    using DefaultLoaderType = SerialAssetLoader<ShaderProgram>;

  public:
    ShaderProgram();
    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator&(const ShaderProgram&) = delete;
    ~ShaderProgram();

    bool attachShader(AssetManager& asset_mgr, AssetHandle handle);

    void bindAttribLocation(const std::string& name, unsigned int index);
    void bindFragDataLocation(const std::string& name, unsigned int index);

    GLint  getUniformLocation(const std::string& name) const;
    GLuint getUniformBlockIndex(const std::string& name) const;
    GLint  getUniformBlockDataSize(GLuint block_index) const;
    GLint  getUniformBlockActiveUniforms(GLuint block_index) const;
    void getUniformBlockActiveUniformIndices(GLuint block_index, std::vector<GLint>* indices) const;

    GLint getActiveUniformOffset(GLuint uniform_index) const;
    void getActiveUniform(GLuint uniform_index, GLint* size, GLenum* type, std::string* name) const;

    bool link();

    bool use();

    const static std::string& getTypeName();

    static std::unique_ptr<ShaderProgram> loadAsset(
        AssetManager& asset_mgr, const std::string& path);
    static std::unique_ptr<ShaderProgram> createAsset(
        AssetManager& asset_mgr, const xml::Element* properties);

    static void deleteAsset(AssetManager&, std::unique_ptr<ShaderProgram>);

  private:
    GLuint                   program_handle;
    std::vector<AssetHandle> shader_handles;

    bool ready_to_use = false;

    mutable std::unordered_map<std::string, GLint> uniorm_locations;
};

} // namespace bembel::kernel
#endif // include guards
