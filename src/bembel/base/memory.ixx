module;
#include <span>
export module bembel.base:Memory;

import :Types;

namespace bembel::base {

export namespace memory {

    constexpr inline u64 ALLOCATION_GRANULARITY = 64 * 1024;

    using Chunk = std::span<u8, ALLOCATION_GRANULARITY>;

    Chunk alloc();
    void  free(Chunk);

    template <typename T>
    std::span<T, ALLOCATION_GRANULARITY / sizeof(T)> alloc() {
        std::span<T, ALLOCATION_GRANULARITY / sizeof(T)> c{
            (T*)alloc().data(), ALLOCATION_GRANULARITY / sizeof(T)};
        return c;
    }
    template <typename T>
    void free(std::span<T, ALLOCATION_GRANULARITY / sizeof(T)> c) {
        free(Chunk((u8*)c.data(), ALLOCATION_GRANULARITY));
    }
} // namespace memory

} // namespace bembel::base