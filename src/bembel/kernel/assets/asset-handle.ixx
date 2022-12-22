module;
#include <bit>
#include <limits>
export module bembel.kernel.assets:AssetHandle;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export struct AssetHandle final {
    u32 index   = std::numeric_limits<u32>::max();
    u16 version = std::numeric_limits<u16>::max();
    u16 type_id = std::numeric_limits<u16>::max();

    operator u64() { return std::bit_cast<u64>(*this); }
};

export bool operator<(AssetHandle a, AssetHandle b) {
    return u64(a) < u64(b);
}

} // namespace bembel::kernel
