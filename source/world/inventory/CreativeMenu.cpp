#include "CreativeMenu.hpp"
#include "Slot.hpp"

CreativeMenu::CreativeMenu(Container* inventory, Container* container)
    : m_container(container)
{
    int rows = m_container->getContainerSize() / 8;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int index = col + row * 8;
            addSlot(new Slot(m_container, index));
        }
    }

    for (int col = 0; col < 9; ++col)
    {
        addSlot(new Slot(inventory, col, Slot::HOTBAR));
    }

    updateScroll(0.0f);
}

void CreativeMenu::updateScroll(float scroll)
{
    int scrollLimit = creativeItems.size() / 8 - 8 + 1;
    if (scrollLimit < 0) scrollLimit = 0;
    int scrollOffset = (int)((scroll * scrollLimit) + 0.5f);

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int itemIndex = col + (row + scrollOffset) * 8;
            if (itemIndex >= 0 && itemIndex < (int)creativeItems.size())
            {
                m_container->setItem(col + row * 8, creativeItems[itemIndex]);
            }
            else
            {
                m_container->setItem(col + row * 8, ItemStack::EMPTY);
            }
        }
    }
}
