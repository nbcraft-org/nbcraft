#include "ResultContainer.hpp"

ResultContainer::ResultContainer()
{
}

ResultContainer::~ResultContainer()
{
}

uint16_t ResultContainer::getContainerSize() const
{
    return 1;
}

ItemStack& ResultContainer::getItem(StackID stackId)
{
    return m_item;
}

std::string ResultContainer::getName() const
{
    return "";
}

ItemStack ResultContainer::removeItem(StackID stackId, int)
{
    if (stackId != 0)
        return ItemStack::EMPTY;

    ItemStack result = m_item;
    m_item = ItemStack::EMPTY;
    return result;
}

void ResultContainer::setItem(StackID stackId, const ItemStack& item)
{
    if (stackId == 0)
        m_item = item;
}

void ResultContainer::setContainerChanged(StackID stackId)
{
}

bool ResultContainer::stillValid(Player*) const
{
    return true;
}
