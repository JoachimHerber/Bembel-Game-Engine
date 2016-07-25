#ifndef BEMBEL_ASSETDESCRIPTION_H
#define BEMBEL_ASSETDESCRIPTION_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelBase/XML.h>
#include <BembelBase/Conversion.h>

#include <string>
#include <unordered_map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API AssetDescription
{
public:
	AssetDescription();
	AssetDescription(AssetDescription&&);
	~AssetDescription();

	const std::string& GetName() const;
	void SetName(const std::string&);

	template<typename T>
	bool GetProperty(const std::string& name, T& value) const;
	bool GetProperty(const std::string& name, std::string& value) const;
	void SetProperty(const std::string& name, const std::string& value);

	static AssetDescription Parse(const xml::Element*);

private:
	std::string _name;
	std::unordered_map<std::string, std::string> _properties;
};

template<typename T>
bool AssetDescription::GetProperty(const std::string& name, T& value) const
{

	auto it = _properties.find(name);
	if (it != _properties.end())
	{
		conversion::FromString(it->second, value);
		return true;
	}
	return false;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
