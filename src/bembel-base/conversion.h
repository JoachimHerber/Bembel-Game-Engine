#ifndef BEMBEL_CONVERSION_H
#define BEMBEL_CONVERSION_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>

#include <sstream>
#include <string>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{
namespace conversion{

BEMBEL_API bool FromString(const std::string&, glm::vec2&);
BEMBEL_API bool FromString(const std::string&, glm::vec3&);
BEMBEL_API bool FromString(const std::string&, glm::vec4&);

template<typename T>
bool FromString(const std::string&, T&);


template<typename T>
bool FromString(const std::string& str, T& value)
{
	std::istringstream ss(str);
	ss >> value;
	char c;
	if (ss.fail() || ss.bad() || ss.get(c))
		return false;
	return true;
}

} //end of namespace conversion
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
