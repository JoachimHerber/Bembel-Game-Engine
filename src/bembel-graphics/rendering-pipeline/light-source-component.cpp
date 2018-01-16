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

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "light-source-component.h"

#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& PointLightProperties::GetComponentTypeName()
{
	const static std::string typeName = "PointLight";
	return typeName;
}

bool PointLightProperties::InitComponent(
	AssetManager*,
	const xml::Element* properties,
	PointLightProperties* component	)
{
	component->bulb_radius = 1;
	xml::GetAttribute(properties, "color", component->color);
	float intensity;
	if( xml::GetAttribute(properties, "intensity", intensity) )
		component->color *= intensity;

	xml::GetAttribute(properties, "bulbRadius", component->bulb_radius);
	xml::GetAttribute(properties, "cutoffRadius", component->cutoff_radius);

	return true;
}

const std::string& DirLightProperties::GetComponentTypeName()
{
	const static std::string typeName = "DirectionalLight";
	return typeName;
}

bool DirLightProperties::InitComponent(
	AssetManager*,
	const xml::Element* properties,
	DirLightProperties* component)
{
	xml::GetAttribute(properties, "color", component->color);
	float intensity;
	if( xml::GetAttribute(properties, "intensity", intensity) )
		component->color *= intensity;
	xml::GetAttribute(properties, "direction", component->direction);
	component->direction = glm::normalize(component->direction);

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
