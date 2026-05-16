#include "TileEntityType.hpp"
#include "FurnaceTileEntity.hpp"
#include "ChestTileEntity.hpp"
#include "MusicTileEntity.hpp"
// #include "MobSpawnerTileEntity.hpp"
// #include "DispenserTileEntity.hpp"
// #include "SignTileEntity.hpp"
// #include "RecordPlayerTileEntity.hpp"
// #include "PistonMovingTileEntity.hpp"

TileEntityType* TileEntityType::furnace;
TileEntityType* TileEntityType::chest;
TileEntityType* TileEntityType::noteblock;

std::map<std::string, TileEntityType*> TileEntityFactory::_types;

void TileEntityFactory::initTileEntities()
{
	TileEntityType::furnace =       registerTileEntity<FurnaceTileEntity>("Furnace");
	TileEntityType::chest =         registerTileEntity<ChestTileEntity>("Chest");
    TileEntityType::noteblock =     registerTileEntity<MusicTileEntity>("Music");
}

void TileEntityFactory::teardownTileEntities()
{
    // delete all heap allocated tile entity types (furnace, chest, etc.)
    for (std::map<std::string, TileEntityType*>::const_iterator it = _types.begin(); it != _types.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
	_types.clear();
}
