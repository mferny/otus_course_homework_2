#pragma once

#include "CustomAllocator.h"

#include <memory>
#include <algorithm>

template <typename T, typename Alloc = std::allocator<T>>
class SimpleContainer
{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = std::size_t;
    using reference = T&;

    constexpr SimpleContainer() = default;

    ~SimpleContainer()
    {
        m_alloc.deallocate(m_begin, m_endOfStorage);
    }

    constexpr SimpleContainer& operator= (SimpleContainer&& other) noexcept
    {
        std::copy(other.m_begin, other.m_begin + m_current, m_begin);
    }

    constexpr SimpleContainer(size_type size)
    {
        m_begin = m_alloc.allocate(size);
        m_endOfStorage = size;
    }

    constexpr iterator begin() noexcept
    {
        return m_begin;
    }

    constexpr iterator end() noexcept
    {
        return m_begin + m_current;
    }

    constexpr bool empty() noexcept
    {
        return m_current == 0;
    }

    constexpr size_type size() noexcept
    {
        return m_current;
    }

    constexpr size_type capacity() noexcept
    {
        return m_endOfStorage - m_current;
    }

    constexpr reference operator [] (size_type n)
    {
        return *(m_begin + n);
    }

    constexpr size_type max_size() noexcept
    {
        return std::numeric_limits<std::ptrdiff_t>::max();
    }

    constexpr void reserve(size_type new_cap)
    {
        if (new_cap > max_size())
        {
            throw std::length_error(
                "Cannot reserve. The new size is bigger than max_size.");
        }
        // allocate new bunch of memory for the first time
        if (m_begin == nullptr)
        {
            m_begin = m_alloc.allocate(new_cap);
            m_current = {};
            m_endOfStorage = new_cap;
        }
        else
        {
            // allocate new memory and clean previous
            iterator newData = m_alloc.allocate(new_cap);
            std::copy(m_begin, m_begin + m_current, newData);
            m_endOfStorage = new_cap;

            m_alloc.deallocate(m_begin, m_current);
            m_begin = std::move(newData);
        }
    }

    void push_back(const T& value)
    {
        if (m_current == m_endOfStorage)
        {
            const auto size = m_current == 0 ? 1 : m_current;
            reserve(size * 2);
        }
        m_begin[m_current] = value;
        m_current++;
    }

    void print() noexcept
    {
        for (const auto i : *this)
        {
            std::cout << i << std::endl;
        }
        std::cout << '\n';
    }

private:
    iterator m_begin = nullptr;
    size_type m_current = 0;
    size_type m_endOfStorage = 0;

    allocator_type m_alloc = {};
};

