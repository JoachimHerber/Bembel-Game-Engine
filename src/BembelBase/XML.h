#ifndef BEMBEL_XML_H
#define BEMBEL_XML_H

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <BembelConfig.h>

#include <tinyxml2/tinyxml2.h>

#include <glm/glm.hpp>

#include <sstream>

/*============================================================================*/
/* DEFINITIONS                                                                */
/*============================================================================*/
namespace bembel{
namespace xml{

using Element = tinyxml2::XMLElement;
using Document = tinyxml2::XMLDocument;

BEMBEL_API bool GetAttribute(const Element* node, const std::string& name, bool& value);
BEMBEL_API bool GetAttribute(const Element* node, const std::string& name, glm::vec2& value);
BEMBEL_API bool GetAttribute(const Element* node, const std::string& name, glm::vec3& value);
BEMBEL_API bool GetAttribute(const Element* node, const std::string& name, glm::vec4& value);

template<typename T>
bool SetAttribute(Element* node, const std::string& name, const T& value);
template<typename T>
bool GetAttribute(const Element* node, const std::string& name, T& value);
template<typename T>
bool GetAttribute(const Element* node, const std::string& childNode, const std::string& name, T& value);

template<typename T>
inline bool SetAttribute(Element* node, const std::string& name, const T& value)
{
	if (!node)
		return false;

	std::stringstream sstream;
	sstream << value;
	node->SetAttribute(name.c_str(), sstream.str().c_str());
	return true;
}

template<typename T>
inline bool GetAttribute(const Element* node, const std::string& name, T& value)
{
	if (!node)
		return false;

	const char* attrib = node->Attribute(name.c_str());
	if (!attrib)
		return false;

	std::stringstream sstream;
	sstream << attrib;
	sstream >> value;
	return true;
}

template<typename T>
inline bool GetAttribute(
	const Element* node, 
	const std::string& childNode, 
	const std::string& name, 
	T& value)
{
	if (!node)
		return false;

	return GetAttribute<T>(
		node->FirstChildElement(childNode.c_str()), name, value);
}

class ElementIterator
{
public:
	ElementIterator(const Element* element, const std::string& name)
		: _element(element)
		, _name(name)
	{}

	ElementIterator begin()
	{
		if (_name.empty())
			return ElementIterator(
				_element->FirstChildElement(), _name);
		else
			return ElementIterator(
				_element->FirstChildElement(_name.c_str()), _name);
		
	}

	ElementIterator end()
	{
		return ElementIterator(nullptr, "");
	}

	const Element* operator*()
	{
		return _element;
	}
	bool operator != (const ElementIterator& other)
	{
		if (_element != other._element)
			return true;
		return _element!=nullptr && _name != other._name;
	}
	ElementIterator& operator++()
	{
		if (_name.empty())
			_element = _element->NextSiblingElement();
		else
			_element = _element->NextSiblingElement(_name.c_str());

		return *this;
	}
private:
	const Element* _element;
	const std::string _name;
};

inline ElementIterator IterateChildElements(
	const Element* element, const std::string& name="")
{
	return ElementIterator(element, name);
}

} //end of namespace xml
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE		                                                          */
/*============================================================================*/
#endif // include guard
