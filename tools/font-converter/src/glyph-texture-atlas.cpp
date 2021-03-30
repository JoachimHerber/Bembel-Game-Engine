
#include "glyph-texture-atlas.hpp"

GlyphTextureAtlas::GlyphTextureAtlas() {
}

GlyphTextureAtlas::~GlyphTextureAtlas() {
}

void GlyphTextureAtlas::update(std::vector<Glyph>& glypes) {
  std::vector<Glyph*> glyphs;

  double reqiredArea = 0;
  for(auto& glyph : glypes) {
    glyph.setTexCoordMin({0, 0});
    glyph.setTexCoordMax({0, 0});

    glm::ivec2 size = glyph.getSize();

    if(!glyph.getOutline().empty()) {
      reqiredArea += (size.x) * (size.y);
      glyphs.push_back(&glyph);
    } else {
      glyph.setTexCoordMin(glm::ivec2(0));
      glyph.setTexCoordMax(glm::ivec2(0));
    }
  }

  std::sort(glyphs.begin(), glyphs.end(), [](const Glyph* g1, const Glyph* g2) {
    return g1->getSize().x * g1->getSize().y
           > g2->getSize().x * g2->getSize().y;
  });

  this->scale  = .99 / sqrt(reqiredArea);
  bool success = false;
  while(!success) {
    // try to fill texture atlas
    this->root = std::make_unique<TextureAtlasNode>(
      this->resolution.x, this->resolution.y);
    success = this->fillAtlas(glyphs);

    if(!success) {
      // scale down glyphs until they fit
      this->scale *= .95;
    }
  }

  this->updateSignal.emit();
}

const TextureAtlasNode* GlyphTextureAtlas::getRoot() const {
  return this->root.get();
}

void GlyphTextureAtlas::setResolution(unsigned w, unsigned h) {
  this->resolution.x = w;
  this->resolution.y = h;
}

float GlyphTextureAtlas::getTexCoordScaleU() const {
  return 1.f / float(this->resolution.x);
}

float GlyphTextureAtlas::getTexCoordScaleV() const {
  return 1.f / float(this->resolution.y);
}

double GlyphTextureAtlas::getScaleFactor() const {
  return this->scale;
}

bembel::Signal<>& GlyphTextureAtlas::getUpdateSignal() {
  return this->updateSignal;
}

bool GlyphTextureAtlas::fillAtlas(const std::vector<Glyph*>& glypes) {
  for(auto& glyph : glypes) {
    glm::ivec2 size = glyph->getSize();
    double w, h;

    w = this->resolution.x * this->scale * (size.x);
    h = this->resolution.y * this->scale * (size.y);
    auto node =
      this->root->getNode(unsigned(floor(w) + 1), unsigned(floor(h) + 1));

    if(node) {
      glyph->setTexCoordMin(node->getPos());
      glyph->setTexCoordMax(node->getPos() + node->getSize());
      node->setGlyph(glyph);
    } else
      return false;
  }

  return true;
}
