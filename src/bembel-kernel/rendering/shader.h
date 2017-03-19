#ifndef BEMBEL_KERNEL_RENDERING_SHADER_H_
#define BEMBEL_KERNEL_RENDERING_SHADER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Shader final
{
public:
	Shader();
	~Shader();

	bool AttachShader(GLenum type, const std::string& source);
	bool AttachShaderFromFile(GLenum type, const std::string& file_name);

	void BindAttribLocation(const std::string& name, unsigned int index);
	void BindFragDataLocation(const std::string& name, unsigned int index);

	GLint  GetUniformLocation(const std::string& name) const;
	GLuint GetUniformBlockIndex(const std::string& name) const;
	GLint  GetUniformBlockDataSize(GLuint) const;

	bool Link();

	bool Use();

private:
	GLuint              program_handle_;
	std::vector<GLuint> shader_handles_;

	bool ready_to_use_ = false;

	mutable std::unordered_map<std::string, GLint> uniorm_locations_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
