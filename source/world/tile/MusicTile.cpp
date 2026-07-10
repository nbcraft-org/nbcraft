#include "MusicTile.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"
#include "world/entity/Player.hpp"
#include "entity/MusicTileEntity.hpp"

MusicTile::MusicTile(TileID id, int texture) : EntityTile(id, texture, Material::wood)
{
}

void MusicTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
    if (tile <= TILE_AIR || !Tile::tiles[tile]->isSignalSource())
        return;

    TileEntity* tileEnt = source.getTileEntity(pos);
    if (!tileEnt)
        return;

    bool signalProvided = source.hasDirectSignal(pos);
    MusicTileEntity* musEnt = static_cast<MusicTileEntity*>(tileEnt);
    if (musEnt->m_bOn == signalProvided)
        return;

    if (signalProvided)
        musEnt->play(source, pos);

    musEnt->m_bOn = signalProvided;
}

bool MusicTile::use(const TilePos& pos, Player& player)
{
	if (player.isSneaking() && !player.getSelectedItem().isEmpty())
		return false;

    Level& level = player.getLevel();
    TileSource& source = player.getTileSource();

    if (level.m_bIsClientSide)
        return true;

    TileEntity* tileEnt = source.getTileEntity(pos);
    if (!tileEnt)
        return false;

    MusicTileEntity* musEnt = static_cast<MusicTileEntity*>(tileEnt);

    musEnt->tune();
    musEnt->play(source, pos);
    return true;
}

void MusicTile::attack(const TilePos& pos, Player& player)
{
    Level& level = player.getLevel();

    if (level.m_bIsClientSide)
        return;

    TileSource& source = player.getTileSource();

    TileEntity* tileEnt = source.getTileEntity(pos);
    if (!tileEnt)
        return;

    (static_cast<MusicTileEntity*>(tileEnt))->play(source, pos);
}

bool MusicTile::hasTileEntity() const
{
	return true;
}

TileEntity* MusicTile::newTileEntity()
{
	return new MusicTileEntity();
}

void MusicTile::triggerEvent(TileSource& source, const TileEvent& event)
{
    int instrument = event.b0;
    int note = event.b1;
    
    std::string soundType;
    switch (instrument)
    {
    default:
        soundType = "harp";
        break;
    case 1:
        soundType = "bd";
        break;
    case 2:
        soundType = "snare";
        break;
    case 3:
        soundType = "hat";
        break;
    case 4:
        soundType = "bassattack";
        break;
    }

    Level& level = source.getLevel();

    level.playSound(event.pos + 0.5f, "note." + soundType, 3.0f, std::pow(2.0f, (note - 12) / 12.0f));
    level.addParticle("note", Vec3(event.pos.x + 0.5f, event.pos.y + 1.2f, event.pos.z + 0.5f), Vec3(note / 24.0f, 0.0f, 0.0f));
}
