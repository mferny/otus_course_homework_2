#pragma once

#include <iostream>
#include <array>

static constexpr std::size_t DefaultSize = 1;

template<class T, std::size_t N = DefaultSize, bool isLimited = false>
class CustomAllocator
{
    static_assert(!std::is_same_v<T, void>, "Type of the allocator can not be void");

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    CustomAllocator() = default;

    ~CustomAllocator() = default;

    template<typename U>
    CustomAllocator(const CustomAllocator<U, N, isLimited>&) {}

    template<typename U>
    struct rebind
    {
        typedef CustomAllocator<U, N, isLimited> other;
    };

    [[nodiscard]] pointer allocate(std::size_t count_objects)
    {
        // allocator has predefined size
        if constexpr (isLimited)
        {
            if (m_allocatedCount + count_objects <= N)
            {
                std::cout << __PRETTY_FUNCTION__
                          << "[n = " << count_objects << ']' << std::endl;

                m_memoryPool[m_allocatedCount] = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * count_objects));
                m_allocatedCount += count_objects;
                return m_memoryPool[m_allocatedCount - count_objects];
            }

            throw std::length_error(
                    "Cannot allocate. Size is bigger than predefined in constructor");
        }
        else
        {
            // allocator doesn't have predefined size, so it is unframed
            std::cout << __PRETTY_FUNCTION__
                      << "[n = " << count_objects << ']' << std::endl;

            return reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * count_objects));
        }

    }

    void deallocate(T* ptr, std::size_t count_objects)
    {
        std::cout << __PRETTY_FUNCTION__
                  << "[n = " << count_objects << ']' << std::endl;

        free(ptr);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~T();
    }

private:
    std::array<pointer, N> m_memoryPool;
    std::size_t m_allocatedCount = 0;
};