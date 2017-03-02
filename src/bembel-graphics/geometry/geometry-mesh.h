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

	bool GetSubMesh(const std::string& name, unsigned& firstIndex, unsigned& numIndices);
	GLuint GetVAO() const;

	const static std::string& GetTypeName();

	static std::unique_ptr<GeometryMesh> CreateAsset(AssetManager* assetMgr, const xml::Element* properties);
	static std::unique_ptr<GeometryMesh> LoadFromFile(AssetManager* assetMgr, const std::string& fileName);

private:
	static bool ParseVertexData(const xml::Element*, std::vector<float>&);
	static bool ParseIndexData(const xml::Element*, std::vector<unsigned>&);

private:
	GLuint _vao;
	GLuint _vbo;
	GLuint _ibo;

	struct SubMesh
	{
		unsigned    firstIndex;
		unsigned    numIndices;
	};
	std::unordered_map<std::string, SubMesh> _subMeshes;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
