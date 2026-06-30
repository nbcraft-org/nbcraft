#pragma once

#include <vector>
#include <set>
#include "world/item/ItemStack.hpp"
#include "client/player/input/MouseDevice.hpp"
#include "Container.hpp"
#include "ContainerContentChangeListener.hpp"

class Player;
class Inventory;
class Slot;
class ContainerListener;

class ContainerMenu : public ContainerContentChangeListener
{
protected:
    typedef std::set<ContainerListener*> ContainerListeners;

public:
    ContainerMenu(Container::Type containerType);
    virtual ~ContainerMenu();

protected:
    void _clearSlots();

public:
    void addSlot(Slot* slot);
    virtual void addSlotListener(ContainerListener* listener);
    void sendData(int id, int value);
    virtual void broadcastChanges(Container::SlotID slotId);
    virtual void broadcastChanges();
    virtual void removed(Player* player);
    virtual void slotsChanged(Container* container);

    // Called getItems in PE and Java
    std::vector<ItemStack> cloneItems(bool all = false);
    Slot* getSlotFor(Container* container, Container::StackID stackId);
    Slot* getSlot(Container::SlotID slotId) { return m_slots[slotId]; }
    virtual ItemStack clicked(Container::SlotID slotId, MouseButtonType mouseButton, bool quickMove, Player* player);
    virtual ItemStack quickMoveStack(Container::SlotID slotId);
    virtual void moveItemStackTo(ItemStack& item, Container::SlotID slotFrom, Container::SlotID slotTo, bool take);

    void setItem(Container::SlotID slotId, ItemStack item);
    void setAll(const std::vector<ItemStack>& items);
    virtual void setData(int id, int value);

    uint16_t backup(Inventory* inventory);
    void deleteBackup(uint16_t id);
    void rollbackToBackup(uint16_t id);

    bool isSynched(Player* player) const;
    void setSynched(Player* player, bool isSynched);

public:
    virtual bool isResultSlot() const { return false; } // PE
    virtual bool stillValid(Player* player) const = 0;
    //Unused
    virtual bool isPauseScreen() const { return false; }

public:
    void containerContentChanged(Container* container, Container::StackID stackId) override;

protected:
    std::vector<ItemStack> m_lastSlots;
    uint16_t m_changeUid;
    ContainerListeners m_listeners;
    std::set<Player*> m_unsynchedPlayers;

public:
    int m_containerId;
    Container::Type m_containerType;
    std::vector<Slot*> m_slots;
    bool m_bBroadcastChanges;
};
