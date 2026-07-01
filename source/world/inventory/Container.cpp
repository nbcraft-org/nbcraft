#include "Container.hpp"

std::vector<ItemStack> Container::getSlotCopies()
{
    std::vector<ItemStack> content;
    Size size = getContainerSize();
    content.reserve(size);

    StackID stackId = 0;
    while (stackId < size)
    {
        const ItemStack& item = getItem(stackId);
        content.push_back(item);
        stackId++;
    }

    return content;
}