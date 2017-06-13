/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template<typename ObjectType, typename ... TArgs>
inline TFactory<ObjectType, TArgs ...>::TFactory()
{}

template<typename ObjectType, typename ... TArgs>
inline TFactory<ObjectType, TArgs ...>::~TFactory()
{}

template<typename ObjectType, typename ... TArgs>
inline void TFactory<ObjectType, TArgs ...>::RegisterObjectGenerator(
	const std::string& type, std::unique_ptr<ObjectGeneratorBase> generator)
{
	generators_.emplace(type, std::move(generator));
}

template<typename ObjectType, typename ... TArgs>
inline std::unique_ptr<ObjectType>
TFactory<ObjectType, TArgs ...>::CreateObject(
	const std::string& type,
	TArgs ... args) const
{
	auto it = generators_.find(type);

	if( it == generators_.end() )
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
	generators_.emplace(
		type, std::make_unique<DefaultObjectGenerator<T>>());
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/