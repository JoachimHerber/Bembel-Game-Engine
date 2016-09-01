#ifndef BEMBEL_RENDERINGTEST_H
#define BEMBEL_RENDERINGTEST_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <BembelKernel/Application.h>
#include <BembelKernel/Events/EventManager.h>
#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Scene/Scene.h>
#include <BembelGraphics/GraphicSystem.h>
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>

#include "CameraControle.h"
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class RenderingTest : public Application
{
public:
	RenderingTest();
	~RenderingTest();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void NextScene();
	void PrevScene();

	void HandleEvent(const WindowShouldCloseEvent&);
	void HandleEvent(const KeyPressEvent&);

private:
	void InitTestGeometry();
	void InitScene();

	std::shared_ptr<Scene> LoadScene(const std::string&);

private:
	std::shared_ptr<GraphicSystem>  _graphicSys;
	std::shared_ptr<CameraControle> _cam;

	std::shared_ptr<AssetManager> _assetMgr;

	int _currentScene;
	std::vector<std::shared_ptr<Scene>> _scenes;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
