/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
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

#ifndef BEMBEL_BASE_LOGGING_LOGMESSAGE_H_
#define BEMBEL_BASE_LOGGING_LOGMESSAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <sstream>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Logger;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API LogMessage
{
	LogMessage(Logger*, const std::string&, unsigned int);
public:
	LogMessage(LogMessage&&);
	~LogMessage();

	template <typename T>
	LogMessage& operator<< (const T& value);
	LogMessage& operator<< (std::ostream& (*fn)(std::ostream&));

	const std::string& GetFile() const;
	unsigned int       GetLine() const;

	std::string GetDescription() const;

private:
	Logger* logger_;

	std::string  file_;
	std::string  function_;
	unsigned int line_;

	std::stringstream buffer_;

	friend class Logger;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/
namespace bembel {

template <typename T>
inline LogMessage& LogMessage::operator<<(const T& value)
{
	buffer_ << value;
	return *this;
}
inline LogMessage& LogMessage::operator<<(std::ostream& (*function)(std::ostream&))
{
	buffer_ << function;
	return *this;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
