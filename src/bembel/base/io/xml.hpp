#ifndef BEMBEL_BASE_IO_XML_HPP
#define BEMBEL_BASE_IO_XML_HPP

#include <tinyxml2/tinyxml2.h>

#include <bembel/library.hpp>

namespace bembel::base::xml {

using Element  = tinyxml2::XMLElement;
using Document = tinyxml2::XMLDocument;

BEMBEL_API bool getAttribute(const Element* node, const std::string& name, std::string& value);
BEMBEL_API bool getAttribute(const Element* node, const std::string& name, bool& value);
BEMBEL_API bool getAttribute(const Element* node, const std::string& name, glm::vec2& value);
BEMBEL_API bool getAttribute(const Element* node, const std::string& name, glm::vec3& value);
BEMBEL_API bool getAttribute(const Element* node, const std::string& name, glm::vec4& value);

template <typename T>
bool setAttribute(Element* node, const std::string& name, const T& value);
template <typename T>
bool getAttribute(const Element* node, const std::string& name, T& value);
template <typename T>
bool getAttribute(const Element* node, const std::string& childNode, const std::string& name, T& value);

class ElementIterator {
  public:
    ElementIterator(const Element* element, const std::string& name)
    : element(element)
    , name(name) {}

    ElementIterator begin();

    ElementIterator end() { return ElementIterator(nullptr, ""); }

    const Element*   operator*() { return this->element; }
    bool             operator!=(const ElementIterator& other);
    ElementIterator& operator++();

  private:
    const Element*    element;
    const std::string name;
};

} // namespace bembel::base::xml
#include "xml.inl"
#endif // include guard
