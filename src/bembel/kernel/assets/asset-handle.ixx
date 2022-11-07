export module bembel.kernel.assets:AssetHandle;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export struct AssetHandle final {
    union {
        struct {
            u32 index;
            u16 version;
            u16 type_id;
        };
        u64 value = ~(base::u64)(0U);
    };
};

export constexpr bool operator<(AssetHandle const& first, AssetHandle const& second) {
    return first.value < second.value;
}

} // namespace bembel::kernel
