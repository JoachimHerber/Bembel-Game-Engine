module;
#include <memory>
#include <string_view>
export module bembel.graphics.pipeline:RenderingPipeline;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import :Camera;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class RenderingPipeline final {
  public:
    class Stage {
      public:
        Stage(RenderingPipeline& pipline) : m_pipline{pipline} {}
        virtual ~Stage() = default;

        virtual bool configure(xml::Element const*) = 0;

        virtual void setScene(Scene*) {}

        virtual void init() { m_fbo->init(); }
        virtual void cleanup() { m_fbo->cleanup(); }

        using RendererPtr = std::shared_ptr<GeometryRendererBase>;
        virtual void execute(
            GeometryRenderQueue& renderQueue, std::vector<RendererPtr> const& renderer
        ) = 0;

        using RendertingStageFactory = Factory<Stage, RenderingPipeline&>;
        static RendertingStageFactory& getFactory();

        static std::unique_ptr<Stage> create(std::string_view type, RenderingPipeline& pipeline) {
            return getFactory().createObject(type, pipeline);
        }

        template <typename T>
        static bool registerStageType(std::string_view type_name) {
            return getFactory().registerObjectGenerator<T>(type_name);
        }

      protected:
        void setInputTextures(std::vector<std::string> const&);

        void setDepthOutputTexture(std::string_view texture);
        void setColorOutputTexture(uint index, std::string_view texture);

        void bindInputTextures();
        void releaseInputTextures();

      protected:
        RenderingPipeline& m_pipline;

        std::vector<Texture*>              m_textures;
        std::unique_ptr<FrameBufferObject> m_fbo = std::make_unique<FrameBufferObject>();
    };

    class View : public Viewport::View {
      public:
        View(Texture* texture) : m_texture{texture} { m_fbo->setColorAttechment(0, texture); }
        ~View() = default;

        void setRelativeViewArea(In<vec2> pos, In<vec2> size) {
            m_relative_view_area_pos  = pos;
            m_relative_view_area_size = size;
        }
        void updateViewArea(In<ivec2> resolution) {
            m_view_area_pos  = vec2(resolution) * m_relative_view_area_pos;
            m_view_area_size = vec2(resolution) * m_relative_view_area_size;
        }
        ivec2 getViewAreaPosition() const { return m_view_area_pos; }
        uvec2 getViewAreaSize() const { return m_view_area_size; }

        void init() { m_fbo->init(); }
        void cleanup() { m_fbo->cleanup(); }

        void draw(ivec2 const& viewport_position, uvec2 const& viewport_size) override;

      private:
        Texture* m_texture;

        ivec2 m_view_area_pos  = {0, 0};
        uvec2 m_view_area_size = {1, 1};

        vec2 m_relative_view_area_pos;
        vec2 m_relative_view_area_size;

        std::unique_ptr<FrameBufferObject> m_fbo = std::make_unique<FrameBufferObject>();
    };

  public:
    RenderingPipeline(DisplayManager& display_mgr) : m_display_mgr{display_mgr} {}
    RenderingPipeline(RenderingPipeline const&)            = delete;
    RenderingPipeline& operator=(RenderingPipeline const&) = delete;
    ~RenderingPipeline()                                   = default;

    void         setResulution(ivec2 const& value);
    ivec2 const& getResulution() const { return m_resolution; }

    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }
    bool isEnabled() { return m_enabled; }

    bool configure(xml::Element const*);

    void init();
    void cleanup();

    void                   setScene(std::shared_ptr<Scene>);
    std::shared_ptr<Scene> getScene() const { return m_scene; }

    std::shared_ptr<Camera> getCamera() const { return m_camera; }

    kernel::Texture* getTexture(std::string_view name) const;
    bool             createTexture(xml::Element const*);
    bool             createTexture(std::string_view name, Texture::Format format);

    template <typename StageType>
    StageType* addRenderingStage() {
        m_render_stages.push_back(std::make_unique<StageType>(this));
        return static_cast<StageType*>(m_render_stages.back().get());
    }

    View*                                     createView(std::string_view texture_name);
    std::vector<std::unique_ptr<View>> const& getViews() const { return m_views; }

    using RendererPtr = std::shared_ptr<GeometryRendererBase>;
    void update(GeometryRenderQueue& renderQueue, std::vector<RendererPtr> const& renderer);

  private:
    void configureTextures(xml::Element const*);
    void configureStages(xml::Element const*);
    void configureViews(xml::Element const*);
    void configureCamera(xml::Element const*);

  private:
    DisplayManager& m_display_mgr;

    glm::ivec2 m_resolution = {800, 600};
    bool       m_enabled    = true;
    bool       m_initalized = false;

    std::shared_ptr<Scene>  m_scene  = nullptr;
    std::shared_ptr<Camera> m_camera = std::make_shared<Camera>();

    Dictionary<std::unique_ptr<Texture>> m_textures;
    std::vector<std::unique_ptr<Stage>>  m_render_stages;
    std::vector<std::unique_ptr<View>>   m_views;
};

} // namespace bembel::graphics
