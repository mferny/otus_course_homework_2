#include "CustomAllocator.h"
#include "SimpleContainer.h"

#include <map>

using MapWithPredefSize = std::map<int, int, std::less<>, CustomAllocator<std::pair<const int, int>, 10>>;
using Map = std::map<int, int, std::less<>, CustomAllocator<std::pair<const int, int>>>;

constexpr int factorial(int n)
{
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

template <typename Map_t>
void printMap(const Map_t& item)
{
    for (const auto i : item)
    {
        std::cout << i.first << ' ' << i.second << std::endl;
    }
    std::cout << '\n';
}

int main()
{
    {
        std::cout << "Map containing 10 elements without predefined size" << std::endl;
        Map map_with_custom_allocator;
        for(int i = 0; i < 10; i++)
        {
            map_with_custom_allocator[i] = factorial(i);
        }
        printMap(map_with_custom_allocator);
    }

    std::cout << std::endl;

    {
        std::cout << "Map containing 10 elements with predefined size 10" << std::endl;
        MapWithPredefSize map_with_custom_allocator;
        for(int i = 0; i < 10; i++)
        {
            map_with_custom_allocator[i] = factorial(i);
        }
        printMap(map_with_custom_allocator);
    }

    std::cout << std::endl;

    {
        std::cout << "Custom container containing 10 elements without predefined size" << std::endl;
        SimpleContainer<int, CustomAllocator<int>> container;
        for (int i = 0; i < 10; ++i)
        {
            container.push_back(42);
        }
        container.print();
    }

    std::cout << std::endl;

    {
        std::cout << "Custom container containing 10 elements with predefined size 10" << std::endl;
        SimpleContainer<int, CustomAllocator<int, 10>> containerWithPrealloc(10);
        for (int i = 0; i < 10; ++i)
        {
            containerWithPrealloc.push_back(42);
        }
        containerWithPrealloc.print();
    }

    std::cout << std::endl;

    {
        std::cout << "Custom container containing 10 elements with reserve of 10" << std::endl;
        SimpleContainer<int, CustomAllocator<int>> containerWithReserve;
        containerWithReserve.reserve(10);
        for (int i = 0; i < 10; ++i)
        {
            containerWithReserve.push_back(42);
        }
        containerWithReserve.print();
    }

    return 0;
}
