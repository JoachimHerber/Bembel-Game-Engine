#ifndef BEMBEL_SELECTIONRENDERINGSTAGE_H
#define BEMBEL_SELECTIONRENDERINGSTAGE_H

#include <bembel/bembel.hpp>
#include <chrono>
#include <memory>
#include <vector>

#include "selection-component.h"

class SelectionRenderingStage : public bembel::graphics::RenderingStage {
  public:
    SelectionRenderingStage(bembel::graphics::RenderingPipeline& pipline);
    ~SelectionRenderingStage();

    void setShader(const bembel::kernel::Asset<bembel::kernel::ShaderProgram>&);
    void setShader(bembel::kernel::Asset<bembel::kernel::ShaderProgram>&&);

    void setDepthOutputTexture(const std::string&);
    void setColorOutputTexture(const std::string&);

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute() override;

    void setScene(bembel::kernel::Scene*) override;

    static std::unique_ptr<SelectionRenderingStage> createInstance(
        const bembel::base::xml::Element*, bembel::graphics::RenderingPipeline&);

  private:
    struct GeometryObject {
        glm::vec3                        position;
        glm::quat                        rotation;
        float                            dist;
        bembel::graphics::GeometryModel* model;
        unsigned                         state;
    };
    void getHiglightedObjects(std::vector<GeometryObject>&);

  private:
    bembel::kernel::Asset<bembel::kernel::ShaderProgram> shader_program;

    std::unique_ptr<bembel::kernel::Texture> noise;

    bembel::kernel::Scene*                              scene;
    bembel::kernel::PositionComponent::ContainerType*   position_components;
    bembel::kernel::RotationComponent::ContainerType*   rotation_components;
    bembel::graphics::GeometryComponent::ContainerType* geometry_components;
    SelectionComponent::ContainerType*                  selection_components;

    std::chrono::time_point<std::chrono::steady_clock> start_time;
};
#endif // include guards
