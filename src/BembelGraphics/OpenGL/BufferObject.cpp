/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "BufferObject.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

BufferObject::BufferObject(GLenum target, GLenum usage)
	: _target(target)
	, _usage(usage)
	, _handle(0)
	, _capacity(0)
{
	glGenBuffers(1, &_handle);
	glBindBuffer(_target, _handle);
	glBindBuffer(_target, 0);
}
BufferObject::BufferObject(GLenum target, GLsizeiptr sizeInByte,  GLenum usage)
	: _target(target)
	, _usage(usage)
	, _handle(0)
	, _capacity(sizeInByte)
{
	glGenBuffers(1, &_handle);
	glBindBuffer(_target, _handle);
	glBufferData(_target, sizeInByte, nullptr, usage);
	glBindBuffer(_target, 0);
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &_handle);
}

void BufferObject::SetData(GLsizeiptr sizeInByte, void* data)
{
	glBindBuffer(_target, _handle);
	if (sizeInByte < _capacity)
	{
		glBufferSubData(_target, 0, sizeInByte, data);
	}
	else
	{
		glBufferData(_target, sizeInByte, data, _usage);
		_capacity = sizeInByte;
	}
	glBindBuffer(_target, 0);
}

void BufferObject::BufferData(GLsizeiptr sizeInByte, void* data)
{
	glBindBuffer(_target, _handle);
	glBufferData(_target, sizeInByte, nullptr, _usage);
	_capacity = sizeInByte;
	glBindBuffer(_target, 0);
}

bool BufferObject::BufferSubData(
	GLsizeiptr sizeInByte, GLsizeiptr offset, void* data)
{
	if (offset + sizeInByte > _capacity)
		return false;

	glBindBuffer(_target, _handle);
	glBufferSubData(_target, sizeInByte, offset, data);
	glBindBuffer(_target, 0);
	return true;
}

GLenum BufferObject::GetTarget() const
{
	return _target;
}
GLenum BufferObject::GetUsage() const
{
	return _usage;
}
GLuint BufferObject::GetHandle() const
{
	return _handle;
}
GLsizeiptr BufferObject::GetCapacity() const
{
	return _capacity;
}

BufferObjectPtr BufferObject::CreateVertexBufferObject(
	const std::vector<float>& data)
{
	BufferObjectPtr vbo =
		std::make_shared<BufferObject>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vbo->BufferData(data.size()*sizeof(float), (void*)(&data[0]));
	return vbo;
}

BufferObjectPtr BufferObject::CreateIndexBufferObject(
	const std::vector<unsigned>& data)
{
	BufferObjectPtr ibo = 
		std::make_shared<BufferObject>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	ibo->BufferData(data.size()*sizeof(unsigned), (void*)(&data[0]));
	return ibo;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
