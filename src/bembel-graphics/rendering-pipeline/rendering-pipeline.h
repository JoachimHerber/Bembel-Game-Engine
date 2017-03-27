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
namespace bembel {

class Scene;
class Camera;
class Texture;
class GeometryRendererBase;
class GraphicSystem;
class RenderingStage;
class AssetManager;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API RenderingPipeline final
{
public:;
	RenderingPipeline(GraphicSystem*);
	RenderingPipeline(const RenderingPipeline&) = delete;
	RenderingPipeline& operator=(const RenderingPipeline&) = delete;
	~RenderingPipeline();

	GraphicSystem* GetGraphicSystem() const;
	AssetManager*  GetAssetManager() const;

	void SetResulution(const glm::ivec2& value);
	const glm::ivec2& GetResulution() const;

	void Enable();
	void Disable();
	bool IsEnabled();

	bool Configure(const xml::Element*);

	void Init();
	void Cleanup();

	void SetScene(std::shared_ptr<Scene>);
	std::shared_ptr<Scene> GetScene() const;

	std::shared_ptr<Camera> GetCamera() const;

	Texture* GetTexture(const std::string& name) const;
	bool CreateTexture(const xml::Element*);
	bool CreateTexture(const std::string& name, GLenum format);

	template<typename StageType>
	StageType* AddRenderingStage();

	TextureView* CreateView(const std::string& texture_name);
	const std::vector<std::shared_ptr<TextureView>>& GetViews() const;

	void Update();

private:
	void InitTextures(const xml::Element*);
	void InitStages(const xml::Element*);
	void InitViews(const xml::Element*);
	void InitCamera(const xml::Element*);

	GLenum StringToTextureFormat(const std::string&);

private:
	GraphicSystem* grapic_system_;

	glm::ivec2 resolution_;
	bool       enabled_;
	bool       initalized_;

	std::shared_ptr<Scene>  scene_;
	std::shared_ptr<Camera> camera_;

	std::map<std::string, std::unique_ptr<Texture>> textures_;
	std::vector<std::unique_ptr<RenderingStage>>    render_stages_;

	std::vector<std::shared_ptr<TextureView>> views_;
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
