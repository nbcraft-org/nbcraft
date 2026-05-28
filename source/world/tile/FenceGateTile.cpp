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
	int i = (Mth::floor(mob->m_rot.x * 4.0f / 360.0f + 0.5) & 3) % 4;
	pLevel->setData(pos, i);
}

bool FenceGateTile::use(Level* pLevel, const TilePos& pos, Player* player)
{
    int i = pLevel->getData(pos);
    if (isOpen(i)) {
        pLevel->setData(pos, i & C_CLOSE_MASK);
    }
    else {
        int j = (Mth::floor(player->m_rot.x * 4.0f / 360.0f + 0.5) & 3) % 4;
        int k = getDir(i);
        if (k == (j + 2) % 4) {
            i = j;
        }

        pLevel->setData(pos, i | C_OPEN_BIT);
    }

    pLevel->levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0, player));
    return true;
}

int FenceGateTile::getDir(TileData data) const
{
    return data & C_DIR_MASK;
}
