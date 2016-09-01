#ifndef BEMBEL_ASSETHANDLE_H
#define BEMBEL_ASSETHANDLE_H
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

struct AssetHandle final
{
	unsigned index;
	unsigned hash;
};

inline bool operator < (const AssetHandle& first, const AssetHandle& second)
{
	return first.index < second.index;
}

} // end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
