#ifndef BEMBE_KERNEL_SYSTEM_HPP
#define BEMBE_KERNEL_SYSTEM_HPP

#include <bembel/base/io/xml.hpp>

namespace bembel::kernel {

class Kernel;
class AssetManager;

class BEMBEL_API System {
  public:
    System(Kernel& kernel, const std::string& name);
    virtual ~System();

    const std::string&  getName() const;
    Kernel&             getKernel();
    const Kernel&       getKernel() const;
    AssetManager&       getAssetManager();
    const AssetManager& getAssetManager() const;

    virtual bool configure(const base::xml::Element*);

    virtual bool init();
    virtual void shutdown();

    virtual void update(double time_since_last_update) = 0;

  protected:
    Kernel&           kernel;
    const std::string system_name;
};

} // namespace bembel::kernel
#endif // include guards
