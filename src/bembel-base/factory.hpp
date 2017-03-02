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
namespace bembel{

template<typename ObjectType, typename ... TArgs>
class TFactory
{
public:
	TFactory();
	~TFactory();

	using ObjectPtr = std::unique_ptr<ObjectType>;

	class ObjectGeneratorBase
	{
	public:
		virtual ObjectPtr CreateObject(TArgs ... args) = 0;
	};
	using ObjectGeneratorPtr = std::shared_ptr<ObjectGeneratorBase>;

	void RegisterObjectGenerator(const std::string& type, ObjectGeneratorPtr);

	ObjectPtr CreateObject(const std::string&, TArgs ... args) const;

	template<typename T>
	class DefaultObjectGenerator : public ObjectGeneratorBase
	{
	public:
		virtual ObjectPtr CreateObject(TArgs ... args) override;
	};

	template<typename T>
	void RegisterDefaultObjectGenerator(const std::string& type);

private:
	std::map<std::string, ObjectGeneratorPtr> _generators;
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
