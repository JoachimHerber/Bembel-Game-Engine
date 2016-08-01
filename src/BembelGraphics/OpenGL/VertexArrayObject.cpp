/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "VertexArrayObject.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &_handle);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &_handle);
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(_handle);
}

void VertexArrayObject::EnableVertexAttribArray(GLuint index)
{
	glEnableVertexAttribArray(index);
}

void VertexArrayObject::DisableVertexAttribArray(GLuint index)
{
	glDisableVertexAttribArray(index);
}

bool VertexArrayObject::SpecifyVertexAttribArray(
	GLuint index,
	BufferObjectPtr vbo,
	GLint components, 
	GLsizei stride,
	const void* offset,
	GLenum type, 
	bool normalize)
{
	if (vbo && vbo->GetTarget() != GL_ARRAY_BUFFER)
		return false;

	if (_vbos.size() <= index)
		_vbos.resize(index + 1, nullptr);

	_vbos[index] = vbo;

	glBindVertexArray(_handle);
	if (vbo)
	{
		glEnableVertexAttribArray(index);
		glBindBuffer(vbo->GetTarget(), vbo->GetHandle());
		glVertexAttribPointer(index, components, type, normalize ? GL_TRUE : GL_FALSE, stride, offset);
	}
	else
	{
		glDisableVertexAttribArray(index);
	}
	glBindVertexArray(0);

	return true;
}

bool VertexArrayObject::SetIndexBuffer(BufferObjectPtr ibo, GLenum type)
{
	if (ibo && ibo->GetTarget() != GL_ELEMENT_ARRAY_BUFFER)
		return false;

	_ibo = ibo;
	_indexType = type;
	if (_ibo)
	{
		glBindVertexArray(_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo->GetHandle());
		glBindVertexArray(0);
	}
}

bool VertexArrayObject::HasIndexBuffer() const
{
	return _ibo != nullptr;
}

GLenum VertexArrayObject::GetIndexType() const
{
	if (_ibo != nullptr)
		return _indexType;
	return GL_NONE;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
