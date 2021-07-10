#pragma once

#include <iostream>
#include <array>

template<class T, std::size_t N = std::size_t()>
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
    CustomAllocator(const CustomAllocator<U, N>&) {}

    template<typename U>
    struct rebind
    {
        typedef CustomAllocator<U, N> other;
    };

    [[nodiscard]] pointer allocate(std::size_t count_objects)
    {
        // allocator has predefined size
        if constexpr(N > 0)
        {
            if (m_memoryPool == nullptr)
            {
                std::cout << __PRETTY_FUNCTION__
                          << "[n = " << N << ']' << std::endl;

                m_memoryPool = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * N));
                if (not m_memoryPool)
                {
                    throw std::bad_alloc();
                }

                m_allocatedCount += count_objects;
                return m_memoryPool;
            }
            // check if we are not out boundaries
            if (m_allocatedCount + count_objects <= N)
            {
                m_memoryPool += m_allocatedCount;
                m_allocatedCount += count_objects;

                return m_memoryPool;
            }

            throw std::length_error(
                    "Cannot allocate. Size is bigger than predefined in constructor");
        }

        // allocator doesn't have predefined size, so it is unframed
        std::cout << __PRETTY_FUNCTION__
                  << "[n = " << count_objects << ']' << std::endl;

        m_memoryPool = reinterpret_cast<pointer>(std::malloc(sizeof(value_type) * count_objects));
        if (not m_memoryPool)
        {
            throw std::bad_alloc();
        }

        return m_memoryPool;
    }

    void deallocate(T* ptr, std::size_t count_objects)
    {
        std::cout << __PRETTY_FUNCTION__
                  << "[n = " << count_objects << ']' << std::endl;

        std::free(ptr);
        m_allocatedCount -= count_objects;
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