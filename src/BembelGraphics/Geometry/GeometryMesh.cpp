/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryMesh.h"

#include <vector>
#include <sstream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryMesh::GeometryMesh()
{}

GeometryMesh::~GeometryMesh()
{}

bool GeometryMesh::GetSubMesh(
	const std::string& name, 
	unsigned& firstIndex, 
	unsigned& numIndices)
{
	auto it = _subMeshes.find(name);
	if (it == _subMeshes.end())
		return false;

	firstIndex = it->second.firstIndex;
	numIndices = it->second.numIndices;
	return true;
}

GLuint GeometryMesh::GetVAO() const
{
	return _vao;
}

const std::string& GeometryMesh::GetTypeName()
{
	const static std::string typeName = "GeometryMesh";
	return typeName;
}

std::unique_ptr<GeometryMesh> GeometryMesh::CreateAsset(
	AssetManager*, const xml::Element* properties)
{
	std::vector<float> vertexData;
	std::vector<unsigned> indexData;

	auto format   = properties->FirstChildElement("Format");
	auto vertices = properties->FirstChildElement("Vertices");
	auto indices  = properties->FirstChildElement("Indices");
	auto subMeshs = properties->FirstChildElement("SubMeshs");

	if (!format || !vertices || !indices || !subMeshs)
	{
		return nullptr;
	}

	unsigned numVertices;
	unsigned numIndices;
	if (!xml::GetAttribute(vertices, "count", numVertices) ||
		!xml::GetAttribute(indices, "count", numIndices))
	{
		return nullptr;
	}

	vertexData.resize(numVertices);
	indexData.resize(numIndices);

	if (!ParseVertexData(vertices, vertexData) ||
		!ParseIndexData(indices, indexData))
	{
		return nullptr;
	}

	auto mesh = std::make_unique<GeometryMesh>();

	glGenVertexArrays(1, &mesh->_vao);
	glBindVertexArray(mesh->_vao);

	glGenBuffers(1, &mesh->_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(float)*vertexData.size(),
				 &vertexData[0], GL_STATIC_DRAW);
	glGenBuffers(1, &mesh->_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				 sizeof(unsigned)*indexData.size(), 
				 &indexData[0], GL_STATIC_DRAW);

	for (const xml::Element* attrib : xml::IterateChildElements(format))
	{
		unsigned index, components, stride, offset;
		xml::GetAttribute(attrib, "index",      index);
		xml::GetAttribute(attrib, "components", components);
		xml::GetAttribute(attrib, "stride",     stride);
		xml::GetAttribute(attrib, "offset",     offset);

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index, components, GL_FLOAT, 
			GL_FALSE, stride*sizeof(float), 
			(void*)(offset*sizeof(float)));
	}
	glBindVertexArray(0);

	for (const xml::Element* subMesh : xml::IterateChildElements(subMeshs))
	{
		std::string name;
		unsigned first, count;
		xml::GetAttribute(subMesh, "name", name);
		xml::GetAttribute(subMesh, "first_index", first);
		xml::GetAttribute(subMesh, "num_indiecs", count);
		mesh->_subMeshes.emplace(
			name, SubMesh{first, count});
	}
	return std::move(mesh);
}

std::unique_ptr<GeometryMesh> GeometryMesh::LoadFromFile(
	AssetManager* assetMgr,
	const std::string& fileName)
{
	xml::Document doc;
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << fileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("GeometryMesh");
	if (!root)
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << fileName << "' has no root element 'GeometryMesh'"
			<< std::endl;
		return false;
	}
	return CreateAsset(assetMgr, root);
}

bool GeometryMesh::ParseVertexData(
	const xml::Element* element, std::vector<float>& data)
{
	if (element->GetText() == nullptr)
		return false;
	
	std::stringstream stream;
	stream << element->GetText();
	for (size_t n = 0; n<data.size(); ++n)
	{
		stream >> data[n];
	}
	return true;
}

bool GeometryMesh::ParseIndexData(
	const xml::Element* element, std::vector<unsigned>& data)
{
	if (element->GetText() == nullptr)
		return false;

	std::stringstream stream;
	stream << element->GetText();
	for (size_t n = 0; n<data.size(); ++n)
	{
		stream >> data[n];
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
