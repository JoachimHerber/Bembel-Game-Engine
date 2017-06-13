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
/* INCLUDES                                                                   */
/*============================================================================*/

#include "logger.h"
#include "log-sink.h"
#include "console-log-sink.h"


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Logger Logger::info(std::make_shared<ConsoleLogSink>("", 0x0F));
Logger Logger::error(std::make_shared<ConsoleLogSink>("###ERROR###\t", 0xCE));
Logger Logger::debug(std::make_shared<ConsoleLogSink>("", 0x09));
Logger Logger::warning(std::make_shared<ConsoleLogSink>("- WARNING -\t", 0xE0));

Logger::Logger(std::shared_ptr<LogSink> sink)
	: log_sink_(sink)
{}

Logger::~Logger()
{}

LogMessage Logger::operator()(
	const std::string& file,
	unsigned int line)
{
	return LogMessage(
		this, file, line);
}

void Logger::Flush(const LogMessage& message)
{
	if( log_sink_ )
		log_sink_->WriteMessage(message);
}

void Logger::SetLogSink(std::shared_ptr<LogSink> sink)
{
	log_sink_ = sink;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

