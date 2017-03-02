#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGPIPELINE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGPIPELINE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include "../texture-view.h"

#include <memory>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/scene.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Scene;
class Camera;
class Texture;
class GeometryRendererBase;
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
	using RenderingStagePtr = std::shared_ptr<RenderingStage>;
	using ViewPtr = std::shared_ptr<TextureView>;

	RenderingPipeline(GraphicSystem*);
	~RenderingPipeline();

	GraphicSystem* GetGraphicSystem() const;

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
	
	template<typename StageType>
	StageType* AddRenderingStage();

	ViewPtr CreateView(const std::string& textureName);
	std::vector<ViewPtr>& GetViews();

	void Update();

private:
	void InitTextures(const xml::Element*);
	void InitStages(const xml::Element*);
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

	std::vector<ViewPtr> _views;
};

template<typename StageType>
inline StageType* RenderingPipeline::AddRenderingStage()
{
	_stages.push_back(std::make_unique<StageType>(this));
	return static_cast<StageType*>(_stages.back().get());
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
