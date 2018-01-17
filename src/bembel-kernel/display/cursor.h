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

#ifndef BEMBEL_KERNEL_DISPLAY_CURSOR_H_
#define BEMBEL_KERNEL_DISPLAY_CURSOR_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <bembel-kernel/assets/serial-asset-loader.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWcursor;

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Cursor
{
public:
	using DefaultLoaderType = SerialAssetLoader<Cursor>;

public:
	Cursor(GLFWcursor*);
	~Cursor();

	GLFWcursor* GetCursor();

	const static std::string& GetTypeName();

	static std::unique_ptr<Cursor> LoadAsset(
		AssetManager* asset_manager, const std::string& path);
	static std::unique_ptr<Cursor> CreateAsset(
		AssetManager* asset_manager, const xml::Element* properties);

	static void DeleteAsset(AssetManager*, std::unique_ptr<Cursor>){}

private:
	GLFWcursor* cursor_imp_;
};

struct BEMBEL_API SetCursorEvent
{
	Cursor* const cursor;
	unsigned window_id_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
