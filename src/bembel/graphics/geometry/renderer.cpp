#include "./renderer.hpp"

#include <bembel/kernel/rendering/texture.hpp>

#include "./mesh.hpp"
#include "./model.hpp"

namespace bembel::graphics {

DefaultGeometryRenderer::DefaultGeometryRenderer(kernel::AssetManager& asset_mgr, unsigned id)
: GeometryRendererBase(asset_mgr, id) {
}

DefaultGeometryRenderer::~DefaultGeometryRenderer() {
}

void DefaultGeometryRenderer::addRequiredTexture(
    const std::string& texture_name, const std::string& uniform_sampler_name) {
    required_textures.push_back({texture_name, uniform_sampler_name});
}

bool DefaultGeometryRenderer::setShader(const kernel::Asset<kernel::ShaderProgram>& shader) {
    if(this->updateUniformLocations(shader.get())) {
        this->shader = shader;
        return true;
    }
}

bool DefaultGeometryRenderer::updateUniformLocations(kernel::ShaderProgram* shader) {
    if(shader == nullptr) return false;

    this->material_uniform_block_index = shader->getUniformBlockIndex("Material");
    this->material_uniform_buffer_size =
        shader->getUniformBlockDataSize(this->material_uniform_block_index);

    std::vector<GLint> active_uniform_indices;
    shader->getUniformBlockActiveUniformIndices(
        this->material_uniform_block_index, &active_uniform_indices);

    for(GLint uniform_index : active_uniform_indices) {
        MaterialParam param;
        param.offset = shader->getActiveUniformOffset(uniform_index);
        std::string name;
        shader->getActiveUniform(uniform_index, &param.size, &param.type, &name);

        this->material_params[name]       = param;
        std::string param_name            = name.substr(name.find_last_of(".") + 1);
        this->material_params[param_name] = param;
    }

    shader->use();
    GLuint sampler_location = 0;
    for(const auto& it : this->required_textures) {
        glUniform1i(shader->getUniformLocation(it.uniform_sampler_name), sampler_location);
        ++sampler_location;
    }
    return true;
}

void DefaultGeometryRenderer::render(
    const glm::mat4& proj, const glm::mat4& view, const std::vector<GeometryRenderData>& data) {
    auto shader_pointer = this->shader.get();

    if(!shader_pointer) return;

    shader_pointer->use();

    glUniformMatrix4fv(
        shader_pointer->getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, &proj[0][0]);

    GeometryMesh* currentMesh     = nullptr;
    Material*     currentMaterial = nullptr;
    for(const auto& it : data) {
        if(!it.mesh || !it.material) continue;

        if(it.mesh != currentMesh) {
            currentMesh = it.mesh;
            glBindVertexArray(currentMesh->getVAO());
        }
        if(it.material != currentMaterial) {
            currentMaterial = it.material;
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, currentMaterial->getUniformBufferObject());

            GLenum active_texture = GL_TEXTURE0;
            for(const auto& texture : currentMaterial->getTextures()) {
                if(!texture) {
                    BEMBEL_LOG_WARNING() << "Texture handle is invalid";
                    continue;
                }
                glActiveTexture(active_texture);
                texture->bind();
                active_texture = active_texture + 1;
            }
        }

        glm::mat4 modleView = view * it.transform;

        glUniformMatrix4fv(
            shader_pointer->getUniformLocation("uModleViewMatrix"),
            1,
            GL_FALSE,
            &(modleView[0][0]));
        glUniformMatrix4fv(
            shader_pointer->getUniformLocation("uNormalMatrix"), 1, GL_FALSE, &(modleView[0][0]));

        glLoadIdentity();
        glMultMatrixf(&(it.transform[0][0]));
        glDrawElements(
            GL_TRIANGLES, it.count, GL_UNSIGNED_INT, (void*)(it.first * sizeof(unsigned)));
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
}

namespace {
    template <typename T>
    bool ReadMaterialParam(
        const base::xml::Element* properties, const std::string& param_name, GLbyte* dest) {
        T value;
        if(base::xml::getAttribute(properties, param_name, value)) {
            memcpy(dest, &value, sizeof(value));
            return true;
        }
        return false;
    }
} // namespace

bool DefaultGeometryRenderer::readMaterialParameter(
    const base::xml::Element* properties,
    const std::string&        param_name,
    const MaterialParam&      param,
    GLbyte*                   buffer) {
    switch(param.type) {
        case GL_FLOAT:
            return ReadMaterialParam<float>(properties, param_name, buffer + param.offset);
        case GL_FLOAT_VEC2:
            return ReadMaterialParam<glm::vec2>(properties, param_name, buffer + param.offset);
        case GL_FLOAT_VEC3:
            return ReadMaterialParam<glm::vec3>(properties, param_name, buffer + param.offset);
        case GL_FLOAT_VEC4:
            return ReadMaterialParam<glm::vec4>(properties, param_name, buffer + param.offset);
    }
    return false;
}

std::unique_ptr<Material> DefaultGeometryRenderer::createMaterial(
    const base::xml::Element* properties) {
    auto mat = std::make_unique<Material>(getRendererID(), this->material_uniform_buffer_size);

    std::vector<kernel::Asset<kernel::Texture>> textures;
    for(auto& it : this->required_textures) {
        auto texture_name = properties->FirstChildElement(it.texture_name.c_str());
        if(texture_name == nullptr) {
            BEMBEL_LOG_ERROR() << "Material does not secify a '" << it.texture_name << "' texture.";
            return nullptr;
        }

        kernel::Asset<kernel::Texture> texture;
        if(!texture.request(this->asset_mgr, texture_name)) {
            BEMBEL_LOG_ERROR() << "Can't find reqired '" << it.texture_name
                               << "' texture for material.";
            return nullptr;
        }
        textures.push_back(std::move(texture));
    }
    mat->setTextures(std::move(textures));

    std::vector<GLbyte> data;
    data.resize(this->material_uniform_buffer_size);
    for(auto& it : this->material_params) {
        readMaterialParameter(properties, it.first, it.second, &data[0]);
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mat->getUniformBufferObject());
    glBufferSubData(GL_UNIFORM_BUFFER, 0, this->material_uniform_buffer_size, &data[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return std::move(mat);
}

std::unique_ptr<DefaultGeometryRenderer> DefaultGeometryRenderer::createRenderer(
    const base::xml::Element* properties, kernel::AssetManager& asset_mgr, unsigned id) {
    auto shader_params = properties->FirstChildElement("ShaderProgram");
    if(shader_params == nullptr) {
        BEMBEL_LOG_ERROR() << "Can't create DefaultGeometryRenderer: ShaderProgram "
                              "parameter is missing";
        return nullptr;
    }

    kernel::Asset<kernel::ShaderProgram> shader;
    if(!shader.request(asset_mgr, shader_params)) {
        BEMBEL_LOG_ERROR() << "Could not load ShaderProgram for DefaultGeometryRenderer";
        return nullptr;
    }
    auto renderer = std::make_unique<DefaultGeometryRenderer>(asset_mgr, id);

    for(auto it : base::xml::IterateChildElements(properties, "RequiredTexture")) {
        std::string texture_name, sampler_uniform;
        base::xml::getAttribute(it, "texturen_name", texture_name);
        base::xml::getAttribute(it, "sampler_uniform_name", sampler_uniform);

        renderer->addRequiredTexture(texture_name, sampler_uniform);
    }

    if(!renderer->setShader(shader)) return nullptr;

    return std::move(renderer);
}

} // namespace bembel::graphics
