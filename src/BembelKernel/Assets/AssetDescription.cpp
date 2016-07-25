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
	, _properties(std::move(other._properties))
{}

AssetDescription::~AssetDescription()
{}

const std::string& AssetDescription::GetName() const
{
	return _name;
}

void AssetDescription::SetName(const std::string& name)
{
	_name = name;
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
	auto attrib = properties->FirstAttribute();
	while(attrib)
	{
		const char* name  = attrib->Name();
		const char* value = attrib->Value();
		if (name != nullptr && value != nullptr)
			asset._properties[name] = value;

		attrib = attrib->Next();
	}
	return asset;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
