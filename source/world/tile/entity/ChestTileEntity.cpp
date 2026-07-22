#include "ChestTileEntity.hpp"
#include "world/level/TileSource.hpp"

ChestTileEntity::ChestTileEntity() : SimpleContainer(27, "Chest")
{
    m_pType = TileEntityType::chest;
}

void ChestTileEntity::load(const CompoundTag& tag)
{
    TileEntity::load(tag);
    SimpleContainer::load(tag);
}

void ChestTileEntity::save(CompoundTag& tag) const
{
    TileEntity::save(tag);
    SimpleContainer::save(tag);
}

bool ChestTileEntity::stillValid(Player& player) const
{
    TileSource& source = player.getTileSource();
    
    if (source.getTileEntity(m_pos) != this)
        return false;

    return player.distanceToSqr(m_pos + 0.5f) <= 64.0f;
}

void ChestTileEntity::setContainerChanged(StackID stackId)
{
    SimpleContainer::setContainerChanged(stackId);
    TileEntity::setChanged();
}
