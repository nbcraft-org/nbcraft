#include "Slot.hpp"

Slot::Slot(Container* container, Container::StackID stackId, Group group)
    : m_pContainer(container)
    , m_stackId(stackId)
    , m_group(group)
{
}

Slot::~Slot()
{
}

bool Slot::canSync() const
{
    return true;
}

void Slot::set(const ItemStack& item)
{
    m_pContainer->setItem(m_stackId, item);
    setChanged();
}
