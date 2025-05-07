#pragma once

#include <string>
#include <flib/base/Assert.hpp>
#include <flib/common.hpp>
#include <flib/object/Allocator.hpp>

namespace neo {

    template <typename T>
    class FlibAllocator {
    public:
        using value_type = T;

        FlibAllocator() noexcept : allocator(flib::getGlobalAllocator()) {}
        explicit FlibAllocator(flib::IAllocator& alloc) noexcept : allocator(alloc) {
        }

        template <typename U>
        FlibAllocator(const FlibAllocator<U>& other) noexcept : allocator(other.getAllocator()) {}

        T* allocate(std::size_t n) {
            void* ptr = allocator.allocate(n * sizeof(T), alignof(T));
            FLIB_ASSERT(ptr != nullptr);
            return static_cast<T*>(ptr);
        }

        void deallocate(T* p, std::size_t /*n*/) noexcept {
            allocator.deallocate(p);
        }

        template <typename U>
        struct rebind {
            using other = FlibAllocator<U>;
        };

        flib::IAllocator& getAllocator() const noexcept { return allocator; }

        // Required for equality comparison
        // bool operator==(const FlibAllocator& rhs) const noexcept { return allocator == rhs.allocator; }
        // bool operator!=(const FlibAllocator& rhs) const noexcept { return allocator != rhs.allocator; }

    private:
        template <typename U> friend class FlibAllocator;
        flib::IAllocator& allocator;
    };

    using NString = std::basic_string<char, std::char_traits<char>, FlibAllocator<char>>;

} // namespace flib
