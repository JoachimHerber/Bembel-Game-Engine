#include "./glyph.hpp"

#include <algorithm>

using namespace bembel;

Glyph::vec2 toVec2(FT_Vector& point) {
  return Glyph::vec2(point.x, point.y);
}

Glyph::Glyph() {
}
Glyph::~Glyph() {
}

void Glyph::init(FT_Face& face, unsigned glyph_index, unsigned border) {
  auto error = FT_Load_Glyph(
    face,              /* handle to face object */
    glyph_index,       /* glyph index           */
    FT_LOAD_NO_SCALE); /* load flags, see below */
  if(error) return;

  FT_Outline& outline = face->glyph->outline;

  this->advance = face->glyph->metrics.horiAdvance;

  std::vector<OutlinePoint> points;
  for(int i = 0; i < outline.n_points; ++i) {
    points.push_back({glm::ivec2(outline.points[i].x, outline.points[i].y),
                      (0x0001 & outline.tags[i]) == 0,
                      (0x0002 & outline.tags[i]) == 0});
    if(0x0001 & outline.tags[i]) {
      this->edges.push_back(
        glm::ivec2(outline.points[i].x, outline.points[i].y));
    }
  }

  short start = 0;
  for(int i = 0; i < outline.n_contours; ++i) {
    short count = outline.contours[i] - start + 1;
    this->readContour(count, &(points[start]));
    start = outline.contours[i] + 1;
  }

  if(!points.empty()) {
    this->extends_min = this->extends_max = points[0].pos;
    for(size_t n = 1; n < points.size(); ++n) {
      this->extends_min = glm::min(this->extends_min, points[n].pos);
      this->extends_max = glm::max(this->extends_max, points[n].pos);
    }
  }
  //  if (face->glyph->metrics.horiBearingY != _extendsMax.y)
  //  {
  //    cout << "ERROR" << endl;
  //  }

  this->extends_min -= glm::ivec2(border);
  this->extends_max += glm::ivec2(border);

  this->size = this->extends_max - this->extends_min;
}

const std::vector<Glyph::Line>& Glyph::getOutline() const {
  return this->outline;
}

const std::vector<glm::ivec2>& Glyph::getEdges() const {
  return this->edges;
}

const glm::ivec2& Glyph::getExtendsMin() const {
  return this->extends_min;
}

const glm::ivec2& Glyph::getExtendsMax() const {
  return this->extends_max;
}

const glm::ivec2& Glyph::getSize() const {
  return this->size;
}

void Glyph::setTexCoordMin(const glm::ivec2& value) {
  this->tex_corrds_min = value;
}

void Glyph::setTexCoordMax(const glm::ivec2& value) {
  this->tex_corrds_max = value;
}

const glm::ivec2& Glyph::getTexCoordMin() const {
  return this->tex_corrds_min;
}

const glm::ivec2& Glyph::getTexCoordMax() const {
  return this->tex_corrds_max;
}

float Glyph::getAdvance() const {
  return this->advance;
}

unsigned Glyph::getID() const {
  return this->id;
}

void Glyph::setID(unsigned val) {
  this->id = val;
}

void Glyph::readContour(int count, OutlinePoint* points) {
  int start = 0;
  while(start < count && points[start].isControlPoint) ++start;

  int prev = start;
  for(int i = 1; i <= count; ++i) {
    int current         = (i + start);
    OutlinePoint* point = &points[current % count];
    if(point->isControlPoint) continue;
    if(prev + 1 == current) {
      this->outline.push_back({
        points[prev % count].pos,
        points[current % count].pos,
      });
    } else
      this->addBézier(count, points, prev, current);
    prev = current;
  }
}

void Glyph::addBézier(int count, OutlinePoint* points, int start, int end) {
  if(points[(start + 1) % count].isSecondOrderBézier) {
    glm::vec2 prev    = points[(start + 0) % count].pos;
    glm::vec2 current = points[(start + 1) % count].pos;
    for(int i = start + 2; i < end; ++i) {
      glm::vec2 next = points[(i) % count].pos;
      this->addBézier(prev, current, 0.5f * (current + next));
      prev    = 0.5f * (current + next);
      current = next;
    }
    this->addBézier(prev, current, points[end % count].pos);
  } else {
    if(start + 3 == end) {
      this->addBézier(
        points[(start + 0) % count].pos,
        points[(start + 1) % count].pos,
        points[(start + 2) % count].pos,
        points[(start + 3) % count].pos);
    }
  }
}
void Glyph::addBézier(const vec2& start, const vec2& control, const vec2& end) {
  vec2 prev = start;
  for(int i = 0; i < 16; ++i) {
    double f = (i + 1.0f) / (16.0f);
    double g = 1 - f;

    vec2 point;

    point.x = g * g * start.x + 2 * g * f * control.x + f * f * end.x;
    point.y = g * g * start.y + 2 * g * f * control.y + f * f * end.y;

    this->outline.push_back(Line{prev, point});

    prev = point;
  }
}
void Glyph::addBézier(
  const vec2& start,
  const vec2& control1,
  const vec2& control2,
  const vec2& end) {
  vec2 prev = start;
  for(int i = 0; i < 16; ++i) {
    double f = (i + 1.0f) / (16.0f);
    double g = 1 - f;

    vec2 point;
    point.x = 1 * g * g * g * start.x + 3 * g * g * f * control1.x
              + 3 * g * f * f * control2.x + 1 * f * f * f * end.x;
    point.y = 1 * g * g * g * start.y + 3 * g * g * f * control1.y
              + 3 * g * f * f * control2.y + 1 * f * f * f * end.y;
    this->outline.push_back(Line{prev, point});

    prev = point;
  }
}

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
