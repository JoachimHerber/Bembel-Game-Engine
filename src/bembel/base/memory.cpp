module;

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif
module bembel.base;

namespace bembel::base {

namespace memory {

    Chunk alloc() {
#ifdef _WIN32
        u8* mem = (u8*)_aligned_malloc(ALLOCATION_GRANULARITY, ALLOCATION_GRANULARITY);
#else
        u8* mem = (u8*)std::aligned_alloc(ALLOCATION_GRANULARITY, ALLOCATION_GRANULARITY);
#endif
        Chunk c{mem, ALLOCATION_GRANULARITY};
        return c;
    }

    void free(Chunk c) {
#ifdef _WIN32
        _aligned_free(c.data());
#else
        std::free(c.data());
#endif
    }

} // namespace memory

} // namespace bembel::base
