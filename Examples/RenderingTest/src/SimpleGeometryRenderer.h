#ifndef TESTRENDERER_H
#define TESTRENDERER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelBase/XML.h>
#include <BembelGraphics/OpenGL/ShaderProgram.h>
#include <BembelGraphics/RenderingPipeline/Renderer.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Assets/AssetManager.h>
#include <BembelGraphics/OpenGL/Texture.h>
#include <BembelGraphics/Geometry/Material.h>

#include "SimpleGeometryComponent.h"

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class SimpleGeometryRenderer : public Renderer
{
public:
	SimpleGeometryRenderer();
	~SimpleGeometryRenderer();

	void Init() override;
	void Cleanup() override;

	virtual void SetScene(ScenePtr) override;

	void DoGeometryPass(
		const glm::mat4& view,
		const glm::mat4& projection,
		const ViewFrustum& frustum
	) override;
	void DoShadowPass(
		const glm::mat4& view, 
		const glm::mat4& projection,
		const ViewFrustum& frustum
	) override;

	static std::unique_ptr<SimpleGeometryRenderer> CreateInstance(const xml::Element*);

private:
	void InitGeometry();

	struct VertexData
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	void InitSpereGeometry(std::vector<VertexData>&, std::vector<unsigned>&);

	void InitXZPlaneGeometry(std::vector<VertexData>&, std::vector<unsigned>&);

	void InitVBO(const std::vector<VertexData>&);
	void InitIBO(const std::vector<unsigned>&);
	void InitVAO();

	void ApplyTexture(AssetHandle, unsigned, unsigned);

private:
	std::unique_ptr<ShaderProgram> _shader;

	ScenePtr                              _scene;
	SimpleGeometryComponent::ContainerPtr _geometryContainer;
	PositionComponent::ContainerPtr       _positionConteiner;

	TextureContainerPtr  _textureConteiner;
	MaterialContainerPtr _materialConteiner;

	GLuint _vbo;
	GLuint _ibo;
	GLuint _vao;

	std::vector<std::pair<GLsizei, GLsizei>> _shapes;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
