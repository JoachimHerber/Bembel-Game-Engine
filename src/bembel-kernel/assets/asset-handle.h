#ifndef BEMBEL_KERNEL_ASSETS_ASSETHANDLE_H_
#define BEMBEL_KERNEL_ASSETS_ASSETHANDLE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <stdint.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

struct BEMBEL_API AssetHandle final
{
	union
	{
		struct
		{
			uint32_t index;
			uint16_t generation;
			uint16_t type_id;
		};
		uint64_t value = ~uint64_t(0U);
	};
};

inline bool operator < (const AssetHandle& first, const AssetHandle& second)
{
	return first.value < second.value;
}

} // end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
