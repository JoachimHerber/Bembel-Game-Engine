/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "BufferObject.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

BufferObject::BufferObject(GLenum target)
	: _target(target)
	, _handle(0)
{}

BufferObject::~BufferObject()
{
	Cleanup();
}

void BufferObject::Init()
{
	glGenBuffers(1, &_handle);
	glBindBuffer(_target, _handle);
	glBindBuffer(_target, 0);
}

void BufferObject::Init(GLsizeiptr sizeInByte, GLenum usage /*= GL_STATIC_DRAW*/)
{
	glGenBuffers(1, &_handle);
	glBindBuffer(_target, _handle);
	glBufferData(_target, sizeInByte, nullptr, usage);
	glBindBuffer(_target, 0);
}

void BufferObject::Init(GLsizeiptr sizeInByte, void* data, GLenum usage /*= GL_STATIC_DRAW*/)
{
	glGenBuffers(1, &_handle);
	glBindBuffer(_target, _handle);
	glBufferData(_target, sizeInByte, data, usage);
	glBindBuffer(_target, 0);
}

void BufferObject::Cleanup()
{
	if (_handle != 0)
	{
		glDeleteBuffers(1, &_handle);
		_handle = 0;
	}
}

void BufferObject::SetData(GLsizeiptr sizeInByte, void* data, GLenum usage /*= GL_STATIC_DRAW*/)
{
	glBindBuffer(_target, _handle);
	glBufferData(_target, sizeInByte, data, usage);
	glBindBuffer(_target, 0);
}

GLenum BufferObject::GetTarget() const
{
	return _target;
}

GLuint BufferObject::GetHandle() const
{
	return _handle;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
