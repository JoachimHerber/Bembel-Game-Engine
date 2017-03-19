#ifndef BEMBEL_BASE_CONVERSION_H_
#define BEMBEL_BASE_CONVERSION_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <sstream>
#include <string>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {
namespace conversion {

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
	if( ss.fail() || ss.bad() || ss.get(c) )
		return false;
	return true;
}

} //end of namespace conversion
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
