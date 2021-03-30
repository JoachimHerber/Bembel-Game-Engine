#ifndef BEMBEL_GRAPHICS_GEOMETRY_MESH_HPP
#define BEMBEL_GRAPHICS_GEOMETRY_MESH_HPP

#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/kernel/open-gl.hpp>

namespace bembel::graphics {

class BEMBEL_API GeometryMesh {
  public:
    using DefaultLoaderType = kernel::SerialAssetLoader<GeometryMesh>;

  public:
    GeometryMesh();
    GeometryMesh(const GeometryMesh&) = delete;
    GeometryMesh(GeometryMesh&&)      = delete;
    ~GeometryMesh();

    bool   getSubMesh(const std::string& name, unsigned& first_index, unsigned& num_indices);
    GLuint getVAO() const;

    const static std::string& getTypeName();

    static std::unique_ptr<GeometryMesh> loadAsset(
        kernel::AssetManager& asset_mgr, const std::string& file_name);
    static std::unique_ptr<GeometryMesh> createAsset(
        kernel::AssetManager& asset_mgr, const base::xml::Element* properties);

    static void deleteAsset(kernel::AssetManager& asset_mgr, std::unique_ptr<GeometryMesh> mesh);

  private:
    static std::unique_ptr<GeometryMesh> createGeometryMesh(const base::xml::Element* properties);

    static bool parseVertexData(const base::xml::Element*, std::vector<float>&);
    static bool parseIndexData(const base::xml::Element*, std::vector<unsigned>&);

  private:
    GLuint vao;
    GLuint vbo;
    GLuint ibo;

    struct SubMesh {
        unsigned first_index;
        unsigned num_indices;
    };
    std::unordered_map<std::string, SubMesh> sub_meshs;
};

} // namespace bembel::graphics
#endif // include guards
