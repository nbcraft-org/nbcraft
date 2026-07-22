#include "FenceGateTile.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/Level.hpp"

FenceGateTile::FenceGateTile(int a, int b) : Tile(a, b, Material::wood)
{
}

bool FenceGateTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
	TilePos below = pos.below();
	return source.getTile(below) == m_ID || (Tile::mayPlace(source, pos) && source.getMaterial(below)->isSolid());
}

AABB* FenceGateTile::getAABB(const TileSource& source, const TilePos& pos)
{
    TileData data = source.getData(pos);
    if (isOpen(data))
    {
        return nullptr;
    }
	AABB* rAABB = Tile::getAABB(source, pos);
	rAABB->max.y += 0.5f;
	return rAABB;
}

bool FenceGateTile::isSolidRender() const
{
	return false;
}

bool FenceGateTile::isCubeShaped() const
{
	return false;
}

eRenderShape FenceGateTile::getRenderShape() const
{
	return SHAPE_FENCE_GATE;
}

void FenceGateTile::setPlacedBy(const TilePos& pos, Mob& mob) 
{
    TileSource& source = mob.getTileSource();

	int i = (Mth::floor(mob.m_rot.yaw * 4.0f / 360.0f + 0.5) & 3) % 4;
	source.setTileAndData(pos, FullTile(this, i));
}

bool FenceGateTile::use(const TilePos& pos, Player& player)
{
    TileSource& source = player.getTileSource();

    setOpen(source, pos, !isOpen(source.getData(pos)), &player);
    return true;
}

void FenceGateTile::setOpen(TileSource& source, const TilePos& pos, bool bOpen, Player* player)
{
    int data = source.getData(pos);

    if (isOpen(data) == bOpen)
        return;

    if (!bOpen)
    {
        source.setTileAndData(pos, FullTile(this, data & C_CLOSE_MASK));
    }
    else
    {
        int j = 1;

        if (player)
            j = (Mth::floor(player->m_rot.yaw * 4.0f / 360.0f + 0.5) & 3) % 4;

        int k = getDir(data);
        if (k == (j + 2) % 4)
            data = j;

        source.setTileAndData(pos, FullTile(this, data | C_OPEN_BIT));
    }

    Level& level = source.getLevel();

    level.levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0, player));
}

void FenceGateTile::neighborChanged(TileSource& source, const TilePos& pos, TileID newTile)
{
	if (newTile > 0 && Tile::tiles[newTile]->isSignalSource())
	{
		bool bOpen = false;
		if (source.hasNeighborSignal(pos) || source.hasNeighborSignal(pos.above()))
			bOpen = true;

		setOpen(source, pos, bOpen);
	}
}
