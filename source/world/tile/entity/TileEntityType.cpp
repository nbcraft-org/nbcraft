#include "TileEntityType.hpp"
#include "FurnaceTileEntity.hpp"
#include "ChestTileEntity.hpp"
#include "MusicTileEntity.hpp"
#include "MobSpawnerTileEntity.hpp"
#include "DispenserTileEntity.hpp"
#include "SignTileEntity.hpp"
#include "RecordPlayerTileEntity.hpp"
#include "PistonMovingTileEntity.hpp"

TileEntityType* TileEntityType::furnace;
TileEntityType* TileEntityType::chest;
TileEntityType* TileEntityType::noteblock;
TileEntityType* TileEntityType::dispenser;
TileEntityType* TileEntityType::mobSpawner;
TileEntityType* TileEntityType::sign;
TileEntityType* TileEntityType::recordPlayer;
TileEntityType* TileEntityType::piston;

HashMap<std::string, TileEntityType*> TileEntityFactory::_types;

void TileEntityFactory::initTileEntities()
{
	TileEntityType::furnace =       registerTileEntity<FurnaceTileEntity>("Furnace");
	TileEntityType::chest =         registerTileEntity<ChestTileEntity>("Chest");
    TileEntityType::noteblock =     registerTileEntity<MusicTileEntity>("Music");
    TileEntityType::dispenser =     registerTileEntity<DispenserTileEntity>("Trap");
    TileEntityType::mobSpawner =    registerTileEntity<MobSpawnerTileEntity>("MobSpawner");
    TileEntityType::sign =          registerTileEntity<SignTileEntity>("Sign");
    TileEntityType::recordPlayer =  registerTileEntity<RecordPlayerTileEntity>("RecordPlayer");
    TileEntityType::piston =        registerTileEntity<PistonMovingTileEntity>("Piston");
}

void TileEntityFactory::teardownTileEntities()
{
    // delete all heap allocated tile entity types (furnace, chest, etc.)
    for (HashMap<std::string, TileEntityType*>::iterator it = _types.begin(); it != _types.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
	_types.clear();
}
