export module bembel.kernel.core:Scheduler;

import std;
import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export class Scheduler {
  public:
    Scheduler()  {}
    ~Scheduler() = default;

    Signal<Seconds> frameStart;
    Signal<Seconds> update;
    Signal<Seconds> render;
    Signal<Seconds> frameEnd;

  private:
};

} // namespace bembel::kernel