#ifndef BEMBEL_VERTEXARRAYOBJECT_H
#define BEMBEL_VERTEXARRAYOBJECT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include "BufferObject.h"

#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Bind();

	void EnableVertexAttribArray(GLuint index);
	void DisableVertexAttribArray(GLuint index);

	/**
	 * Specifies the data for a vertex attribute array.
	 * @param index 
	 *     The index of the vertex attribute
	 * @param vbo
	 *     A pointer to the Vertex Buffer Object (VBO), that contains the data
	 *     for the vertex attribute.
	 *     In order to disable the Vertex attribute set vbo to nullptr.
	 * @param components
	 *     The number of components of the attribute.
	 * @param stride
	 *     The stride specifies the offset in byte between the data for
	 *     one vertex and the next.
	 *     Can be set to 0, if the data is tightly pack (the VBO contains
	 *     data for one attribute only and there is no gap between the data
	 *     for one vertex and the next).
	 *     For example the stride of a VBO with the format 
	 *     (x,y,z, nx, ny, nz, uv) would be 8*sizeof(DataType)
	 * @param offset
	 *		The offset specifies the position of the data for the attribute
	 *      within the VBO.
	 *      For example for a VBO with the format (x,y,z, nx, ny, nz, uv)
	 *		the offset for the position (x,y,z) is 0,
	 *      the offset for the normal (nx, ny, nz) is 3*sizeof(DataType) and
	 *      the offset for the texture coordinate (u,v) is 6*sizeof(DataType).
	 *@param type
	 *      The data type of the data in the VBO (usually GL_FLOAT).
	 *@param normalize
	 *      Specifies whether integer attributes should be interpreted as normalized float
	 *		For Example this should be set to true for color values which are
	 *		given as unsigned bytes.
	 *@return
	 *     false if the specified buffer object id not a VBO (target != GL_ARRAY_BUFFER).
	 *     true otherwise
	 */
	bool SpecifyVertexAttribArray(
		GLuint index,
		BufferObjectPtr vbo,
		GLint components = 3,
		GLsizei stride = 0,
		const void* offset = 0,
		GLenum type = GL_FLOAT,
		bool normalize = false);

	bool SetIndexBuffer(BufferObjectPtr ibo, GLenum type);

	bool   HasIndexBuffer() const;
	GLenum GetIndexType() const;

public:
	GLuint _handle;

	std::vector<BufferObjectPtr> _vbos;
	BufferObjectPtr              _ibo;
	GLenum                       _indexType;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
