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

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "xml.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {
namespace xml {

bool GetAttribute(const Element* node, const std::string& name, bool& value)
{
	if( !node )
		return false;

	return tinyxml2::XML_SUCCESS ==	node->QueryBoolAttribute(name.c_str(), &value);
}

bool GetAttribute(const Element* node, const std::string& name, glm::vec2& value)
{
	if( !node )
		return false;

	const char* attribute = node->Attribute(name.c_str());
	if( !attribute )
		return false;

	std::stringstream string_stream;
	string_stream << attribute;
	string_stream >> value.x;
	string_stream >> value.y;
	return true;
}

bool GetAttribute(const Element* node, const std::string& name, glm::vec3& value)
{
	if( !node )
		return false;

	const char* attribute = node->Attribute(name.c_str());
	if( !attribute )
		return false;

	std::stringstream string_stream;
	string_stream << attribute;
	string_stream >> value.x;
	string_stream >> value.y;
	string_stream >> value.z;
	return true;
}

bool GetAttribute(const Element* node, const std::string& name, glm::vec4& value)
{
	if( !node )
		return false;

	const char* attribute = node->Attribute(name.c_str());
	if( !attribute )
		return false;

	std::stringstream string_stream;
	string_stream << attribute;
	string_stream >> value.x;
	string_stream >> value.y;
	string_stream >> value.z;
	string_stream >> value.w;
	return true;
}

} //end of namespace xml
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
