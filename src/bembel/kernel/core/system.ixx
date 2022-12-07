module;
#include "bembel/pch.h"
export module bembel.kernel.core:System;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

export class System {
  public:
    System(std::string_view name) : name{name} {}
    virtual ~System() = default;

    virtual bool configure(xml::Element const*) { return true; }

    virtual bool init() { return true; }
    virtual void shutdown() {}

    virtual void update(double time_since_last_update) = 0;

  public:
    const std::string name;
};

} // namespace bembel::kernel
