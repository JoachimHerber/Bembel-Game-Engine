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

AssetDescription::AssetDescription(AssetDescription&& other)
	: _name(std::move(other._name))
	, _file(std::move(other._file))
	, _properties(std::move(other._properties))
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

AssetDescription AssetDescription::Parse(const xml::Element* properties)
{
	AssetDescription asset;
	xml::GetAttribute(properties, "name", asset._name);
	xml::GetAttribute(properties, "file", asset._file);
	for (auto it : xml::IterateChildElements(properties, "property"))
	{
		const char* name  = it->Attribute("name");
		const char* value = it->Attribute("value");
		if (name != nullptr && value != nullptr)
			asset._properties[name] = value;
	}
	return asset;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
