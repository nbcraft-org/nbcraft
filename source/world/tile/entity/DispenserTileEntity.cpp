#include "DispenserTileEntity.hpp"

DispenserTileEntity::DispenserTileEntity() : SimpleContainer(9, "Dispenser")
{
    m_pType = TileEntityType::dispenser;
}

ItemStack DispenserTileEntity::removeRandomItem()
{
	int var1 = -1;
	int var2 = 1;

	for (size_t var3 = 0; var3 < m_items.size(); ++var3)
	{
		if (!m_items[var3].isEmpty() && m_random.nextInt(var2) == 0)
		{
			var1 = var3;
			++var2;
		}
	}

	if (var1 >= 0)
	{
		return removeItem(var1, 1);
	}
	else
	{
		return ItemStack::EMPTY;
	}
}

void DispenserTileEntity::load(const CompoundTag& tag)
{
    TileEntity::load(tag);
    SimpleContainer::load(tag);
}

void DispenserTileEntity::save(CompoundTag& tag) const
{
    TileEntity::save(tag);
    SimpleContainer::save(tag);
}

bool DispenserTileEntity::stillValid(Player& player) const
{
    if (m_pLevel->getTileEntity(m_pos) != this)
        return false;

    return player->distanceToSqr(m_pos + 0.5f) <= 64.0f;
}

void DispenserTileEntity::setContainerChanged(StackID stackId)
{
    SimpleContainer::setContainerChanged(stackId);
    TileEntity::setChanged();
}
