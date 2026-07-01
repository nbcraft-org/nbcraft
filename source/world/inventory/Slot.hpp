#pragma once

#include <stdint.h>

#include "Container.hpp"

class ItemStack;

class Slot
{
public:
    enum Group
    {
        CONTAINER,
        INVENTORY,
        HOTBAR,
        INPUT,
        OUTPUT,
        ARMOR
    };

    Slot(Container* container, Container::StackID stackId, Group group = CONTAINER);
    virtual ~Slot();

    virtual bool canSync() const;

    virtual void onTake(ItemStack&) { setChanged(); }

    virtual bool mayPlace(const ItemStack& item) const { return true; }

    virtual ItemStack& getItem() { return m_pContainer->getItem(m_stackId); }

    virtual bool hasItem() { return !getItem().isEmpty(); }

    virtual void set(const ItemStack& item);

    virtual void setChanged() { m_pContainer->setContainerChanged(m_stackId); }

    virtual int getMaxStackSize() const { return m_pContainer->getMaxStackSize(); }

    virtual ItemStack remove(int count) { return m_pContainer->removeItem(m_stackId, count); }

    virtual bool isAt(Container* cont, Container::StackID stackId) { return cont == m_pContainer && stackId == m_stackId; }

public:
    Container* m_pContainer;
    Container::SlotID m_id; // the position in the ContainerMenu::m_slots vector
    Container::StackID m_stackId; // the position in the Container::m_items vector
    Group m_group;
};