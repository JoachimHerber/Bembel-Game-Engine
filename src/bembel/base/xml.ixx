module;
#include <tinyxml2/tinyxml2.h>

#include <string_view>
export module bembel.base:Xml;

import :Types;
import :ObservableValue;
import :Conversion;
import :Unicode;

export namespace bembel::base::xml {
using Element  = tinyxml2::XMLElement;
using Document = tinyxml2::XMLDocument;

template <typename T>
    requires std::integral<T> || std::floating_point<T> bool
setAttribute(In<Element*> node, In<std::string_view> name, T value) {
    if(!node) return false;

    std::string name_str{name};
    node->SetAttribute(name_str.c_str(), value);
    return true;
}

template <typename T>
bool setAttribute(In<Element*> node, In<std::string_view> name, const T& value) {
    if(!node) return false;

    std::string name_str{name};
    node->SetAttribute(name_str.c_str(), conversion::toString(value).c_str());
    return true;
}
template <typename T>
bool getAttribute(In<Element const*> node, In<std::string_view> name, T& value) {
    if(!node) return false;

    std::string name_str{name};
    char const* attrib = node->Attribute(name_str.c_str());
    if(!attrib) return false;

    return conversion::fromString(attrib, value);
}

template <typename T>
bool setAttribute(In<Element*> node, In<std::string_view> name, ObservableValue<T> const& value) {
    if(!node) return false;

    std::string name_str{name};
    node->SetAttribute(name_str.c_str(), conversion::toString(value.get()).c_str());
    return true;
}

template <typename T>
bool getAttribute(In<Element const*> node, In<std::string_view> name, ObservableValue<T>& value) {
    if(!node) return false;

    std::string name_str{name};
    char const* attrib = node->Attribute(name_str.c_str());
    if(!attrib) return false;

    T tmp = value.get();
    if(!conversion::fromString(attrib, tmp)) return false;
    value.set(tmp);
    return true;
}

template <typename T>
bool getAttribute(
    In<Element const*> node, In<std::string_view> child_node, In<std::string_view> name, T& value
) {
    if(!node) return false;

    std::string child_node_str{child_node};
    return getAttribute<T>(node->FirstChildElement(child_node_str.c_str()), name, value);
}

class ElementIterator {
  public:
    ElementIterator(In<Element const*> element, In<std::string_view> name)
      : m_element(element), m_name(name) {}

    ElementIterator begin() {
        if(m_name.empty()) {
            return ElementIterator(m_element->FirstChildElement(), m_name);
        } else {
            return ElementIterator(m_element->FirstChildElement(m_name.c_str()), m_name);
        }
    }
    ElementIterator end() { return ElementIterator(nullptr, ""); }

    Element const* operator*() { return m_element; }

    bool operator!=(ElementIterator const& other) {
        if(m_element != other.m_element) return true;
        return m_element != nullptr && m_name != other.m_name;
    }
    ElementIterator& operator++() {
        if(m_name.empty())
            m_element = m_element->NextSiblingElement();
        else
            m_element = m_element->NextSiblingElement(m_name.c_str());

        return *this;
    }

  private:
    Element const*    m_element;
    const std::string m_name;
};

ElementIterator IterateChildElements(In<Element const*> element, In<std::string_view> name = "") {
    return ElementIterator(element, name);
}

} // namespace bembel::base::xml
