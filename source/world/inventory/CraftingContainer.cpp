#include "CraftingContainer.hpp"

CraftingContainer::CraftingContainer(ContainerMenu* menu, int width, int height)
    : m_items(width * height)
    , m_pMenu(menu)
    , m_width(width)
{
}

CraftingContainer::~CraftingContainer()
{
}

uint16_t CraftingContainer::getContainerSize() const
{
    return uint16_t(m_items.size());
}

ItemStack& CraftingContainer::getItem(StackID stackId)
{
    return m_items[stackId];
}

const ItemStack& CraftingContainer::getItem(int x, int y)
{
    if (x >= 0 && x < m_width)
    {
        ItemStack* stack = tryGetItem(x + y * m_width);
        return stack ? *stack : ItemStack::EMPTY;
    }
    return ItemStack::EMPTY;
}

std::string CraftingContainer::getName() const
{
    return "Crafting";
}

ItemStack CraftingContainer::removeItem(StackID stackId, int count)
{
    if (stackId < 0 || stackId >= getContainerSize()) return ItemStack::EMPTY;

    ItemStack& item = m_items[stackId];
    if (item)
    {
        ItemStack removed;

        if (item.m_count <= count)
        {
            removed = item;
            m_items[stackId] = ItemStack::EMPTY;
        } 
        else
        {
            removed = item.remove(count);
            if (item.m_count == 0)
                m_items[stackId] = ItemStack::EMPTY;
        }

        m_pMenu->slotsChanged(this);
        return removed;
    }

    return ItemStack::EMPTY;
}

void CraftingContainer::setItem(StackID stackId, const ItemStack& item)
{
    if (stackId >= 0 && stackId < getContainerSize())
    {
        m_items[stackId] = item;
        m_pMenu->slotsChanged(this);
    }
}

void CraftingContainer::setContainerChanged(StackID stackId)
{
}

bool CraftingContainer::stillValid(Player* player) const
{
    return true;
}