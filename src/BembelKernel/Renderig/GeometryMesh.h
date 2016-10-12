#ifndef BEMBEL_GEOMETRYMESH_H
#define BEMBEL_GEOMETRYMESH_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelKernel/Assets/SerialAssetLoader.hpp>

#include <memory>
#include <string>
#include <unordered_map>

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

	void Bind();

	bool GetSubMesh(const std::string& name, unsigned& firstIndex, unsigned& numIndices);

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
