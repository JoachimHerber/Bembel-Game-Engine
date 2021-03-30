#include "./mesh.hpp"

namespace bembel::graphics {

GeometryMesh::GeometryMesh() {
}

GeometryMesh::~GeometryMesh() {
}

bool GeometryMesh::getSubMesh(
    const std::string& name, unsigned& first_index, unsigned& num_indices) {
    auto it = this->sub_meshs.find(name);
    if(it == this->sub_meshs.end()) return false;

    first_index = it->second.first_index;
    num_indices = it->second.num_indices;
    return true;
}

GLuint GeometryMesh::getVAO() const {
    return this->vao;
}

const std::string& GeometryMesh::getTypeName() {
    const static std::string type_name = "GeometryMesh";
    return type_name;
}

std::unique_ptr<GeometryMesh> GeometryMesh::loadAsset(
    kernel::AssetManager&, const std::string& file_name) {
    base::xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return nullptr;
    }

    const base::xml::Element* root = doc.FirstChildElement("GeometryMesh");
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'GeometryMesh'"
                           << std::endl;
        return nullptr;
    }
    return GeometryMesh::createGeometryMesh(root);
}

std::unique_ptr<GeometryMesh> GeometryMesh::createAsset(
    kernel::AssetManager&, const base::xml::Element* properties) {
    return GeometryMesh::createGeometryMesh(properties);
}

void GeometryMesh::deleteAsset(kernel::AssetManager&, std::unique_ptr<GeometryMesh>) {
}

std::unique_ptr<GeometryMesh> GeometryMesh::createGeometryMesh(
    const base::xml::Element* properties) {
    std::vector<float>    vertex_data;
    std::vector<unsigned> index_data;

    auto format    = properties->FirstChildElement("Format");
    auto vertices  = properties->FirstChildElement("Vertices");
    auto indices   = properties->FirstChildElement("Indices");
    auto sub_meshs = properties->FirstChildElement("SubMeshs");

    if(!format || !vertices || !indices || !sub_meshs) { return nullptr; }

    unsigned num_vertices;
    unsigned num_indices;
    if(!base::xml::getAttribute(vertices, "count", num_vertices)
       || !base::xml::getAttribute(indices, "count", num_indices)) {
        return nullptr;
    }

    vertex_data.resize(num_vertices);
    index_data.resize(num_indices);

    if(!parseVertexData(vertices, vertex_data) || !parseIndexData(indices, index_data)) {
        return nullptr;
    }

    auto mesh = std::make_unique<GeometryMesh>();

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), &vertex_data[0], GL_STATIC_DRAW);
    glGenBuffers(1, &mesh->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned) * index_data.size(),
        &index_data[0],
        GL_STATIC_DRAW);

    for(const base::xml::Element* attrib : base::xml::IterateChildElements(format)) {
        unsigned index, components, stride, offset;
        base::xml::getAttribute(attrib, "index", index);
        base::xml::getAttribute(attrib, "components", components);
        base::xml::getAttribute(attrib, "stride", stride);
        base::xml::getAttribute(attrib, "offset", offset);

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            components,
            GL_FLOAT,
            GL_FALSE,
            stride * sizeof(float),
            (void*)(offset * sizeof(float)));
    }
    glBindVertexArray(0);

    for(const base::xml::Element* sub_mesh : base::xml::IterateChildElements(sub_meshs)) {
        std::string name;
        unsigned    first, count;
        base::xml::getAttribute(sub_mesh, "name", name);
        base::xml::getAttribute(sub_mesh, "first_index", first);
        base::xml::getAttribute(sub_mesh, "num_indiecs", count);
        mesh->sub_meshs.emplace(name, SubMesh{first, count});
    }
    return std::move(mesh);
}

bool GeometryMesh::parseVertexData(const base::xml::Element* element, std::vector<float>& _data) {
    if(element->GetText() == nullptr) return false;

    std::stringstream string_stream;
    string_stream << element->GetText();
    for(size_t n = 0; n < _data.size(); ++n) { string_stream >> _data[n]; }
    return true;
}

bool GeometryMesh::parseIndexData(const base::xml::Element* element, std::vector<unsigned>& _data) {
    if(element->GetText() == nullptr) return false;

    std::stringstream string_stream;
    string_stream << element->GetText();
    for(size_t n = 0; n < _data.size(); ++n) { string_stream >> _data[n]; }
    return true;
}

} // namespace bembel::graphics
