/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

#ifndef BEMBEL_BASE_XML_H_
#define BEMBEL_BASE_XML_H_

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <sstream>

#include <glm/glm.hpp>
#include <tinyxml2/tinyxml2.h>

/*============================================================================*/
/* DEFINITIONS                                                                */
/*============================================================================*/
namespace bembel {
namespace xml {

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
	if( !node )
		return false;

	std::stringstream sstream;
	sstream << value;
	node->SetAttribute(name.c_str(), sstream.str().c_str());
	return true;
}

template<typename T>
inline bool GetAttribute(const Element* node, const std::string& name, T& value)
{
	if( !node )
		return false;

	const char* attrib = node->Attribute(name.c_str());
	if( !attrib )
		return false;

	std::stringstream sstream;
	sstream << attrib;
	sstream >> value;
	return true;
}

template<typename T>
inline bool GetAttribute(
	const Element* node,
	const std::string& child_node,
	const std::string& name,
	T& value)
{
	if( !node )
		return false;

	return GetAttribute<T>(
		node->FirstChildElement(child_node.c_str()), name, value);
}

class ElementIterator
{
public:
	ElementIterator(const Element* element, const std::string& name)
		: element_(element)
		, name_(name)
	{}

	ElementIterator begin()
	{
		if( name_.empty() )
			return ElementIterator(
				element_->FirstChildElement(), name_);
		else
			return ElementIterator(
				element_->FirstChildElement(name_.c_str()), name_);

	}

	ElementIterator end()
	{
		return ElementIterator(nullptr, "");
	}

	const Element* operator*()
	{
		return element_;
	}
	bool operator != (const ElementIterator& other)
	{
		if( element_ != other.element_ )
			return true;
		return element_!=nullptr && name_ != other.name_;
	}
	ElementIterator& operator++()
	{
		if( name_.empty() )
			element_ = element_->NextSiblingElement();
		else
			element_ = element_->NextSiblingElement(name_.c_str());

		return *this;
	}
private:
	const Element* element_;
	const std::string name_;
};

inline ElementIterator IterateChildElements(
	const Element* element, const std::string& name = "")
{
	return ElementIterator(element, name);
}

} //end of namespace xml
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE		                                                          */
/*============================================================================*/
#endif // include guard
