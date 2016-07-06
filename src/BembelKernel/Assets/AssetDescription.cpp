/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "AssetDescription.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

AssetDescription::AssetDescription()
{}

AssetDescription::~AssetDescription()
{}

const std::string& AssetDescription::GetName() const
{
	return _name;
}
const std::string& AssetDescription::GetFilePath() const
{
	return _file;
}

void AssetDescription::SetName(const std::string& name)
{
	_name = name;
}
void AssetDescription::SetFilePath(const std::string& path)
{
	_file = path;
}

bool AssetDescription::GetProperty(const std::string& name, std::string& value) const
{
	auto it = _properties.find(name);
	if (it != _properties.end())
	{
		value = it->second;
		return true;
	}
	return false;
}

void AssetDescription::SetProperty(const std::string& name, const std::string& value)
{
	_properties[name] = value;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
