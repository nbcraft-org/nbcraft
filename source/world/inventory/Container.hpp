#pragma once

#include <set>
#include <vector>

#include "world/item/ItemStack.hpp"

#define C_MAX_CONTAINER_STACK_SIZE (64)

class ContainerContentChangeListener;
class ContainerSizeChangeListener;

class Container
{
protected:
	typedef std::set<ContainerContentChangeListener*> ContentChangeListeners;
	typedef std::set<ContainerSizeChangeListener*> SizeChangeListeners;

public:
	typedef uint16_t Size;
	typedef int16_t SlotID;
	typedef uint16_t StackID;

public:
	enum Type
	{
		CONTAINER,
		CRAFTING,
		FURNACE,
		DISPENSER
	};

public:
	virtual ~Container() {}

public:
	virtual Size getContainerSize() const = 0;
	virtual ItemStack& getItem(StackID stackId) = 0;
	virtual ItemStack* tryGetItem(StackID stackId)
	{
		if (stackId >= 0 && stackId < getContainerSize())
			return &getItem(stackId);
		else
			return nullptr;
	}
	virtual ItemStack removeItem(StackID stackId, int count) = 0;
	virtual void setItem(StackID stackId, const ItemStack& item) = 0;
	virtual std::string getName() const = 0;
	virtual int getMaxStackSize()
	{
		return C_MAX_CONTAINER_STACK_SIZE;
	}
	// Was called setChanged in Java
	virtual void setContainerChanged(StackID stackId) = 0;
	virtual bool stillValid(Player* player) const = 0;
	virtual std::vector<ItemStack> getSlotCopies();

	virtual void addContentChangeListener(ContainerContentChangeListener* listener) {}
	virtual void addSizeChangeListener(ContainerSizeChangeListener* listener) {}
	virtual void removeContentChangeListener(ContainerContentChangeListener* listener) {}
	virtual void removeSizeChangeListener(ContainerSizeChangeListener* listener) {}
};
