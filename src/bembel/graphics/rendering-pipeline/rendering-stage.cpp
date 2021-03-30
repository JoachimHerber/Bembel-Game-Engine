#include "./rendering-stage.hpp"

#include "./rendering-pipeline.hpp"

namespace bembel::graphics {

RenderingStage::RenderingStage(RenderingPipeline& pipline)
: pipline{pipline}
, fbo{std::make_unique<kernel::FrameBufferObject>()} {
}

RenderingStage::~RenderingStage() {
}

void RenderingStage::setScene(kernel::Scene*) {
}

void RenderingStage::init() {
    this->fbo->init();
}

void RenderingStage::cleanup() {
    this->fbo->cleanup();
}

kernel::AssetManager& RenderingStage::getAssetManager() {
    return this->pipline.getAssetManager();
}

void RenderingStage::setInputTextures(const std::vector<std::string>& textures) {
    this->textures.clear();
    for(size_t n = 0; n < textures.size(); ++n) {
        this->textures.push_back(this->pipline.getTexture(textures[n]));
    }
}

void RenderingStage::setDepthOutputTexture(const std::string& texture) {
    this->fbo->setDepthAttechment(this->pipline.getTexture(texture));
}

void RenderingStage::setColorOutputTexture(unsigned index, const std::string& texture) {
    this->fbo->setColorAttechment(index, this->pipline.getTexture(texture));
}

void RenderingStage::bindInputTextures() {
    for(unsigned n = 0; n < this->textures.size(); ++n) {
        if(this->textures[n]) {
            glActiveTexture(GL_TEXTURE0 + n);
            this->textures[n]->bind();
        }
    }
}

void RenderingStage::releaseInputTextures() {
    for(unsigned n = unsigned(this->textures.size()); n > 0; --n) {
        if(this->textures[n - 1]) {
            glActiveTexture(GL_TEXTURE0 + n - 1);
            this->textures[n - 1]->release();
        }
    }
}

} // namespace bembel::graphics
