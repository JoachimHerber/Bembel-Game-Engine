#ifndef BEMBEL_SHADERPROGRAM_H
#define BEMBEL_SHADERPROGRAM_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API ShaderProgram final
{
public:
	ShaderProgram();
	~ShaderProgram();

	bool AttachShader(GLenum type, const std::string& source);
	bool AttachShaderFromFile(GLenum type, const std::string& fileName);

	void BindAttribLocation(const std::string& name, unsigned int index);
	void BindFragDataLocation(const std::string& name, unsigned int index);

	GLint  GetUniformLocation(const std::string& name) const;
	GLuint GetUniformBlockIndex(const std::string& name) const;


	bool Link();

	bool Use();

private:
	GLuint              _programHandle;
	std::vector<GLuint> _shaderHandles;

	bool _readyToUse = false;

	mutable std::unordered_map<std::string, GLint> _uniormLocations;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
