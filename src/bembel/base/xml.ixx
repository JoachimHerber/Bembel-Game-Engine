module;
#include <tinyxml2/tinyxml2.h>

#include <optional>
#include <string_view>
#include <type_traits>
export module bembel.base:Xml;

import :Types;
import :ObservableValue;
import :Conversion;

export namespace bembel::base::xml {
using Element  = tinyxml2::XMLElement;
using Document = tinyxml2::XMLDocument;

template <typename T>
    requires std::integral<T> || std::floating_point<T>
bool setAttribute(In<not_null_ptr<Element>> node, In<std::string_view> name, T value) {
    if(!node) return false;

    std::string name_str{name};
    node->SetAttribute(name_str.c_str(), value);
    return true;
}

template <typename T>
    requires(!std::is_arithmetic_v<std::remove_cv_t<T>>)
bool setAttribute(In<not_null_ptr<Element>> node, In<std::string_view> name, T const& value) {
    if(!node) return false;

    std::string name_str{name};
    if constexpr(IsObservableValue<T>::value) {
        node->SetAttribute(name_str.c_str(), conversion::toString(value.get()).c_str());
    } else if constexpr(std::is_same_v<std::remove_cv_t<T>, std::u8string>) {
        node->SetAttribute(
            name_str.c_str(), value.c_str()
        ); // TinyXML-2 assumes all inputs and outputs are UTF-8
    } else {
        node->SetAttribute(name_str.c_str(), conversion::toString(value).c_str());
    }
    return true;
}

template <typename T>
std::optional<T> getAttribute(In<not_null_ptr<const Element>> node, In<std::string_view> name) {
    if(!node) return {};

    std::string name_str{name};
    char const* attrib = node->Attribute(name_str.c_str());

    if(!attrib) return {};

    if constexpr(std::is_same_v<std::remove_cv_t<T>, std::u8string>) {
        // TinyXML-2 assumes all inputs and outputs are UTF-8
        return std::u8string((char8_t*)attrib);
    }
    if constexpr(std::is_same_v<std::remove_cv_t<T>, std::u8string_view>) {
        // TinyXML-2 assumes all inputs and outputs are UTF-8
        return std::u8string_view((char8_t*)attrib);
    }
    T value;
    if(conversion::fromString(attrib, value)) { return value; }
    return {};
}

template <typename T>
bool getAttribute(In<not_null_ptr<const Element>> node, In<std::string_view> name, T& value) {
    if(!node) return false;

    std::string name_str{name};
    char const* attrib = node->Attribute(name_str.c_str());

    if(!attrib) return false;

    if constexpr(IsObservableValue<T>::value) {
        typename T::Type tmp = value.get();
        if(!conversion::fromString(attrib, tmp)) return false;
        value.set(tmp);
        return true;
    } else if constexpr(std::is_same_v<std::remove_cv_t<T>, std::u8string>) {
        value = (char8_t*)attrib; // TinyXML-2 assumes all inputs and outputs are UTF-8
        return true;
    } else {
        return conversion::fromString(attrib, value);
    }
}

template <typename T>
bool getAttribute(
    In<not_null_ptr<const Element>> node,
    In<std::string_view>            child_node,
    In<std::string_view>            name,
    T&                              value
) {
    if(!node) return false;

    std::string child_node_str{child_node};
    return getAttribute<T>(node->FirstChildElement(child_node_str.c_str()), name, value);
}
template <typename T = Element>
class ElementIterator {
  public:
    ElementIterator(In<not_null_ptr<T>> element, In<std::string_view> name)
      : m_element(element), m_name(name) {}

    ElementIterator begin() {
        if(m_name.empty()) {
            return ElementIterator(m_element->FirstChildElement(), m_name);
        } else {
            return ElementIterator(m_element->FirstChildElement(m_name.c_str()), m_name);
        }
    }
    ElementIterator end() { return ElementIterator(nullptr, ""); }

    T const* operator*() { return m_element; }

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
    not_null_ptr<T>   m_element;
    const std::string m_name;
};

ElementIterator<Element> IterateChildElements(
    In<not_null_ptr<Element>> element, In<std::string_view> name = ""
) {
    return ElementIterator(element, name);
}

ElementIterator<const Element> IterateChildElements(
    In<not_null_ptr<const Element>> element, In<std::string_view> name = ""
) {
    return ElementIterator<const Element>(element, name);
}

} // namespace bembel::base::xml
