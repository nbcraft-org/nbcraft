#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "FillingContainer.hpp"
#include "common/Logger.hpp"
#include "nbt/CompoundTag.hpp"
#include "world/entity/Player.hpp"
#include "world/item/crafting/Recipes.hpp"

#define C_INVALID_SLOT UINT16_MAX
#define C_LINKED_SLOT_ID 255
#define C_LINKED_SLOT_COUNT 255

FillingContainer::FillingContainer(Size size, Size linkedSlotsCount, bool isUnlimited)
	: m_linkedSlots(nullptr)
	, m_size(size)
	, m_linkedSlotsCount(linkedSlotsCount)
	, m_bUnlimited(isUnlimited)
{
	if (linkedSlotsCount > 0)
	{
		m_linkedSlots = new StackID[m_linkedSlotsCount];
		std::fill(m_linkedSlots, &m_linkedSlots[m_linkedSlotsCount], C_INVALID_SLOT);
	}

	m_items.resize(size);
}

FillingContainer::~FillingContainer()
{
	_clear();

	if (m_linkedSlotsCount > 0)
		SAFE_DELETE_ARRAY(m_linkedSlots);
}

void FillingContainer::_release(StackID stackId)
{
	if (stackId == C_INVALID_SLOT)
		return;

	_getItem(stackId) = ItemStack::EMPTY;
}

void FillingContainer::_clear()
{
	for (size_t i = 0; i < m_linkedSlotsCount; i++)
	{
		m_linkedSlots[i] = i;
	}

	m_items.resize(m_size);
	std::fill(m_items.begin(), m_items.end(), ItemStack::EMPTY);
}

void FillingContainer::_swapSlots(StackID stackIdA, StackID stackIdB)
{
	std::swap(_getItem(stackIdA), _getItem(stackIdB));
}

ItemStack* FillingContainer::_tryGetLinked(int linkedIdx)
{
	StackID stackId = linkedIdx;

	if (linkedIdx >= 0 && linkedIdx <= m_linkedSlotsCount)
	{
		stackId = m_linkedSlots[linkedIdx];

		if (stackId >= m_linkedSlotsCount && stackId < m_size)
		{
			std::vector<ItemStack>& slotList = _getSlotList(stackId);

			return &slotList[stackId];
		}
	}
	
	return nullptr;
}

bool FillingContainer::_linkSlot(int key, StackID value, bool cycleSlots)
{
	if (key < 0)
		return false;
	if (key >= m_linkedSlotsCount)
		return false;
	if (m_linkedSlotsCount > value)
		return false;
	if (value >= this->m_size)
		return false;

	StackID& slotId = m_linkedSlots[key];
	if (slotId == value)
		return false;

	//LOG_I("shifting linkedSlots: %d, %d, %d, %d, %d, %d, %d, %d, %d", m_linkedSlots[0], m_linkedSlots[1], m_linkedSlots[2], m_linkedSlots[3], m_linkedSlots[4], m_linkedSlots[5], m_linkedSlots[6], m_linkedSlots[7], m_linkedSlots[8]);

	if (cycleSlots)
	{
		Size end = m_linkedSlotsCount - 1;
		if (end > 0 && m_linkedSlots[0] != value)
		{
			StackID id;
			for (id = 0; id < end && m_linkedSlots[id] != value; id++)
			{
			}

			// loop right-to-left and shift slots over
			if (key < id)
			{
				for (StackID i = id; i != key; i--)
				{
					m_linkedSlots[i] = m_linkedSlots[i - 1];
				}
			}
		}
	}

	slotId = value;

	//LOG_I("shifted linkedSlots: %d, %d, %d, %d, %d, %d, %d, %d, %d", m_linkedSlots[0], m_linkedSlots[1], m_linkedSlots[2], m_linkedSlots[3], m_linkedSlots[4], m_linkedSlots[5], m_linkedSlots[6], m_linkedSlots[7], m_linkedSlots[8]);

	return true;
}

void FillingContainer::_compressLinkedSlotList(Size toSize)
{
	// also unread, validate
	StackID i = toSize, j;
	for (j = 0; i < m_linkedSlotsCount; i++)
	{
		m_linkedSlots[i - j] = m_linkedSlots[i];
		if (!_tryGetLinked(i))
			++j;
	}

	StackID v6 = i - j;
	if (v6 < i)
	{
		std::fill(&m_linkedSlots[v6], &m_linkedSlots[i+1], C_INVALID_SLOT);
	}
}

bool FillingContainer::_linkEmptySlot(StackID stackId)
{
	if (m_linkedSlotsCount <= 0)
		return false;
	if (m_linkedSlots[0] == stackId)
		return true;

	for (int i = 0; i != m_linkedSlotsCount; i++)
	{
		if (m_linkedSlots[i] == stackId)
			return true;
	}

	int j = 0;
	while (_tryGetLinked(j))
	{
		j++;
		if (m_linkedSlotsCount <= j)
			return false;
	}

	m_linkedSlots[j] = stackId;
	return true;
}

void FillingContainer::_fixBackwardCompabilityItem(ItemStack& item) const
{
	if (item.getId() == Tile::treeTrunk->m_ID)
	{
		if (item.getAuxValue() == 0)
			item.setAuxValue(1);
	}
}

Container::StackID FillingContainer::_getSlot(int itemId) const
{
	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		const ItemStack& item = m_items[i];
		if (item.getId() == itemId)
			return i;
	}

	return C_INVALID_SLOT;
}

Container::StackID FillingContainer::_getSlot(int itemId, ItemStack::AuxValue auxValue) const
{
	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		const ItemStack& item = m_items[i];
		if (item.getId() == itemId && item.getAuxValue() == auxValue)
			return i;
	}

	return C_INVALID_SLOT;
}

Container::StackID FillingContainer::_getNonEmptySlot(int itemId) const
{
	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		const ItemStack& item = m_items[i];
		if (item.getId() == itemId && item.m_count > 0)
			return i;
	}

	return C_INVALID_SLOT;
}

Container::StackID FillingContainer::_getNonEmptySlot(int itemId, ItemStack::AuxValue auxValue) const
{
	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		const ItemStack& item = m_items[i];
		if (item.getId() == itemId && item.getAuxValue() == auxValue && item.m_count > 0)
			return i;
	}

	return C_INVALID_SLOT;
}

Container::StackID FillingContainer::_getFreeSlot() const
{
	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		const ItemStack& item = m_items[i];
		if (item.m_count == 0)
			return i;
	}

	return C_INVALID_SLOT;
}

Container::StackID FillingContainer::_getSlotWithRemainingSpace(const ItemStack& item) const
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		const ItemStack& slotItem = m_items[i];
		if (slotItem.getId() != item.getId())
			continue;

		if (!slotItem.isStackable())
			continue;

		if (slotItem.m_count >= getMaxStackSize())
			continue;

		if (!slotItem.isStackedByData())
			return i;
		else if (slotItem.getAuxValue() == item.getAuxValue())
			return i;
	}

	return C_INVALID_SLOT;
}

ItemStack& FillingContainer::_getItem(StackID stackId)
{
	std::vector<ItemStack>& slotList = _getSlotList(stackId);

	return slotList[stackId];
}

Container::Size FillingContainer::_addItem(ItemStack& item)
{
	Size currentSize = m_items.size();
	Size startIndex = m_linkedSlotsCount;

	if (startIndex < currentSize)
	{
		for (int i = startIndex; i < currentSize; i++)
		{
			if (m_items[i].isEmpty())
			{
				m_items[i] = item;
				return i;
			}
		}
	}

	Size targetSize = currentSize + 1;

	if (m_bUnlimited)
	{
		if (targetSize > m_size)
			m_size = targetSize;
	}
	else if (targetSize > m_size)
	{
		std::string itemName = item.getDescriptionId();

		LOG_E("Error@addItem: adding an item to an already full inventory: %s!", itemName.c_str());

		return 0;
	}

	m_items.push_back(item);

	return targetSize;
}

std::vector<ItemStack>& FillingContainer::_getSlotList(StackID& stackId)
{
	return m_items;
}

void FillingContainer::replaceSlot(StackID stackId, const ItemStack* pItemStack)
{
	if (pItemStack)
	{
		m_items[stackId] = *pItemStack;
	}
	else
	{
		_release(stackId);
	}
}

void FillingContainer::clearSlot(StackID index)
{
	if (index < 0)
		return;

	if (index < m_linkedSlotsCount && m_linkedSlots[index] != C_INVALID_SLOT)
	{
		_release(m_linkedSlots[index]);
		m_linkedSlots[index] = C_INVALID_SLOT;
	}
	else
	{
		_release(index);
	}

	_compressLinkedSlotList(index);
}

bool FillingContainer::hasResource(int itemId) const
{
	if (m_bUnlimited)
		return true;
	
	return _getSlot(itemId) != C_INVALID_SLOT;
}

bool FillingContainer::removeResource(int itemId)
{
	if (m_bUnlimited)
		return true;

	StackID stackId = _getSlot(itemId);
	if (stackId == C_INVALID_SLOT)
		return false;

	ItemStack& item = m_items[stackId];
	item.m_count -= 1;

	if (item.m_count <= 0)
		_release(stackId);

	return true;
}

bool FillingContainer::removeResource(const ItemStack& item)
{
	if (m_bUnlimited)
		return true;

	ItemStack::Count remainingCount = item.m_count;
	if (remainingCount <= 0)
		return true;

	int itemId = item.getId();
	ItemStack::AuxValue auxValue = item.getAuxValue();
	bool isAnyAux = Recipes::IsAnyAuxValue(itemId);

	while (remainingCount > 0)
	{
		StackID stackId;
		if (isAnyAux)
			stackId = _getNonEmptySlot(itemId);
		else
			stackId = _getNonEmptySlot(itemId, auxValue);

		if (stackId < 0)
			return false;

		ItemStack& slotItem = _getItem(stackId);

		ItemStack::Count takeCount = Mth::Min<ItemStack::Count>(remainingCount, slotItem.m_count);

		slotItem.m_count -= takeCount;
		remainingCount -= takeCount;

		if (slotItem.m_count <= 0)
		{
			clearSlot(stackId);
		}
	}

	return true;
}

ItemStack::Count FillingContainer::addResource(const ItemStack& item)
{
	ItemStack::Count remainingCount = item.m_count;

	if (item.getMaxStackSize() == 1)
	{
		StackID freeSlot = _getFreeSlot();
		if (freeSlot == C_INVALID_SLOT)
			return remainingCount;

		ItemStack& slotItem = m_items[freeSlot];

		// check if we were somehow given a non-empty slot
		if (!slotItem.isEmpty())
			return 0;

		slotItem = item;
		_linkEmptySlot(freeSlot);

		return 0;
	}

	StackID slotId = _getSlotWithRemainingSpace(item);
	if (slotId == C_INVALID_SLOT)
		slotId = _getFreeSlot();

	if (slotId == C_INVALID_SLOT)
		return remainingCount; // couldn't find an open slot

	ItemStack& slotItem = m_items[slotId];

	if (slotItem.isEmpty())
	{
		m_items[slotId] = ItemStack(item.getId(), 0, item.getAuxValue());
	}

	_linkEmptySlot(slotId);

	int maxStack = Mth::Min(slotItem.getMaxStackSize(), getMaxStackSize());
	int availableSpace = maxStack - slotItem.m_count;

	if (availableSpace <= 0)
		return remainingCount; // no space left with its peers

	ItemStack::Count countToAdd = Mth::Min<ItemStack::Count>(remainingCount, availableSpace);

	if (countToAdd > 0)
	{
		remainingCount -= countToAdd;
		slotItem.m_count += countToAdd;
	}

	return remainingCount;
}

void FillingContainer::dropSlot(StackID index, bool onlyClearContainer, bool b)
{
	if (index < m_linkedSlotsCount)
		return;

	StackID stackId = m_linkedSlots[index];
	if (stackId == C_INVALID_SLOT)
		return;

	ItemStack& slotItem = _getItem(stackId);
	if (slotItem.isEmpty())
		return;

	if (onlyClearContainer)
	{
		slotItem.m_count = 0;
	}
	else
	{
		doDrop(slotItem, b);
	}

	_release(stackId);
	_compressLinkedSlotList(stackId);
}

void FillingContainer::dropAll(bool onlyClearContainer)
{
	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		dropSlot(i, onlyClearContainer);
	}
}

void FillingContainer::replace(const std::vector<ItemStack>& items)
{
	_clear();

	Size minSize = Mth::Min<Size>(getContainerSize() - m_linkedSlotsCount, m_items.size());
	for (StackID i = 0; i < minSize; i++)
	{
		const ItemStack& slotItem = items[i];
		replaceSlot(i + m_linkedSlotsCount, slotItem.isEmpty() ? nullptr : &slotItem);
	}
}

void FillingContainer::save(ListTag& tag) const
{
	if (m_bUnlimited)
		return;

	for (size_t i = 0; i < m_items.size(); i++)
	{
		ItemStack item;

		if (i < m_linkedSlotsCount)
		{
			item = ItemStack(C_LINKED_SLOT_ID, C_LINKED_SLOT_COUNT, m_linkedSlots[i]);
		}
		else
		{
			const ItemStack& slotItem = m_items[i];

			if (slotItem.isEmpty())
				continue;

			item = slotItem;
		}

		// @NOTE: clamping to uint8_t just like in Java & PE
		item.m_count = Mth::clamp<ItemStack::Count>(item.m_count, 0, UINT8_MAX);

		CompoundTag* itemTag = new CompoundTag();
		itemTag->putInt8("Slot", i);

		item.save(*itemTag);
		tag.add(itemTag);
	}
}

void FillingContainer::load(const ListTag& tag)
{
	if (m_bUnlimited)
		return;

	_clear();

	size_t containerSize = m_items.size();

	const std::vector<Tag*>& itemTags = tag.rawView();

	for (std::vector<Tag*>::const_iterator it = itemTags.begin(); it != itemTags.end(); it++)
	{
		const Tag* pTag = *it;

		if (pTag->getId() != Tag::TAG_TYPE_COMPOUND)
			continue;

		const CompoundTag* itemTag = (const CompoundTag*)pTag;

		uint8_t slotId = UINT8_MAX; // if we don't know where it's going, write that shit out-of-bounds so it gets dropped "safely" on the ground
		if (itemTag->contains("Slot", Tag::TAG_TYPE_INT8))
		{
			slotId = itemTag->getInt8("Slot");
		}

		ItemStack item = ItemStack::fromTag(*itemTag);
		if (!item.isEmpty())
		{
			if (item.getId() == C_LINKED_SLOT_ID && item.m_count == C_LINKED_SLOT_COUNT)
			{
				if (slotId >= m_linkedSlotsCount)
				{
					//LOG_E("Error: Slot %d is selection slot (inventory size: %d) but id/count is %d/%d", slotId, containerSize, item.getId(), item.m_count);
					LOG_E("Error: Slot %d is selection slot (inventory size: %d) but is out-of-bounds", slotId, containerSize);
				}
				else
				{
					ItemStack::AuxValue auxValue = item.getAuxValue();

					if (auxValue >= m_linkedSlotsCount && auxValue < containerSize)
					{
						_linkSlot(slotId, auxValue, 0);
					}
				}
			}
			else if (slotId < m_items.size())
			{
				ItemStack& slotItem = m_items[slotId];
				slotItem = item;
				//_fixBackwardCompabilityItem(slotItem);
			}
			/*else if (slotId >= 100 && slotId < m_armor.size() + 100)
			{
				m_armor[slotId - 100] = item;
			}*/
			else
			{
				doDrop(item, true);
			}
		}
	}

	_compressLinkedSlotList(0);
}

bool FillingContainer::contains(const ItemStack& item) const
{
	std::vector<ItemStack>::const_iterator itr = std::find(m_items.begin(), m_items.end(), item);

	return itr != m_items.end();
}

Container::Size FillingContainer::getNumEmptySlots() const
{
	Size slotCount = 0;

	for (size_t i = m_linkedSlotsCount; i < m_items.size(); i++)
	{
		const ItemStack& item = m_items[i];
		if (item.isEmpty())
			slotCount++;
	}

	return slotCount;
}

ItemStack& FillingContainer::getItem(StackID index)
{
	if (index >= getContainerSize())
	{
		LOG_E("index is invalid for getItem: index=%d size=%d", index, getContainerSize());
		throw std::invalid_argument("index");
	}

	if (index < m_linkedSlotsCount)
	{
		StackID newIndex = m_linkedSlots[index];
		if (newIndex != C_INVALID_SLOT)
			index = newIndex;
	}

	return _getItem(index);
}

ItemStack FillingContainer::removeItem(StackID index, int count)
{
	// a mish-mash of 0.3.3's FillingContainer and our SimpleContainer's impl
	ItemStack& itemStack = getItem(index);
	if (!itemStack.isEmpty())
	{
		ItemStack result = ItemStack::EMPTY;
		if (itemStack.m_count <= count)
		{
			result = itemStack;
			clearSlot(index);
			setContainerChanged(index);
			return result;
		}
		else
		{
			result = itemStack.remove(count);
			if (!itemStack.m_count)
				clearSlot(index);

			setContainerChanged(index);
			return result;
		}
	}

	return ItemStack::EMPTY;
}

void FillingContainer::setItem(StackID index, const ItemStack& item)
{
	ItemStack& slotItem = getItem(index);
	slotItem = item;

	if (!item.isEmpty() && item.m_count > getMaxStackSize())
		slotItem.m_count = getMaxStackSize();

	setContainerChanged(index);
}

Container::Size FillingContainer::getContainerSize() const
{
	Size containerSize = m_size;
	Size slotCount = (Size)(m_items.size());
	if (containerSize != slotCount)
	{
		LOG_E("Error@getContainerSize: num items != InventorySize: %d != %d", slotCount, containerSize);
	}

	return containerSize;
}

int FillingContainer::getMaxStackSize() const
{
	return 254;
}

bool FillingContainer::stillValid(Player& player) const
{
	return !player.m_bRemoved;
}

bool FillingContainer::add(ItemStack& item)
{
	if (m_bUnlimited || item.isEmpty())
		return true;

	if (item.isDamaged())
	{
		SlotID freeSlot = _getFreeSlot();
		if (freeSlot < 0)
			return false;

		m_items[freeSlot] = item;
		_linkEmptySlot(freeSlot);

		item = ItemStack::EMPTY;

		return true;
	}

	// some fuck-ass shit
	ItemStack::Count count;
	for (ItemStack::Count i = 0; ; i = count)
	{
		count = addResource(item);
		item.m_count = count;
		if (count <= 0 || count >= i)
			return i > count;
	}
}

void FillingContainer::doDrop(ItemStack& item, bool b)
{
	item = ItemStack::EMPTY;
}
