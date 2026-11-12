#pragma once

#include <vector>

#include "BaseContainer.hpp"
#include "nbt/ListTag.hpp"

class FillingContainer : public BaseContainer
{
public:
	FillingContainer(Size size, Size linkedSlotsCount, bool isUnlimited);
	virtual ~FillingContainer();

protected:
	void _release(StackID stackId);
	void _clear();
	void _swapSlots(StackID stackIdA, StackID stackIdB);
	ItemStack* _tryGetLinked(int linkedIdx);
	bool _linkSlot(int key, StackID value, bool cycleSlots = false);
	void _compressLinkedSlotList(Size toSize);
	bool _linkEmptySlot(StackID stackId);
	void _fixBackwardCompabilityItem(ItemStack& item) const;
	StackID _getSlot(int itemId) const;
	StackID _getSlot(int itemId, ItemStack::AuxValue auxValue) const;
	StackID _getNonEmptySlot(int itemId) const;
	StackID _getNonEmptySlot(int itemId, ItemStack::AuxValue auxValue) const;
	StackID _getFreeSlot() const;
	StackID _getSlotWithRemainingSpace(const ItemStack& item) const;
	ItemStack& _getItem(StackID stackId);
	Size _addItem(ItemStack& item);

protected:
	virtual std::vector<ItemStack>& _getSlotList(StackID& stackId);

public:
	void replaceSlot(StackID stackId, const ItemStack* pItemStack);
	void clearSlot(StackID index);
	bool hasResource(int itemId) const;
	bool removeResource(int itemId);
	bool removeResource(const ItemStack& item);
	ItemStack::Count addResource(const ItemStack& item);
	void dropSlot(StackID index, bool onlyClearContainer = false, bool b = false);
	void dropAll(bool onlyClearContainer = false);
	void replace(const std::vector<ItemStack>& items);
	void save(ListTag& tag) const;
	void load(const ListTag& tag);
	bool contains(const ItemStack& item) const;
	StackID getFirstSlot() const { return m_linkedSlotsCount; }
	Size getNumEmptySlots() const;
	Size getNumLinkedSlots() const { return m_linkedSlotsCount; }

public:
	ItemStack& getItem(StackID index) override;
	ItemStack removeItem(StackID index, int count) override;
	void setItem(StackID index, const ItemStack& item) override;
	Size getContainerSize() const override;
	int getMaxStackSize() const override;
	bool stillValid(Player& player) const override;

public:
	virtual bool add(ItemStack& item);
	virtual void doDrop(ItemStack& item, bool b);

protected:
	StackID* m_linkedSlots;
	std::vector<ItemStack> m_items;
	Size m_size;
	Size m_linkedSlotsCount;
	bool m_bUnlimited;
};

