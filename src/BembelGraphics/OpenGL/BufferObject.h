#ifndef BEMBEL_BUFFEROBJECT_H
#define BEMBEL_BUFFEROBJECT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <memory>
#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API BufferObject
{
public:
	BufferObject(GLenum target, GLenum usage = GL_STATIC_DRAW);
	BufferObject(GLenum target, GLsizeiptr sizeInByte, GLenum usage = GL_STATIC_DRAW);
	~BufferObject();

	void SetData(GLsizeiptr sizeInByte, void* data);
	template<typename T>
	void SetData(const std::vector<T>& data)
	{
		SetData(sizeof(T)*data.size(), (void*)(&data[0]));
	}

	void BufferData(GLsizeiptr sizeInByte, void* data);
	bool BufferSubData(GLsizeiptr sizeInByte, GLsizeiptr offset, void* data);

	GLenum     GetTarget() const;
	GLenum     GetUsage() const;
	GLuint     GetHandle() const;
	GLsizeiptr GetCapacity() const;

	static std::shared_ptr<BufferObject> CreateVertexBufferObject(const std::vector<float>& data);
	static std::shared_ptr<BufferObject> CreateIndexBufferObject(const std::vector<unsigned>& data);

private:
	GLenum _target;
	GLenum _usage;
	GLuint _handle;

	GLsizeiptr _capacity;
};

using BufferObjectPtr = std::shared_ptr<BufferObject>;

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
