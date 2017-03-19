#ifndef BEMBEL_BASE_FACTORY_H_
#define BEMBEL_BASE_FACTORY_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "xml.h"

#include <string>
#include <memory>
#include <map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename ObjectType, typename ... TArgs>
class TFactory
{
public:
	TFactory();
	TFactory(const TFactory&) = delete;
	TFactory& operator=(const TFactory&) = delete;
	~TFactory();

	class ObjectGeneratorBase
	{
	public:
		virtual std::unique_ptr<ObjectType>  CreateObject(TArgs ... args) = 0;
	};

	void RegisterObjectGenerator(const std::string& type, std::unique_ptr<ObjectGeneratorBase>);

	std::unique_ptr<ObjectType> CreateObject(const std::string&, TArgs ... args) const;

	template<typename T>
	class DefaultObjectGenerator : public ObjectGeneratorBase
	{
	public:
		virtual std::unique_ptr<ObjectType> CreateObject(TArgs ... args) override;
	};

	template<typename T>
	void RegisterDefaultObjectGenerator(const std::string& type);

private:
	std::map<std::string, std::unique_ptr<ObjectGeneratorBase>> generators_;
};

template<typename T>
using Factory = TFactory<typename T, const xml::Element*>;

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "Factory.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
