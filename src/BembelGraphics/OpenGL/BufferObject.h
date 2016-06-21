#ifndef BEMBEL_BUFFEROBJECT_H
#define BEMBEL_BUFFEROBJECT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API BufferObject
{
public:
	BufferObject(GLenum target);
	~BufferObject();

	void Init();
	void Init(GLsizeiptr sizeInByte, GLenum usage = GL_STATIC_DRAW);
	void Init(GLsizeiptr sizeInByte, void* data, GLenum usage = GL_STATIC_DRAW);
	void Cleanup();

	void SetData(GLsizeiptr sizeInByte, void* data, GLenum usage = GL_STATIC_DRAW);

	GLenum GetTarget() const;
	GLuint GetHandle() const;

private:
	GLenum _target;
	GLuint _handle;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
