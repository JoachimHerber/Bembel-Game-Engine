/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "XML.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{
namespace xml{

bool GetAttribute(const Element* node, const std::string& name, bool& value)
{
	if (!node)
		return false;

	return tinyxml2::XML_SUCCESS ==	node->QueryBoolAttribute(name.c_str(), &value);
}

bool GetAttribute(const Element* node, const std::string& name, glm::vec3& value)
{
	if (!node)
		return false;


	const char* attrib = node->Attribute(name.c_str());
	if (!attrib)
		return false;

	std::stringstream sstream;
	sstream << attrib;
	sstream >> value.x;
	sstream >> value.y;
	sstream >> value.z;
	return true;
}

bool GetAttribute(const Element* node, const std::string& name, glm::vec4& value)
{
	if (!node)
		return false;


	const char* attrib = node->Attribute(name.c_str());
	if (!attrib)
		return false;

	std::stringstream sstream;
	sstream << attrib;
	sstream >> value.x;
	sstream >> value.y;
	sstream >> value.z;
	sstream >> value.w;
	return true;
}

} //end of namespace xml
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
