#include "SimpleContainer.hpp"

SimpleContainer::SimpleContainer(Size size, const std::string& name)
    : m_items(size)
    , m_name(name)
{
}

ItemStack& SimpleContainer::getItem(StackID index)
{
    return m_items[index];
}

ItemStack SimpleContainer::removeItem(StackID index, int count)
{
    ItemStack& itemStack = m_items[index];

    if (!itemStack.isEmpty())
    {
        ItemStack result = ItemStack::EMPTY;
        if (itemStack.m_count <= count)
        {
            result = itemStack;
            m_items[index] = ItemStack::EMPTY;
            setContainerChanged(index);
            return result;
        }
        else
        {
            result = itemStack.remove(count);
            if (!itemStack.m_count)
                m_items[index] = ItemStack::EMPTY;

            setContainerChanged(index);
            return result;
        }
    }
    return ItemStack::EMPTY;
}

void SimpleContainer::setItem(StackID index, const ItemStack& item)
{
    m_items[index] = item;
    if (!item.isEmpty() && item.m_count > getMaxStackSize())
        m_items[index].m_count = getMaxStackSize();

    setContainerChanged(index);
}

std::string SimpleContainer::getName() const
{
    return m_name;
}

Container::Size SimpleContainer::getContainerSize() const
{
    return (Size)(m_items.size());
}

void SimpleContainer::clear()
{
    std::fill(m_items.begin(), m_items.end(), ItemStack::EMPTY);
}

void SimpleContainer::load(const CompoundTag& tag)
{
    clear();
    const ListTag* list = tag.getList("Items");

    const std::vector<Tag*>& itemTags = list->rawView();

    for (std::vector<Tag*>::const_iterator it = itemTags.begin(); it != itemTags.end(); it++)
    {
        const CompoundTag* itemTag = (const CompoundTag*)*it;
        if (itemTag)
        {
            uint8_t slot = itemTag->getInt8("Slot") & 255;
            ItemStack item = ItemStack::fromTag(*itemTag);
            if (!itemTag->isEmpty() && slot >= 0 && slot < m_items.size())
                m_items[slot] = item;
        }
    }
}

void SimpleContainer::save(CompoundTag& tag) const
{
    ListTag* list = new ListTag;

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (!m_items[i].isEmpty())
        {
            CompoundTag* itemTag = new CompoundTag;
            itemTag->putInt8("Slot", i);
            m_items[i].save(*itemTag);
            list->add(itemTag);
        }
    }

    tag.put("Items", list);
}
