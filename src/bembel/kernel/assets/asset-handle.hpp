#ifndef BEMBEL_KERNEL_ASSETS_ASSETHANDLE_HPP
#define BEMBEL_KERNEL_ASSETS_ASSETHANDLE_HPP

#include <bembel/library.hpp>

namespace bembel::kernel {

struct BEMBEL_API AssetHandle final {
    union {
        struct {
            uint32_t index;
            uint16_t version;
            uint16_t type_id;
        };
        uint64_t value = ~uint64_t(0U);
    };
};

inline bool operator<(const AssetHandle& first, const AssetHandle& second) {
    return first.value < second.value;
}

} // namespace bembel::kernel
#endif // include guards
