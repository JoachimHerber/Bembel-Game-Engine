﻿module;
#include <glbinding/gl/gl.h>

#include <fstream>
#include <memory>
module bembel.kernel.rendering;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;
using namespace gl;

Shader::Shader(Type type, uint handle) : m_type{type}, m_handle(handle) {}

Shader::~Shader() {
    ::gl::glDeleteShader(m_handle);
}

char const* GetShaderTypeName(Shader::Type type) {
    switch(type) {
        case Shader::Type::VERTEX: return "VertexShader";
        case Shader::Type::FRAGMENT: return "FragmentShader";
        case Shader::Type::GEOMETRY: return "GeometryShader";
        default: return "Shader";
    }
}

bool Shader::init(std::string_view source) {
    char const* c_str  = source.data();
    int const   length = source.length();
    glShaderSource(m_handle, 1, &c_str, &length);
    glCompileShader(m_handle);

    int status;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
    if(status != int(GL_TRUE)) {
        char    error_message[4096];
        GLsizei size;
        glGetShaderInfoLog(m_handle, 4096, &size, error_message);

        logError(
            "Can't compile {}\n{}\nSource:\n{}", GetShaderTypeName(m_type), error_message, source
        );

        return false;
    }
    return true;
}

uint Shader::getHandle() const {
    return m_handle;
}

std::unique_ptr<Shader> Shader::loadAsset(std::filesystem::path file) {
    std::string extension = file.extension().string();
    if(extension == ".vert" || extension == ".frag") {
        std::ifstream fs{file};

        fs.seekg(0, std::ios::end);
        size_t      size = fs.tellg();
        std::string source(size, ' ');
        fs.seekg(0);
        fs.read(source.data(), size);

        return createShader(extension == ".vert" ? Type::VERTEX : Type::FRAGMENT, source);
    }

    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("Shader");
    if(!root) {
        logError("File '{}' has no root element 'Shader'", file_path);
        return nullptr;
    }
    return createAsset(root);
}

std::unique_ptr<Shader> Shader::createAsset(xml::Element const* properties) {
    std::string file;
    if(xml::getAttribute(properties, "file", file)) {
        auto path = AssetLocator::getInstance().findAssetLocation<Shader>(file);
        if(path) return Shader::loadAsset(path.value());
    }

    static Dictionary<Type> const shader_type_map{
        {"GL_VERTEX_SHADER", Type::VERTEX},
        {"GL_FRAGMENT_SHADER", Type::FRAGMENT},
        {"GL_GEOMETRY_SHADER", Type::GEOMETRY},
    };

    if(properties->GetText() == nullptr) return nullptr;

    std::string type;
    if(!xml::getAttribute(properties, "type", type)) return nullptr;

    auto it = shader_type_map.find(type);
    if(it != shader_type_map.end()) return createShader(it->second, properties->GetText());

    return nullptr;
}

std::unique_ptr<Shader> Shader::createShader(Type type, std::string_view source) {
    std::unique_ptr<Shader> shader;
    switch(type) {
        case Type::VERTEX:
            shader = std::make_unique<Shader>(type, glCreateShader(GL_VERTEX_SHADER));
            break;
        case Type::FRAGMENT:
            shader = std::make_unique<Shader>(type, glCreateShader(GL_FRAGMENT_SHADER));
            break;
    }

    if(shader->init(source)) return std::move(shader);

    return nullptr;
}

ShaderProgram::ShaderProgram() : m_program_handle{glCreateProgram()} {}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_program_handle);
}

bool ShaderProgram::attachShader(Asset<Shader> shader) {
    if(!shader) return false;

    glAttachShader(m_program_handle, shader->getHandle());
    m_shader_handles.push_back(std::move(shader));
    return true;
}

void ShaderProgram::bindAttribLocation(std::string_view name, unsigned int index) {
    std::string name_str{name};
    glBindAttribLocation(m_program_handle, index, name_str.c_str());
}

void ShaderProgram::bindFragDataLocation(std::string_view name, unsigned int index) {
    std::string name_str{name};
    glBindFragDataLocation(m_program_handle, index, name_str.c_str());
}

bool ShaderProgram::setUniform(std::string_view name, float value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniform1f(location, value);
    return true;
}
bool ShaderProgram::setUniform(std::string_view name, vec2 value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniform2f(location, value.x, value.y);
    return true;
}
bool ShaderProgram::setUniform(std::string_view name, vec3 value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniform3f(location, value.x, value.y, value.z);
    return true;
}
bool ShaderProgram::setUniform(std::string_view name, In<vec4> value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniform4f(location, value.x, value.y, value.z, value.w);
    return true;
}

bool ShaderProgram::setUniform(std::string_view name, int value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniform1i(location, value);
    return true;
}

bool ShaderProgram::setUniform(std::string_view name, mat4 const& value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniformMatrix4fv(location, 1, gl::GL_FALSE, &(value[0][0]));
    return true;
}

bool ShaderProgram::setUniform(std::string_view name, In<std::span<const mat4>> value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniformMatrix4fv(location, value.size(), gl::GL_FALSE, (const GLfloat*)value.data());
    return true;
}

bool ShaderProgram::setUniform(std::string_view name, In<std::span<const float>> value) {
    GLint location = getUniformLocation(name);
    if(location == -1) return false;

    glUniform1fv(location, value.size(), (const GLfloat*)value.data());
    return true;
}

bool ShaderProgram::link() {
    if(m_shader_handles.empty()) return false;

    glLinkProgram(m_program_handle);

    int status;
    glGetProgramiv(m_program_handle, GL_LINK_STATUS, &status);

    if(status == int(GL_FALSE)) {
        GLint max_length = 0;
        glGetProgramiv(m_program_handle, GL_INFO_LOG_LENGTH, &max_length);

        std::string error_message;
        error_message.resize(max_length);
        glGetProgramInfoLog(m_program_handle, max_length, &max_length, &error_message[0]);

        logError("Can't link ShaderProgram\n {}", error_message);

        m_ready_to_use = false;
        return false;
    }

    m_ready_to_use = true;
    return true;
}

bool ShaderProgram::use() {
    if(!m_ready_to_use) return false;

    glUseProgram(m_program_handle);
    return true;
}

std::unique_ptr<ShaderProgram> ShaderProgram::loadAsset(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("ShaderProgram");
    if(!root) {
        logError("File '{}'  has no root element 'Shader'", file_path);
        return nullptr;
    }
    auto pragram = createAsset(root);
    if(!pragram) { logError("Failed to create shader program from file '{}'", file_path); }
    return std::move(pragram);
}

std::unique_ptr<ShaderProgram> ShaderProgram::createAsset(xml::Element const* properties) {
    auto programm = std::make_unique<ShaderProgram>();

    for(auto it : xml::IterateChildElements(properties, "Shader")) {
        Asset<Shader> shader;
        shader.request(it);

        if(!programm->attachShader(std::move(shader))) return nullptr;
    }

    if(!programm->link()) { return nullptr; }

    return std::move(programm);
}

GLint ShaderProgram::getUniformLocation(std::string_view name) const {
    auto it = m_uniorm_locations.find(name);
    if(it != m_uniorm_locations.end()) return it->second;

    std::string name_str{name};
    GLint       location = glGetUniformLocation(m_program_handle, name_str.c_str());

    if(location >= 0) {
        // add location to the map for faster lookup in the future.
        m_uniorm_locations.emplace(name, location);
    } else {
        logError("Unknown Uniform Location for '{}'", name);
    }

    return location;
}

GLuint ShaderProgram::getUniformBlockIndex(std::string_view name) const {
    std::string name_str{name};
    return glGetUniformBlockIndex(m_program_handle, name_str.c_str());
}

GLint ShaderProgram::getUniformBlockDataSize(GLuint block_index) const {
    GLint size;
    glGetActiveUniformBlockiv(m_program_handle, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
    return size;
}
GLint ShaderProgram::getUniformBlockActiveUniforms(GLuint block_index) const {
    GLint active_uniforms;
    glGetActiveUniformBlockiv(
        m_program_handle, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniforms
    );
    return active_uniforms;
}

void ShaderProgram::getUniformBlockActiveUniformIndices(
    GLuint block_index, std::vector<GLint>* indices
) const {
    indices->resize(getUniformBlockActiveUniforms(block_index));
    glGetActiveUniformBlockiv(
        m_program_handle, block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &(*indices)[0]
    );
}

GLint ShaderProgram::getActiveUniformOffset(GLuint uniform_index) const {
    GLint offset;
    glGetActiveUniformsiv(m_program_handle, 1, &uniform_index, GL_UNIFORM_OFFSET, &offset);
    return offset;
}

void ShaderProgram::getActiveUniform(uint uniform_index, int* size, uint* type, std::string* name)
    const {
    GLint name_length;

    glGetActiveUniformsiv(
        m_program_handle, 1, &uniform_index, GL_UNIFORM_NAME_LENGTH, &name_length
    );
    GLchar* buffer = new GLchar[name_length];
    glGetActiveUniform(
        m_program_handle, uniform_index, name_length, nullptr, size, (GLenum*)type, buffer
    );

    *name = buffer;
    delete[] buffer;
}

} // namespace bembel::kernel
