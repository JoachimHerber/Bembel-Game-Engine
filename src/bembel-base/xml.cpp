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
