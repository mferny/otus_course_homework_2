#pragma once

#include <iostream>
#include <array>

static constexpr std::size_t DefaultSize = 1;

template<class T, std::size_t N = DefaultSize>
class CustomAllocator
{
    static_assert(not std::is_same_v<T, void>, "Type of the allocator can not be void");

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    
    CustomAllocator() = default;
    ~CustomAllocator()
    {
        std::cout << "Clearing memory pool" << std::endl;
        free(m_memoryPool);
    }

    template<typename U>
    CustomAllocator(const CustomAllocator<U, N>&) {}

    template<typename U>
    struct rebind
    {
        typedef CustomAllocator<U, N> other;
    };

    [[nodiscard]] pointer allocate(std::size_t count_objects)
    {
        // allocate pool
        if (m_allocatedCount == 0)
        {
            std::cout << __PRETTY_FUNCTION__
                      << "[n = " << N << ']' << std::endl;

            m_memoryPool = reinterpret_cast<pointer>(std::malloc(N * sizeof(value_type)));
            m_allocatedCount += count_objects;
            return m_memoryPool;
        }

        std::cout << "Getting memory of size " << count_objects <<" from pool" << std::endl;
        if (m_allocatedCount + count_objects <= N)
        {
            m_allocatedCount += count_objects;
            return m_memoryPool + (m_allocatedCount - count_objects);
        }

        throw std::length_error(
                "Cannot allocate. Size is bigger than predefined in constructor");

    }

    void deallocate(T* ptr, std::size_t count_objects)
    {

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
    pointer m_memoryPool = nullptr;
    std::size_t m_allocatedCount = 0;
};