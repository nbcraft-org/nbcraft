#include "FenceGateTile.hpp"
#include "world/level/Level.hpp"

FenceGateTile::FenceGateTile(int a, int b) : Tile(a, b, Material::wood)
{
}

bool FenceGateTile::mayPlace(const Level* level, const TilePos& pos) const
{
	TilePos below = pos.below();
	return level->getTile(below) == m_ID || (Tile::mayPlace(level, pos) && level->getMaterial(below)->isSolid());
}

AABB* FenceGateTile::getAABB(const Level* pLevel, const TilePos& pos)
{
    int data = pLevel->getData(pos);
    if (isOpen(data))
    {
        return NULL;
    }
	AABB* rAABB = Tile::getAABB(pLevel, pos);
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

void FenceGateTile::setPlacedBy(Level* pLevel, const TilePos& pos, Mob* mob) 
{
	int i = (Mth::floor(mob->m_rot.yaw * 4.0f / 360.0f + 0.5) & 3) % 4;
	pLevel->setData(pos, i);
}

bool FenceGateTile::use(Level* pLevel, const TilePos& pos, Player* player)
{
    setOpen(pLevel, pos, !isOpen(pLevel->getData(pos)), player);
    return true;
}

void FenceGateTile::setOpen(Level* pLevel, const TilePos& pos, bool bOpen, Player* player)
{
    int data = pLevel->getData(pos);

    if (isOpen(data) == bOpen)
        return;

    if (!bOpen)
    {
        pLevel->setData(pos, data & C_CLOSE_MASK);
    }
    else
    {
        int j = 1;

        if (player)
            j = (Mth::floor(player->m_rot.yaw * 4.0f / 360.0f + 0.5) & 3) % 4;

        int k = getDir(data);
        if (k == (j + 2) % 4)
            data = j;

        pLevel->setData(pos, data | C_OPEN_BIT);
    }

    pLevel->levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0, player));
}

void FenceGateTile::neighborChanged(Level* level, const TilePos& pos, TileID newTile)
{
	if (newTile > 0 && Tile::tiles[newTile]->isSignalSource())
	{
		bool bOpen = false;
		if (level->hasNeighborSignal(pos) || level->hasNeighborSignal(pos.above()))
			bOpen = true;

		setOpen(level, pos, bOpen);
	}
}
