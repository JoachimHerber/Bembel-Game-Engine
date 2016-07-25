/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Conversion.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{
namespace conversion{


BEMBEL_API bool FromString(const std::string& str, glm::vec2& value)
{
	std::istringstream ss(str);
	char c;
	ss >> value.x >> c >> value.y;
	if (ss.fail() || ss.bad() || ss.get(c))
		return false;
	return true;
}

BEMBEL_API bool FromString(const std::string& str, glm::vec3& value)
{
	std::istringstream ss(str);
	char c;
	ss >> value.x >> c >> value.y >> c >> value.z;
	if (ss.fail() || ss.bad() || ss.get(c))
		return false;
	return true;
}

BEMBEL_API bool FromString(const std::string& str, glm::vec4& value)
{
	std::istringstream ss(str);
	char c;
	ss >> value.x >> c >> value.y >> c >> value.z >> c >> value.w;
	if (ss.fail() || ss.bad() || ss.get(c))
		return false;
	return true;
}

} //end of namespace conversion
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
