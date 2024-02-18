module;
#include <glbinding/gl/gl.h>

#include <chrono>
#include <random>
module bembel.examples.gm_helper;

import bembel;
import :RenderingStages;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;
using namespace ::gl;

bool SpriteRenderingStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "ColorOutput", "texture", texture_name))
        setColorOutputTexture(0, texture_name);
    if(xml::getAttribute(properties, "DepthOutput", "texture", texture_name))
        setDepthOutputTexture(texture_name);
    return true;
}

void SpriteRenderingStage::init() {
    m_fbo->init();

    Asset<Shader> vert = Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      uniform mat4 uModelViewProjectionMatrix;
      
      out vec2 vTexCoors;
       
      const vec2 gOffsets[4] = vec2[](
      	vec2( 0.0, 0.0 ),
      	vec2( 1.0, 0.0 ),
      	vec2( 0.0, 1.0 ),
      	vec2( 1.0, 1.0 )
      );
      
      void main()
      {      	
      	vTexCoors    = gOffsets[gl_VertexID];
       	gl_Position  = uModelViewProjectionMatrix * vec4( gOffsets[gl_VertexID] - vec2(0.5), 0, 1.0f );
      }
    )");
    Asset<Shader> frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      in vec2 vTexCoors;
      
      uniform sampler2D uTexture;
      
      layout(location = 0) out vec4 oColor;
       
      void main()
      {	
       	  oColor = texture2D(uTexture, vTexCoors);
      }
    )");

    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->attachShader(vert);
    m_shader_program->attachShader(frag);
    m_shader_program->link();
}

void SpriteRenderingStage::cleanup() {
    m_fbo->cleanup();
    m_shader_program.release();
}

void SpriteRenderingStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_shader_program || !m_scene) return;

    m_fbo->beginRenderToTexture();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.f);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

    m_shader_program->use();

    auto cam       = m_pipline.getCamera();
    mat4 view_proj = cam->getProjectionMatrix() * cam->getViewMatrix();

    auto const& entitis = m_scene->getEntitys();

    auto* transform_components  = m_scene->getComponentContainer<Transform>();
    auto* sprite_components     = m_scene->getComponentContainer<Sprite>();
    auto* selectable_components = m_scene->getComponentContainer<Selectable>();

    ComponentMask mask =
        transform_components->getComponentMask() | sprite_components->getComponentMask();

    ComponentMask selectable_mask = selectable_components->getComponentMask();

    auto const& sprites     = sprite_components->getComponentData();
    auto const& selectables = selectable_components->getComponentData();
    auto        transform   = transform_components->begin();
    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        auto&    sprite  = sprites[entity];
        Texture* texture = sprite.textrue.get();
        if(texture == nullptr) continue;

        mat4 mm = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        mm      = glm::translate(mm, transform->position);
        mm      = glm::rotate(mm, glm::radians(sprite.rotation), vec3(0.f, 0.f, 1.f));
        mm      = glm::scale(mm, vec3(sprite.size, 0));

        m_shader_program->setUniform("uModelViewProjectionMatrix", view_proj * mm);

        bool selected = (entitis[entity] & selectable_mask) && selectables[entity].selected;

        texture->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    m_fbo->endRenderToTexture();
    glDisable(GL_ALPHA_TEST);
}

bool TokenRenderingStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "ColorOutput", "texture", texture_name))
        setColorOutputTexture(0, texture_name);
    if(xml::getAttribute(properties, "DepthOutput", "texture", texture_name))
        setDepthOutputTexture(texture_name);

    m_overlay.request(properties->FirstChildElement("OverlayTexture"));
    m_mask.request(properties->FirstChildElement("MaskTexture"));

    xml::getAttribute(properties, "show_hidden_tokens", m_show_hidden_tokens);

    return true;
}

void TokenRenderingStage::init() {
    m_fbo->init();

    Asset<Shader> vert = Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      uniform mat4 uModelViewProjectionMatrix;
      
      out vec2 vTexCoors;
       
      const vec2 gOffsets[4] = vec2[](
      	vec2( 0.0, 0.0 ),
      	vec2( 1.0, 0.0 ),
      	vec2( 0.0, 1.0 ),
      	vec2( 1.0, 1.0 )
      );
      
      void main()
      {      	
      	vTexCoors    = gOffsets[gl_VertexID];
       	gl_Position  = uModelViewProjectionMatrix * vec4( gOffsets[gl_VertexID] - vec2(0.5), 0, 1.0f );
      }
    )");
    Asset<Shader> frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      #extension GL_ARB_shading_language_420pack: enable

      in vec2 vTexCoors;

      uniform vec2 uHighlight;
      uniform vec4 uColor;
      
      layout(binding=0) uniform sampler2D uOverlayTexture;
      layout(binding=1) uniform sampler2D uMaskTexture;
      layout(binding=2) uniform sampler2D uTokenTexture;
      
      layout(location = 0) out vec4 oColor;
       
      void main()
      {	
          vec4 overlay = texture2D(uOverlayTexture, vTexCoors) * uColor;
       	  vec4 token   = texture2D(uMaskTexture, vTexCoors) * texture2D(uTokenTexture, vTexCoors);

          overlay.rgb  = mix(vec3(1.0), overlay.rgb, uHighlight.x);
       	  oColor.rgb   = mix(token.rgb, overlay.rgb, overlay.a);
          oColor.a     = max(overlay.a, token.a)*uHighlight.y;
      }
    )");

    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->attachShader(vert);
    m_shader_program->attachShader(frag);
    m_shader_program->link();
}

void TokenRenderingStage::cleanup() {
    m_fbo->cleanup();
    m_shader_program.release();
}

void TokenRenderingStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_shader_program || !m_scene) return;

    m_fbo->beginRenderToTexture();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.f);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    m_overlay->bind();
    glActiveTexture(GL_TEXTURE1);
    m_mask->bind();
    glActiveTexture(GL_TEXTURE2);

    m_shader_program->use();

    auto cam       = m_pipline.getCamera();
    mat4 view_proj = cam->getProjectionMatrix() * cam->getViewMatrix();

    auto const& entitis = m_scene->getEntitys();

    auto* transform_components  = m_scene->getComponentContainer<Transform>();
    auto* selectable_components = m_scene->getComponentContainer<Selectable>();
    auto* token_components      = m_scene->getComponentContainer<Token>();

    ComponentMask mask =
        transform_components->getComponentMask() | token_components->getComponentMask();

    ComponentMask selectable_mask = selectable_components->getComponentMask();

    auto const& tokens      = token_components->getComponentData();
    auto const& selectables = selectable_components->getComponentData();
    auto        transform   = transform_components->begin();
    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        auto&    token   = tokens[entity];
        Texture* texture = token.textrue.get();
        if(texture == nullptr) continue;
        if(!m_show_hidden_tokens && token.hidden) continue;

        mat4 mm = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        mm      = glm::translate(mm, transform->position);
        mm      = glm::scale(mm, vec3(TOKEN_SIZE_VALUES[token.size]));

        m_shader_program->setUniform("uModelViewProjectionMatrix", view_proj * mm);
        switch(token.type) {
            case Token::Type::Player:
                m_shader_program->setUniform("uColor", vec4(1.0f, 0.8f, 0.0f, 1.0f));
                break;
            case Token::Type::Monster:
                m_shader_program->setUniform("uColor", vec4(1.0f, 0.0f, 0.0f, 1.0f));
                break;
            case Token::Type::NPC:
                m_shader_program->setUniform("uColor", vec4(0.0f, 0.5f, 1.0f, 1.0f));
                break;
        }

        bool selected = (entitis[entity] & selectable_mask) && selectables[entity].selected;

        m_shader_program->setUniform(
            "uHighlight", vec2(selected ? 0.5f : 1.0f, token.hidden ? 0.5f : 1.0f)
        );

        texture->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    m_fbo->endRenderToTexture();
    glDisable(GL_ALPHA_TEST);
    glActiveTexture(GL_TEXTURE0);
}

bool LightingOverlayStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "ColorOutput", "texture", texture_name))
        setColorOutputTexture(0, texture_name);

    return true;
}

void LightingOverlayStage::init() {
    m_fbo->init();

    Asset<Shader> vert = Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      uniform mat4 uViewProjectionMatrix;
      uniform vec3 uPosition;
      uniform vec2 uRange;

      out vec2 vTexCoords;
      out vec2 vRange;

      const vec2 gVertices[4] = vec2[](
	      vec2( -1.0, -1.0 ),
	      vec2( +1.0, -1.0 ),
	      vec2( -1.0, +1.0 ),
	      vec2( +1.0, +1.0 )
      );

      void main() {
        vRange      = uRange;
        vTexCoords  = gVertices[gl_VertexID] * (uRange.y + 1.0);
        gl_Position = uViewProjectionMatrix*vec4(uPosition.xy + vTexCoords, 0, 1);
      }
    )");
    Asset<Shader> frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      in vec2 vTexCoords;
      in vec2 vRange;
      
      layout(location = 0) out vec4 oColor;

      void main()
      {	
        float dist = length(vTexCoords);
 	      oColor.rgb = vec3(0.75*smoothstep(vRange.x+0.5, vRange.x-0.5, dist) + 0.25*smoothstep(vRange.y+0.5, vRange.y-0.5, dist));
 	      oColor.a   = 1.0;
      }
    )");

    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->attachShader(vert);
    m_shader_program->attachShader(frag);
    m_shader_program->link();
}

void LightingOverlayStage::cleanup() {
    m_fbo->cleanup();
    m_shader_program.release();
}

void LightingOverlayStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_shader_program || !m_scene) return;

    m_fbo->beginRenderToTexture();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_MAX);

    m_shader_program->use();

    auto cam  = m_pipline.getCamera();
    mat4 proj = cam->getProjectionMatrix();
    mat4 view = cam->getViewMatrix();

    m_shader_program->setUniform("uViewProjectionMatrix", proj * view);
    auto const& entitis = m_scene->getEntitys();

    auto* transform_components = m_scene->getComponentContainer<Transform>();
    auto* light_components     = m_scene->getComponentContainer<Light>();

    ComponentMask mask =
        transform_components->getComponentMask() | light_components->getComponentMask();

    auto const& light_data = light_components->getComponentData();
    auto        transform  = transform_components->begin();
    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        m_shader_program->setUniform("uPosition", transform->position);
        m_shader_program->setUniform("uRange", light_data[entity].range);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    m_fbo->endRenderToTexture();
    glDisable(GL_ALPHA_TEST);
}

bool ApplyLightingOverlayStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "Output", "texture", texture_name))
        setColorOutputTexture(0, texture_name);

    std::vector<std::string> intput_textures;
    if(xml::getAttribute(properties, "Input", "texture", texture_name))
        intput_textures.push_back(texture_name);

    xml::getAttribute(properties, "intensity", m_intensity);

    setInputTextures(intput_textures);
    return true;
}

void ApplyLightingOverlayStage::init() {
    m_fbo->init();

    Asset<Shader> vert = Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      out vec2 vTexCoords;

      const vec2 gCoords[4] = vec2[](
	      vec2( 0.0, 0.0 ),
	      vec2( 1.0, 0.0 ),
	      vec2( 0.0, 1.0 ),
	      vec2( 1.0, 1.0 )
      );

      void main() {
        gl_Position = vec4(2.0*gCoords[gl_VertexID] - vec2(1.0), 0, 1);
        vTexCoords  = gCoords[gl_VertexID];
      }
    )");
    Asset<Shader> frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      in vec2 vTexCoords;
      
      uniform sampler2D uTexture;

      uniform float uIntensity;

      layout(location = 0) out vec4 oColor;

      void main()
      {	
        oColor.rgb = vec3(0.0);
 	      oColor.a   = uIntensity*(1.0 - texture2D(uTexture, vTexCoords).r);
      }
    )");

    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->attachShader(vert);
    m_shader_program->attachShader(frag);
    m_shader_program->link();
}

void ApplyLightingOverlayStage::cleanup() {
    m_fbo->cleanup();
    m_shader_program.release();
}

void ApplyLightingOverlayStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_shader_program) return;

    m_fbo->beginRenderToTexture();
    bindInputTextures();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader_program->use();
    m_shader_program->setUniform("uIntensity", m_intensity);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_fbo->endRenderToTexture();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

} // namespace bembel::examples::gm_helper
