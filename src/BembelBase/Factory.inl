/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel{

template<typename ObjectType, typename ... TArgs>
inline TFactory<ObjectType, TArgs ...>::TFactory()
{}

template<typename ObjectType, typename ... TArgs>
inline TFactory<ObjectType, TArgs ...>::~TFactory()
{}

template<typename ObjectType, typename ... TArgs>
inline void TFactory<ObjectType, TArgs ...>::RegisterObjectGenerator(
	const std::string& type, ObjectGeneratorPtr generator)
{
	_generators.emplace(type, generator);
}

template<typename ObjectType, typename ... TArgs>
inline std::unique_ptr<ObjectType> 
TFactory<ObjectType, TArgs ...>::CreateObject(
	const std::string& type,
	TArgs ... args) const
{
	auto it = _generators.find(type);

	if (it == _generators.end())
		return nullptr;

	return it->second->CreateObject(args ...);
}

template<typename ObjectType, typename ... TArgs>
template<typename T>
inline std::unique_ptr<ObjectType> 
TFactory<ObjectType, TArgs ...>::DefaultObjectGenerator<T>::CreateObject(
	TArgs ... args)
{
	return T::CreateInstance(args ...);
}

template<typename ObjectType, typename ... TArgs>
template<typename T>
inline void TFactory<ObjectType, TArgs ...>::RegisterDefaultObjectGenerator(
	const std::string& type)
{
	_generators.emplace(
		type, std::make_shared<DefaultObjectGenerator<T>>());
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/