#ifndef BEMBEL_RENDERINGPIPELINE_H
#define BEMBEL_RENDERINGPIPELINE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <BembelBase/XML.h>
#include <BembelKernel/Scene/Scene.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Scene;
class Camera;
class Texture;
class Renderer;
class TextureView;
class GraphicSystem;
class RenderingStage;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API RenderingPipeline final
{
public:
	using ScenePtr = std::shared_ptr<Scene>;
	using CameraPtr = std::shared_ptr<Camera>;
	using TexturePtr = std::shared_ptr<Texture>;
	using RendererPtr = std::shared_ptr<Renderer>;
	using RenderingStagePtr = std::shared_ptr<RenderingStage>;
	using ViewPtr = std::shared_ptr<TextureView>;

	RenderingPipeline(GraphicSystem*);
	~RenderingPipeline();

	void SetResulution(const glm::ivec2& value);
	const glm::ivec2& GetResulution() const;

	void Enable();
	void Disable();
	bool IsEnabled();

	bool Configure(const xml::Element*);

	void Init();
	void Cleanup();

	void SetScene(ScenePtr);
	ScenePtr GetScene() const;

	CameraPtr GetCamera() const;

	TexturePtr GetTexture(const std::string& name) const;
	TexturePtr CreateTexture(const xml::Element*);
	TexturePtr CreateTexture(const std::string& name, GLenum format);

	void AddRenderingStage(RenderingStagePtr);

	void AddRenderer(RendererPtr);
	std::vector<RendererPtr>& GetRenderer();

	ViewPtr CreateView(const std::string& textureName);
	std::vector<ViewPtr>& GetViews();

	void Update();

private:
	void InitTextures(const xml::Element*);
	void InitStages(const xml::Element*);
	void InitRenderer(const xml::Element*);
	void InitViews(const xml::Element*);
	void InitCamera(const xml::Element*);

	GLenum StringToTextureFormat(const std::string&);

private:
	GraphicSystem* _grapicSys;

	glm::ivec2 _resolution;
	bool       _enabled;
	bool       _initalized;

	ScenePtr  _scene;
	CameraPtr _camera;

	std::map<std::string, TexturePtr> _textures;
	std::vector<RenderingStagePtr>    _stages;
	std::vector<RendererPtr>          _renderer;

	std::vector<ViewPtr> _views;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
