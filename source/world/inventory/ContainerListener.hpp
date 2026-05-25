#pragma once

#include <vector>
#include "Container.hpp"

class ContainerMenu;
class ItemStack;
class Slot;

class ContainerListener
{
public:
    virtual ~ContainerListener();

    virtual void refreshContainer(ContainerMenu* menu, const std::vector<ItemStack>& items) {}
    virtual void refreshContainerItems(ContainerMenu* menu);
    virtual void slotChanged(ContainerMenu* menu, Container::SlotID slotId, Slot* slot, ItemStack& item, bool isResultSlot) {}
    virtual void setContainerData(ContainerMenu* menu, int id, int value) {}
};