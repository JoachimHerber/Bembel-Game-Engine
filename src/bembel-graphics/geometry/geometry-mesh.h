#ifndef BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYMESH_H_
#define BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYMESH_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <string>
#include <unordered_map>

#include <bembel-kernel/assets/serial-asset-loader.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GeometryMesh
{
public:
	using DefaultLoaderType = SerialAssetLoader<GeometryMesh>;

public:
	GeometryMesh();
	~GeometryMesh();

	bool GetSubMesh(const std::string& name, unsigned& first_index, unsigned& num_indices);
	GLuint GetVAO() const;

	const static std::string& GetTypeName();

	static std::unique_ptr<GeometryMesh> LoadAsset(
		AssetManager* asset_manager, const std::string& file_name);
	static std::unique_ptr<GeometryMesh> LoadAsset(
		AssetManager* asset_manager, const xml::Element* properties);

	static void DeleteAsset(
		AssetManager* asset_manager, std::unique_ptr<GeometryMesh> mesh);

private:
	static std::unique_ptr<GeometryMesh> CreateGeometryMesh(const xml::Element* properties);


	static bool ParseVertexData(const xml::Element*, std::vector<float>&);
	static bool ParseIndexData(const xml::Element*, std::vector<unsigned>&);

private:
	GLuint vao_;
	GLuint vbo_;
	GLuint ibo_;

	struct SubMesh
	{
		unsigned first_index;
		unsigned num_indices;
	};
	std::unordered_map<std::string, SubMesh> sub_meshs_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
