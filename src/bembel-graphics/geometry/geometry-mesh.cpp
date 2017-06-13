/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-mesh.h"

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
	unsigned& first_index,
	unsigned& num_indices)
{
	auto it = sub_meshs_.find(name);
	if( it == sub_meshs_.end() )
		return false;

	first_index = it->second.first_index;
	num_indices = it->second.num_indices;
	return true;
}

GLuint GeometryMesh::GetVAO() const
{
	return vao_;
}

const std::string& GeometryMesh::GetTypeName()
{
	const static std::string type_name = "GeometryMesh";
	return type_name;
}


std::unique_ptr<GeometryMesh> GeometryMesh::LoadAsset(
	AssetManager*, const std::string& file_name)
{
	xml::Document doc;
	if( doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return nullptr;
	}

	const xml::Element* root = doc.FirstChildElement("GeometryMesh");
	if( !root )
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << file_name << "' has no root element 'GeometryMesh'"
			<< std::endl;
		return nullptr;
	}
	return CreateGeometryMesh(root);
}

std::unique_ptr<GeometryMesh> GeometryMesh::CreateAsset(
	AssetManager* asset_manager, const xml::Element* properties)
{
	return CreateGeometryMesh(properties);
}

void GeometryMesh::DeleteAsset(AssetManager*, std::unique_ptr<GeometryMesh>)
{}

std::unique_ptr<GeometryMesh> GeometryMesh::CreateGeometryMesh(
	const xml::Element* properties)
{
	std::vector<float> vertex_data;
	std::vector<unsigned> index_data;

	auto format = properties->FirstChildElement("Format");
	auto vertices = properties->FirstChildElement("Vertices");
	auto indices = properties->FirstChildElement("Indices");
	auto sub_meshs = properties->FirstChildElement("SubMeshs");

	if( !format || !vertices || !indices || !sub_meshs )
	{
		return nullptr;
	}

	unsigned num_vertices;
	unsigned num_indices;
	if( !xml::GetAttribute(vertices, "count", num_vertices) ||
		!xml::GetAttribute(indices, "count", num_indices) )
	{
		return nullptr;
	}

	vertex_data.resize(num_vertices);
	index_data.resize(num_indices);

	if( !ParseVertexData(vertices, vertex_data) ||
		!ParseIndexData(indices, index_data) )
	{
		return nullptr;
	}

	auto mesh = std::make_unique<GeometryMesh>();

	glGenVertexArrays(1, &mesh->vao_);
	glBindVertexArray(mesh->vao_);

	glGenBuffers(1, &mesh->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float)*vertex_data.size(),
		&vertex_data[0], GL_STATIC_DRAW);
	glGenBuffers(1, &mesh->ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned)*index_data.size(),
		&index_data[0], GL_STATIC_DRAW);

	for( const xml::Element* attrib : xml::IterateChildElements(format) )
	{
		unsigned index, components, stride, offset;
		xml::GetAttribute(attrib, "index", index);
		xml::GetAttribute(attrib, "components", components);
		xml::GetAttribute(attrib, "stride", stride);
		xml::GetAttribute(attrib, "offset", offset);

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index, components, GL_FLOAT,
			GL_FALSE, stride*sizeof(float),
			(void*) (offset*sizeof(float)));
	}
	glBindVertexArray(0);

	for( const xml::Element* sub_mesh : xml::IterateChildElements(sub_meshs) )
	{
		std::string name;
		unsigned first, count;
		xml::GetAttribute(sub_mesh, "name", name);
		xml::GetAttribute(sub_mesh, "first_index", first);
		xml::GetAttribute(sub_mesh, "num_indiecs", count);
		mesh->sub_meshs_.emplace(
			name, SubMesh{first, count});
	}
	return std::move(mesh);
}

bool GeometryMesh::ParseVertexData(
	const xml::Element* element, std::vector<float>& data)
{
	if( element->GetText() == nullptr )
		return false;

	std::stringstream string_stream;
	string_stream << element->GetText();
	for( size_t n = 0; n<data.size(); ++n )
	{
		string_stream >> data[n];
	}
	return true;
}

bool GeometryMesh::ParseIndexData(
	const xml::Element* element, std::vector<unsigned>& data)
{
	if( element->GetText() == nullptr )
		return false;

	std::stringstream string_stream;
	string_stream << element->GetText();
	for( size_t n = 0; n<data.size(); ++n )
	{
		string_stream >> data[n];
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
