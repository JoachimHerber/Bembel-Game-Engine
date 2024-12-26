module;
#include <glbinding/gl/gl.h>
module bembel.examples.gm_helper;

import std;
import bembel;
import :RenderingStages;
import :Initiative;

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
        Texture* texture = sprite.texture.get();
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

    Asset<Shader> token_vert =
        Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
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
       	gl_Position  = uModelViewProjectionMatrix * vec4( gOffsets[gl_VertexID] - vec2(0.5), 0, 1.0 );
      }
    )");
    Asset<Shader> token_frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
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

    m_token_shader = std::make_unique<ShaderProgram>();
    m_token_shader->attachShader(token_vert);
    m_token_shader->attachShader(token_frag);
    m_token_shader->link();

    Asset<Shader> area_vert =
        Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      uniform mat4 uModelViewProjectionMatrix;

      layout(location = 0) in vec4 aPosition;
      
      void main()
      {      	
       	gl_Position  = uModelViewProjectionMatrix * aPosition;
      }
    )");
    Asset<Shader> area_frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      #extension GL_ARB_shading_language_420pack: enable

      uniform vec4 uColor;
      
      layout(location = 0) out vec4 oColor;
       
      void main()
      {	
          oColor     = uColor;
      }
    )");

    m_area_shader = std::make_unique<ShaderProgram>();
    m_area_shader->attachShader(area_vert);
    m_area_shader->attachShader(area_frag);
    m_area_shader->link();
}

void TokenRenderingStage::cleanup() {
    m_fbo->cleanup();
    m_token_shader.release();
    m_area_shader.release();
}

void TokenRenderingStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_token_shader || !m_area_shader || !m_scene) return;

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

    drawTokens();
    drawAreaMarker();

    m_fbo->endRenderToTexture();
    glDisable(GL_ALPHA_TEST);
    glActiveTexture(GL_TEXTURE0);
}

void TokenRenderingStage::drawTokens() {
    m_token_shader->use();

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
        Texture* texture = token.texture.get();
        if(texture == nullptr) continue;
        if(!m_show_hidden_tokens && token.hidden) continue;

        mat4 mm = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        mm      = glm::translate(mm, transform->position);
        mm      = mm * mat4_cast(transform->rotation);
        mm      = glm::scale(mm, vec3(TOKEN_SIZE_VALUES[token.size]));

        m_token_shader->setUniform("uModelViewProjectionMatrix", view_proj * mm);
        m_token_shader->setUniform("uColor", token.color);

        bool selected = (entitis[entity] & selectable_mask) && selectables[entity].selected;

        m_token_shader->setUniform(
            "uHighlight", vec2(selected ? 0.5f : 1.0f, token.hidden ? 0.5f : 1.0f)
        );

        texture->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void TokenRenderingStage::drawAreaMarker() {
    m_area_shader->use();

    auto cam       = m_pipline.getCamera();
    mat4 view_proj = cam->getProjectionMatrix() * cam->getViewMatrix();

    auto const& entitis = m_scene->getEntitys();

    auto* transform_components   = m_scene->getComponentContainer<Transform>();
    auto* selectable_components  = m_scene->getComponentContainer<Selectable>();
    auto* area_marker_components = m_scene->getComponentContainer<AreaMarker>();

    ComponentMask mask =
        transform_components->getComponentMask() | area_marker_components->getComponentMask();

    ComponentMask selectable_mask = selectable_components->getComponentMask();

    auto const& area_markers = area_marker_components->getComponentData();
    auto const& selectables  = selectable_components->getComponentData();
    auto        transform    = transform_components->begin();
    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        auto const& area_marker = area_markers[entity];
        if(!m_show_hidden_tokens && area_marker.hidden) continue;

        mat4 mm = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        mm      = glm::translate(mm, transform->position);
        mm      = mm * mat4_cast(transform->rotation);
        mm      = glm::scale(mm, vec3(area_marker.size, area_marker.size, 1.f));

        bool selected = (entitis[entity] & selectable_mask) && selectables[entity].selected;

        m_area_shader->setUniform("uModelViewProjectionMatrix", view_proj * mm);
        m_area_shader->setUniform(
            "uColor",
            vec4(
                selected ? glm::mix(area_marker.color, vec3(1.f), 0.5f) : area_marker.color,
                area_marker.hidden ? 0.5f : 1.f
            )
        );

        glLineWidth(4.f);
        glBegin(GL_LINE_STRIP);
        switch(area_marker.shape) {
            case AreaMarker::Shape::SPHERE:
                for(float i = 0; i <= 360; i += 5) {
                    glVertex3f(std::sin(glm::radians(i)), std::cos(glm::radians(i)), 0.f);
                }
                break;
            case AreaMarker::Shape::BOX:
                glVertex3f(-0.5f, -0.5f, 0.f);
                glVertex3f(+0.5f, -0.5f, 0.f);
                glVertex3f(+0.5f, +0.5f, 0.f);
                glVertex3f(-0.5f, +0.5f, 0.f);
                glVertex3f(-0.5f, -0.5f, 0.f);
                break;
            case AreaMarker::Shape::CONE:
                glVertex3f(0.f, 0.f, 0.f);
                for(float i = 0; i <= 90; i += 5) {
                    glVertex3f(std::sin(glm::radians(i)), std::cos(glm::radians(i)), 0.f);
                }
                glVertex3f(0.f, 0.f, 0.f);
                break;
            case AreaMarker::Shape::LINE:
                glVertex3f(0.f, 0.f, 0.f);
                glVertex3f(1.f, 0.f, 0.f);
                break;
        }
        glEnd();
    }
}

bool CreateLightingOverlayStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "ColorOutput", "texture", texture_name))
        setColorOutputTexture(0, texture_name);

    return true;
}

void CreateLightingOverlayStage::init() {
    m_fbo->init();

    Asset<Shader> light_vert =
        Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
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
    Asset<Shader> light_frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
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

    m_light_shader = std::make_unique<ShaderProgram>();
    m_light_shader->attachShader(light_vert);
    m_light_shader->attachShader(light_frag);
    m_light_shader->link();

    Asset<Shader> shadow_vert =
        Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      uniform mat4 uModelViewProjectionMatrix;

      layout(location = 0) in vec4 aPosition;

      out float vShadow;

      void main() {
        vShadow     = aPosition.w;
        gl_Position = uModelViewProjectionMatrix * vec4(aPosition.xyz, 1.0);
      }
    )");
    Asset<Shader> shadow_frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      in float vShadow;

      layout(location = 0) out vec4 oColor;

      void main()
      {	
 	      oColor = vec4(smoothstep(0.0, 1.0, vShadow));
      }
    )");

    m_shadow_shader = std::make_unique<ShaderProgram>();
    m_shadow_shader->attachShader(shadow_vert);
    m_shadow_shader->attachShader(shadow_frag);
    m_shadow_shader->link();
}

void CreateLightingOverlayStage::cleanup() {
    m_fbo->cleanup();
    m_light_shader.release();
    m_shadow_shader.release();
}

void CreateLightingOverlayStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_light_shader || !m_shadow_shader || !m_scene) return;

    m_fbo->beginRenderToTexture();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_MAX);

    drawLights();

    glBlendEquation(GL_MIN);

    drawShadow();

    m_fbo->endRenderToTexture();
    glDisable(GL_ALPHA_TEST);
}

void CreateLightingOverlayStage::drawLights() {
    m_light_shader->use();

    auto cam  = m_pipline.getCamera();
    mat4 proj = cam->getProjectionMatrix();
    mat4 view = cam->getViewMatrix();

    m_light_shader->setUniform("uViewProjectionMatrix", proj * view);
    auto const& entitis = m_scene->getEntitys();

    auto* transform_components = m_scene->getComponentContainer<Transform>();
    auto* light_components     = m_scene->getComponentContainer<Light>();

    ComponentMask mask =
        transform_components->getComponentMask() | light_components->getComponentMask();

    auto const& light_data = light_components->getComponentData();
    auto        transform  = transform_components->begin();
    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        m_light_shader->setUniform("uPosition", transform->position);
        m_light_shader->setUniform("uRange", light_data[entity].range);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void CreateLightingOverlayStage::drawShadow() {
    m_shadow_shader->use();

    auto cam       = m_pipline.getCamera();
    mat4 view_proj = cam->getProjectionMatrix() * cam->getViewMatrix();

    auto const& entitis = m_scene->getEntitys();

    auto* transform_components = m_scene->getComponentContainer<Transform>();
    auto* marker_components    = m_scene->getComponentContainer<AreaMarker>();

    ComponentMask mask =
        transform_components->getComponentMask() | marker_components->getComponentMask();

    auto const& marker_data = marker_components->getComponentData();
    auto        transform   = transform_components->begin();
    for(usize entity = 0; entity < entitis.size(); ++entity, ++transform) {
        if((entitis[entity] & mask) != mask) continue;

        auto const& area_marker = marker_data[entity];
        if(!area_marker.block_light) continue;

        mat4 mm = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        mm      = glm::translate(mm, transform->position);
        mm      = mm * mat4_cast(transform->rotation);
        mm      = glm::scale(mm, vec3(area_marker.size, area_marker.size, 1.f));

        float s = 0.5f / area_marker.size;

        m_shadow_shader->setUniform("uModelViewProjectionMatrix", view_proj * mm);

        switch(area_marker.shape) {
            case AreaMarker::Shape::SPHERE:
                glBegin(GL_TRIANGLE_FAN);
                glVertex4f(0.f, 0.f, 0.f, 0.0f);
                for(float i = 0; i <= 360; i += 5) {
                    glVertex4f(std::cos(glm::radians(i)), std::sin(glm::radians(i)), 0.f, 0.f);
                }
                glEnd();
                glBegin(GL_TRIANGLE_STRIP);
                for(float i = 0; i <= 360; i += 5) {
                    float sin = std::sin(glm::radians(i));
                    float cos = std::cos(glm::radians(i));
                    glVertex4f((1 + s) * sin, (1 + s) * cos, 0.f, 1.f);
                    glVertex4f(sin, cos, 0.f, 0.f);
                }
                glEnd();
                break;
            case AreaMarker::Shape::BOX:
                glBegin(GL_TRIANGLE_STRIP);
                glVertex4f(-0.5f, +0.5f, 0.f, 0.f);
                glVertex4f(-0.5f, -0.5f, 0.f, 0.f);
                glVertex4f(+0.5f, +0.5f, 0.f, 0.f);
                glVertex4f(+0.5f, -0.5f, 0.f, 0.f);
                glEnd();
                glBegin(GL_TRIANGLE_STRIP);
                glVertex4f(-0.5f - s, +0.5f + s, 0.f, 1.f);
                glVertex4f(-0.5f, +0.5f, 0.f, 0.f);
                glVertex4f(+0.5f + s, +0.5f + s, 0.f, 1.f);
                glVertex4f(+0.5f, +0.5f, 0.f, 0.f);
                glVertex4f(+0.5f + s, -0.5f - s, 0.f, 1.f);
                glVertex4f(+0.5f, -0.5f, 0.f, 0.f);
                glVertex4f(-0.5f - s, -0.5f - s, 0.f, 1.f);
                glVertex4f(-0.5f, -0.5f, 0.f, 0.f);
                glVertex4f(-0.5f - s, +0.5f + s, 0.f, 1.f);
                glVertex4f(-0.5f, +0.5f, 0.f, 0.f);
                glEnd();
                break;
        }
    }
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

bool InitiativeOverlayStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "Output", "texture", texture_name))
        setColorOutputTexture(0, texture_name);

    m_overlay.request(properties->FirstChildElement("OverlayTexture"));
    m_mask.request(properties->FirstChildElement("MaskTexture"));

    xml::getAttribute(properties, "size", m_size);
    xml::getAttribute(properties, "margin", m_margin);
    return true;
}

void InitiativeOverlayStage::init() {
    m_fbo->init();

    Asset<Shader> vert = Shader::createShader(Shader::Type::VERTEX, R"(#version 330 compatibility
      uniform vec2 uPosition;
      uniform vec2 uSize;
      
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
       	gl_Position  = vec4(2*(uPosition + uSize*gOffsets[gl_VertexID])-vec2(1.0), 0, 1.0);
      }
    )");
    Asset<Shader> frag = Shader::createShader(Shader::Type::FRAGMENT, R"(#version 330
      #extension GL_ARB_shading_language_420pack: enable

      in vec2 vTexCoors;

      uniform vec4 uColor;
      
      layout(binding=0) uniform sampler2D uOverlayTexture;
      layout(binding=1) uniform sampler2D uMaskTexture;
      layout(binding=2) uniform sampler2D uTokenTexture;
      
      layout(location = 0) out vec4 oColor;
       
      void main()
      {	
          vec4 overlay = texture2D(uOverlayTexture, vTexCoors) * uColor;
       	  vec4 mask    = texture2D(uMaskTexture,    vTexCoors);
          vec4 token   = texture2D(uTokenTexture,   vTexCoors);

       	  oColor = mix(token*mask, overlay, overlay.a);
      }
    )");

    m_shader_program = std::make_unique<ShaderProgram>();
    m_shader_program->attachShader(vert);
    m_shader_program->attachShader(frag);
    m_shader_program->link();
}

void InitiativeOverlayStage::cleanup() {
    m_fbo->cleanup();
    m_shader_program.release();
}

void InitiativeOverlayStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_shader_program || !m_scene) return;

    m_fbo->beginRenderToTexture();

    glDepthFunc(GL_DEPTH_TEST);
    glDepthFunc(GL_CULL_FACE);
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

    vec2  resulution = m_pipline.getResulution();
    float pos_x      = m_margin.x;

    for(auto& entity : getInitiativeOrder(m_scene)) {
        auto*    token      = entity.get<Token>();
        auto*    initiative = entity.get<Initiative>();
        Texture* texture    = token->texture.get();
        if(texture == nullptr) continue;
        if(token->hidden) continue;

        vec2 size = initiative->active ? m_size_active : m_size;
        vec2 pos  = vec2(pos_x, resulution.y - m_margin.y - size.y);

        m_shader_program->setUniform("uPosition", pos / resulution);
        m_shader_program->setUniform("uSize", size / resulution);
        pos_x += (size.x + m_margin.z);
        m_shader_program->setUniform("uColor", token->color);

        texture->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    m_fbo->endRenderToTexture();
    glDisable(GL_ALPHA_TEST);
    glActiveTexture(GL_TEXTURE0);
}

} // namespace bembel::examples::gm_helper
