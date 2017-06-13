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
	static std::unique_ptr<GeometryMesh> CreateAsset(
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
