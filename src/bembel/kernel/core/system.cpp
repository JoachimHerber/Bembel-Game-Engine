#include "./system.hpp"

#include <bembel/base/logging/logger.hpp>

#include "./kernel.hpp"

namespace bembel::kernel {

System::System(Kernel& kernel, const std::string& name)
: kernel(kernel)
, system_name(name) {
}

System::~System() {
}

const std::string& System::getName() const {
    return this->system_name;
}

Kernel& System::getKernel() {
    return this->kernel;
}

const Kernel& System::getKernel() const {
    return this->kernel;
}

AssetManager& System::getAssetManager() {
    return this->kernel.getAssetManager();
}
const AssetManager& System::getAssetManager() const {
    return this->kernel.getAssetManager();
}

bool System::configure(const base::xml::Element*) {
    return true;
}

bool System::init() {
    return true;
}

void System::shutdown() {
}

} // namespace bembel::kernel
