#include "./shader.hpp"

#include <bembel/base/logging/logger.hpp>

namespace bembel::kernel {

Shader::Shader(GLuint handle)
: handle(handle) {
}

Shader::~Shader() {
    glDeleteShader(this->handle);
}

bool Shader::init(const std::string& source) {
    const char* c_str  = source.c_str();
    const int   length = static_cast<int>(source.length());
    glShaderSource(this->handle, 1, &c_str, &length);
    glCompileShader(this->handle);

    int status;
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &status);
    if(status != int(GL_TRUE)) {
        char    error_message[4096];
        GLsizei size;
        glGetShaderInfoLog(this->handle, 4096, &size, error_message);

        BEMBEL_LOG_ERROR() << "Can't compile Shader" << std::endl << error_message << std::endl;

        return false;
    }
    return true;
}

GLuint Shader::getHandle() const {
    return this->handle;
}

const std::string& Shader::getTypeName() {
    const static std::string type_name = "Shader";
    return type_name;
}

std::unique_ptr<Shader> Shader::loadAsset(AssetManager& asset_mgr, const std::string& file_name) {
    xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return nullptr;
    }

    const xml::Element* root = doc.FirstChildElement(getTypeName().c_str());
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'Shader'"
                           << std::endl;
        return nullptr;
    }
    return createAsset(asset_mgr, root);
}

std::unique_ptr<Shader> Shader::createAsset(
    AssetManager& asset_mgr, const xml::Element* properties) {
    const static std::map<std::string, GLenum> shader_type_map{
        {"GL_VERTEX_SHADER", GL_VERTEX_SHADER},
        {"GL_FRAGMENT_SHADER", GL_FRAGMENT_SHADER},
        {"GL_GEOMETRY_SHADER", GL_GEOMETRY_SHADER},
    };

    if(properties->GetText() == nullptr) return nullptr;

    std::string type;
    if(!xml::getAttribute(properties, "type", type)) return nullptr;

    auto it = shader_type_map.find(type);
    if(it != shader_type_map.end()) return createShader(it->second, properties->GetText());

    return nullptr;
}

std::unique_ptr<Shader> Shader::createShader(GLenum type, const std::string& source) {
    auto shader = std::make_unique<Shader>(glCreateShader(type));

    if(shader->init(source)) return std::move(shader);

    return nullptr;
}

ShaderProgram::ShaderProgram()
: program_handle{glCreateProgram()} {
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_handle);
}

bool ShaderProgram::attachShader(AssetManager& asset_mgr, AssetHandle handle) {
    auto shader_ptr = asset_mgr.getAsset<Shader>(handle);
    if(!shader_ptr) return false;

    glAttachShader(this->program_handle, shader_ptr->getHandle());
    this->shader_handles.push_back(handle);
    return true;
}

void ShaderProgram::bindAttribLocation(const std::string& name, unsigned int index) {
    glBindAttribLocation(this->program_handle, index, name.c_str());
}

void ShaderProgram::bindFragDataLocation(const std::string& name, unsigned int index) {
    glBindFragDataLocation(this->program_handle, index, name.c_str());
}

bool ShaderProgram::link() {
    if(this->shader_handles.empty()) return false;

    glLinkProgram(this->program_handle);

    int status;
    glGetProgramiv(this->program_handle, GL_LINK_STATUS, &status);

    if(status == int(GL_FALSE)) {
        GLint max_length = 0;
        glGetProgramiv(this->program_handle, GL_INFO_LOG_LENGTH, &max_length);

        std::string error_message;
        error_message.resize(max_length);
        glGetProgramInfoLog(this->program_handle, max_length, &max_length, &error_message[0]);

        BEMBEL_LOG_ERROR() << "Can't link ShaderProgram" << std::endl << error_message << std::endl;

        this->ready_to_use = false;
        return false;
    }

    this->ready_to_use = true;
    return true;
}

bool ShaderProgram::use() {
    if(!this->ready_to_use) return false;

    glUseProgram(this->program_handle);
    return true;
}

const std::string& ShaderProgram::getTypeName() {
    const static std::string type_name = "ShaderProgram";
    return type_name;
}

std::unique_ptr<ShaderProgram> ShaderProgram::loadAsset(
    AssetManager& asset_mgr, const std::string& file_name) {
    xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return nullptr;
    }

    const xml::Element* root = doc.FirstChildElement(getTypeName().c_str());
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'Shader'"
                           << std::endl;
        return nullptr;
    }
    auto pragram = createAsset(asset_mgr, root);
    if(!pragram) {
        BEMBEL_LOG_ERROR() << "Failed to create shader program from file '" << file_name << "'"
                           << std::endl;
    }
    return std::move(pragram);
}

std::unique_ptr<ShaderProgram> ShaderProgram::createAsset(
    AssetManager& asset_mgr, const xml::Element* properties) {
    auto programm = std::make_unique<ShaderProgram>();

    for(auto it : xml::IterateChildElements(properties, "Shader")) {
        auto shader = asset_mgr.requestAsset<Shader>(it);

        if(!programm->attachShader(asset_mgr, shader)) return nullptr;
    }

    if(!programm->link()) { return nullptr; }

    return std::move(programm);
}

void ShaderProgram::deleteAsset(AssetManager& asset_mgr, std::unique_ptr<ShaderProgram> program) {
    std::vector<AssetHandle> shaders = program->shader_handles;
    program.release();

    for(auto shader_handle : shaders) asset_mgr.releaseAsset(shader_handle);
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const {
    auto it = this->uniorm_locations.find(name);
    if(it != this->uniorm_locations.end()) return it->second;

    GLint location = glGetUniformLocation(this->program_handle, name.c_str());

    if(location >= 0) {
        // add location to the map for faster lookup in the future.
        this->uniorm_locations.emplace(name, location);
    }

    return location;
}

GLuint ShaderProgram::getUniformBlockIndex(const std::string& name) const {
    return glGetUniformBlockIndex(this->program_handle, name.c_str());
}

GLint ShaderProgram::getUniformBlockDataSize(GLuint block_index) const {
    GLint size;
    glGetActiveUniformBlockiv(this->program_handle, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
    return size;
}
GLint ShaderProgram::getUniformBlockActiveUniforms(GLuint block_index) const {
    GLint active_uniforms;
    glGetActiveUniformBlockiv(
        this->program_handle, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniforms);
    return active_uniforms;
}

void ShaderProgram::getUniformBlockActiveUniformIndices(
    GLuint block_index, std::vector<GLint>* indices) const {
    indices->resize(getUniformBlockActiveUniforms(block_index));
    glGetActiveUniformBlockiv(
        this->program_handle, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &(*indices)[0]);
}

GLint ShaderProgram::getActiveUniformOffset(GLuint uniform_index) const {
    GLint offset;
    glGetActiveUniformsiv(this->program_handle, 1, &uniform_index, GL_UNIFORM_OFFSET, &offset);
    return offset;
}

void ShaderProgram::getActiveUniform(
    GLuint uniform_index, GLint* size, GLenum* type, std::string* name) const {
    GLint name_length;

    glGetActiveUniformsiv(
        this->program_handle, 1, &uniform_index, GL_UNIFORM_NAME_LENGTH, &name_length);
    GLchar* buffer = new GLchar[name_length];
    glGetActiveUniform(
        this->program_handle, uniform_index, name_length, nullptr, size, type, buffer);

    *name = buffer;
    delete[] buffer;
}

} // namespace bembel::kernel
