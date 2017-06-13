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

#ifndef BEMBEL_KERNEL_KERNEL_H_
#define BEMBEL_KERNEL_KERNEL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class EventManager;
class AssetManager;
class DisplayManager;
class System;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Kernel final
{
public:
	Kernel();
	~Kernel();

	EventManager*   GetEventManager() const;
	AssetManager*   GetAssetManager() const;
	DisplayManager* GetDisplayManager() const;

	template<typename SystemType, typename ... Args>
	SystemType* AddSystem(Args&& ... args);
	bool RemoveSystem(const std::string& name);
	System* GetSystem(const std::string& name);


	bool LoadSetting(const std::string& config_file_name);

	bool InitSystems();
	void UpdateSystems(double time_since_last_update);
	void ShutdownSystems();

	void PollEvents();
private:
	std::unique_ptr<EventManager>   event_manager_;
	std::unique_ptr<AssetManager>   asset_manager_;
	std::unique_ptr<DisplayManager> display_manager_;

	std::unordered_map<std::string, unsigned> system_mapping_;
	std::vector<std::unique_ptr<System>>      systems_;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel {

template<typename SystemType, typename ... Args>
inline SystemType* Kernel::AddSystem(Args&& ... args)
{
	auto system = std::make_unique<SystemType>(this, args...);
	auto system_pointer = system.get();

	const std::string& name = system->GetName();

	auto it = system_mapping_.find(name);
	if( it != system_mapping_.end() )
	{
		BEMBEL_LOG_ERROR()
			<< "System \""
			<< system->GetName()
			<< "\" has already been added."
			<< std::endl;
		return nullptr;
	}

	for( size_t n = 0; n <systems_.size(); ++n )
	{
		if( !systems_[n] )
		{
			systems_[n] = std::move(system);
			system_mapping_.emplace(name, n);
			return system_pointer;
		}
	}

	system_mapping_.emplace(name, systems_.size());
	systems_.push_back(std::move(system));
	return system_pointer;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
