#include "RecordPlayerTileEntity.hpp"

RecordPlayerTileEntity::RecordPlayerTileEntity() 
    : m_record(0)
{
    m_pType = TileEntityType::recordPlayer;
}

void RecordPlayerTileEntity::load(const CompoundTag& tag)
{
    TileEntity::load(tag);
    m_record = Mth::Max(0, tag.getInt8("Record"));
}

void RecordPlayerTileEntity::save(CompoundTag& tag) const
{
    TileEntity::save(tag);
    tag.putInt8("Record", m_record);
}
