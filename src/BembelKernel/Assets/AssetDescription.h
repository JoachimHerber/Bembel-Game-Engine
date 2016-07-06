#ifndef BEMBEL_ASSETDESCRIPTION_H
#define BEMBEL_ASSETDESCRIPTION_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

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
	~AssetDescription();

	const std::string& GetName() const;
	void SetName(const std::string&);

	const std::string& GetFilePath() const;
	void SetFilePath(const std::string&);

	bool GetProperty(const std::string& name, std::string& value) const;
	void SetProperty(const std::string& name, const std::string& value);

private:
	std::string _name;
	std::string _file;

	std::unordered_map<std::string, std::string> _properties;
};


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
