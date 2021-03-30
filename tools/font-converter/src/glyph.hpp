#ifndef BEMBEL_TOOLS_FONTCONVERTER_GLYPH_HPP
#define BEMBEL_TOOLS_FONTCONVERTER_GLYPH_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H

#include <bembel/bembel.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Glyph {
 public:
  Glyph();
  ~Glyph();

  using vec2 = glm::dvec2;

  void init(FT_Face& face, unsigned, unsigned);

  struct Line {
    vec2 start;
    vec2 end;
  };

  const std::vector<Line>& getOutline() const;
  const std::vector<glm::ivec2>& getEdges() const;

  const glm::ivec2& getExtendsMin() const;
  const glm::ivec2& getExtendsMax() const;

  const glm::ivec2& getSize() const;

  void setTexCoordMin(const glm::ivec2& value);
  void setTexCoordMax(const glm::ivec2& value);
  const glm::ivec2& getTexCoordMin() const;
  const glm::ivec2& getTexCoordMax() const;

  float getAdvance() const;

  unsigned getID() const;
  void setID(unsigned val);

 private:
  struct OutlinePoint {
    glm::ivec2 pos;
    bool isControlPoint;
    bool isSecondOrderBézier;
  };

  void readContour(int count, OutlinePoint* points);

  void addBézier(int count, OutlinePoint* points, int start, int end);

  void addBézier(const vec2&, const vec2&, const vec2&);
  void addBézier(const vec2&, const vec2&, const vec2&, const vec2&);

 private:
  std::vector<Line> outline;
  std::vector<glm::ivec2> edges;

  glm::ivec2 extends_min = glm::ivec2(0, 0);
  glm::ivec2 extends_max = glm::ivec2(0, 0);

  glm::ivec2 size = glm::ivec2(0, 0);

  glm::ivec2 tex_corrds_min = glm::ivec2(0, 0);
  glm::ivec2 tex_corrds_max = glm::ivec2(0, 0);

  unsigned advance = 0;

  unsigned id = 0;
};

#endif // include guards
