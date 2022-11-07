module;
#include <glbinding/gl/gl.h>

#include "bembel/pch.h"
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

GeometryMesh::GeometryMesh() {}

GeometryMesh::~GeometryMesh() {}

bool GeometryMesh::getSubMesh(std::string_view name, uint& first_index, uint& num_indices) {
    auto it = m_sub_meshs.find(name);
    if(it == m_sub_meshs.end()) return false;

    first_index = it->second.first_index;
    num_indices = it->second.num_indices;
    return true;
}

uint GeometryMesh::getVAO() const {
    return m_vao;
}

std::unique_ptr<GeometryMesh> GeometryMesh::loadAsset(AssetManager&, std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("GeometryMesh");
    if(!root) {
        log().error("File '{}' has no root element 'GeometryMesh'", file_path);
        return nullptr;
    }
    return GeometryMesh::createGeometryMesh(root);
}

std::unique_ptr<GeometryMesh> GeometryMesh::createAsset(
    AssetManager&, xml::Element const* properties
) {
    return GeometryMesh::createGeometryMesh(properties);
}

void GeometryMesh::deleteAsset(kernel::AssetManager&, std::unique_ptr<GeometryMesh>) {}

std::unique_ptr<GeometryMesh> GeometryMesh::createGeometryMesh(xml::Element const* properties) {
    std::vector<float>    vertex_data;
    std::vector<unsigned> index_data;

    auto format    = properties->FirstChildElement("Format");
    auto vertices  = properties->FirstChildElement("Vertices");
    auto indices   = properties->FirstChildElement("Indices");
    auto sub_meshs = properties->FirstChildElement("SubMeshs");

    if(!format || !vertices || !indices || !sub_meshs) { return nullptr; }

    unsigned num_vertices;
    unsigned num_indices;
    if(!xml::getAttribute(vertices, "count", num_vertices)
       || !xml::getAttribute(indices, "count", num_indices)) {
        return nullptr;
    }

    vertex_data.resize(num_vertices);
    index_data.resize(num_indices);

    if(!parseVertexData(vertices, vertex_data) || !parseIndexData(indices, index_data)) {
        return nullptr;
    }

    auto mesh = std::make_unique<GeometryMesh>();

    glGenVertexArrays(1, &mesh->m_vao);
    glBindVertexArray(mesh->m_vao);

    glGenBuffers(1, &mesh->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), &vertex_data[0], GL_STATIC_DRAW
    );
    glGenBuffers(1, &mesh->m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ibo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned) * index_data.size(),
        &index_data[0],
        GL_STATIC_DRAW
    );

    for(xml::Element const* attrib : xml::IterateChildElements(format)) {
        uint index, components, stride, offset;
        xml::getAttribute(attrib, "index", index);
        xml::getAttribute(attrib, "components", components);
        xml::getAttribute(attrib, "stride", stride);
        xml::getAttribute(attrib, "offset", offset);

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            components,
            GL_FLOAT,
            GL_FALSE,
            stride * sizeof(float),
            (void*)(offset * sizeof(float))
        );
    }
    glBindVertexArray(0);

    for(xml::Element const* sub_mesh : xml::IterateChildElements(sub_meshs)) {
        std::string name;
        unsigned    first, count;
        xml::getAttribute(sub_mesh, "name", name);
        xml::getAttribute(sub_mesh, "first_index", first);
        xml::getAttribute(sub_mesh, "num_indiecs", count);
        mesh->m_sub_meshs.emplace(name, SubMesh{first, count});
    }
    return std::move(mesh);
}

bool GeometryMesh::parseVertexData(xml::Element const* element, std::vector<float>& _data) {
    if(element->GetText() == nullptr) return false;

    std::stringstream string_stream;
    string_stream << element->GetText();
    for(size_t n = 0; n < _data.size(); ++n) { string_stream >> _data[n]; }
    return true;
}

bool GeometryMesh::parseIndexData(xml::Element const* element, std::vector<unsigned>& _data) {
    if(element->GetText() == nullptr) return false;

    std::stringstream string_stream;
    string_stream << element->GetText();
    for(size_t n = 0; n < _data.size(); ++n) { string_stream >> _data[n]; }
    return true;
}

} // namespace bembel::graphics
