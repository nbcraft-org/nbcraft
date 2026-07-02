#include "Mushroom.hpp"
#include "world/level/Level.hpp"

Mushroom::Mushroom(TileID id, int texture) 
	: Bush(id, texture)
{
	setShape(0.3F, 0.0F, 0.3F, 0.7F, 0.4F, 0.7F);
	setTicking(true);
}

void Mushroom::tick(Level* pLevel, const TilePos& pos, Random* pRandom)
{
	if (pRandom->nextInt(100) == 0)
	{
		TilePos tp = pos;
		tp.x += pRandom->nextInt(3) - 1;
		tp.y += pRandom->nextInt(2) - pRandom->nextInt(2);
		tp.z += pRandom->nextInt(3) - 1;
		if (pLevel->isEmptyTile(pos) && canSurvive(pLevel, tp))
		{
			//@NOTE: Useless random calls, their result isn't used for anything in the original
			pRandom->nextInt(3);
			pRandom->nextInt(3);
			pLevel->setTile(tp, m_ID);
		}
	}
}

bool Mushroom::canSurvive(const Level* level, const TilePos& pos) const
{
	return pos.y >= C_MIN_Y && pos.y < C_MAX_Y && level->getRawBrightness(pos) < 13 && isValidGrowTile(level->getTile(pos.below()));
}

bool Mushroom::isValidGrowTile(const TileID tile) const
{
	return Tile::solid[tile];
}
