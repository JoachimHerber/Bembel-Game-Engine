#ifndef BEMBEL_KERNEL_CORE_APPLICATION_HPP
#define BEMBEL_KERNEL_CORE_APPLICATION_HPP

#include <bembel/library.hpp>

namespace bembel::kernel {

class Kernel;

class BEMBEL_API Application {
  public:
    Application();
    virtual ~Application();

    bool run();
    void quit();

  protected:
    virtual void mainLoop();

    virtual bool init()    = 0;
    virtual void cleanup() = 0;

    virtual void update(double timeDelta) = 0;

  protected:
    std::unique_ptr<Kernel> kernel;

  private:
    bool should_exit_main_loop = false;
};

} // namespace bembel::kernel
#endif // include guards
