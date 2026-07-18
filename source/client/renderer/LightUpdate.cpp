#include "LightUpdate.hpp"
#include "common/Logger.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/levelgen/chunk/ChunkConstants.hpp"
#include "world/level/levelgen/chunk/LevelChunk.hpp"

void LightUpdate::update()
{
    if (!m_pSource)
        return;

	LOG_I("min: (%d, %d, %d) max: (%d, %d, %d)", m_min.x, m_min.y, m_min.z, m_max.x, m_max.y, m_max.z);
    
	int newBr, oldBr, newBrN, x, z, x7, x14, x13, x10, v24, x21, x17_1, v27, x9, x10_1, x8, x7_1, x3, x4, x1, x20;
	int x19, x18, x17, x16, x5, x1_1;
	bool x11;
    
	if ((m_max.z - m_min.z + 1) * (m_max.x + 1 - m_min.x + (m_max.y - m_min.y) * (m_max.x + 1 - m_min.x)) > 32768)
		return;
    
	if (m_max.x < m_min.x)
		return;
    
	x1 = m_min.x + 1;
	for (int i = m_min.x - 1; ; ++i)
	{
		x = x1 - 1;
		if (m_max.z < m_min.z)
		{
			x1_1 = x1;
			goto LABEL_53;
		}
		x1_1 = x1;
		x3 = m_min.z + 1;
		x4 = m_min.z - 1;
		do
		{
			z = x3 - 1;
			if (!m_pSource->hasChunksAt(TilePos(x, 0, x3 - 1), 1)
				|| m_pSource->getChunk(TilePos(x, 0, z))->isEmpty())
			{
				x5 = x3;
			}
			else
			{
				if (m_min.y < 0)   m_min.y = 0;
				if (m_max.y > 127) m_max.y = 127;
                
				if (m_min.y <= m_max.y)
				{
					x7 = m_min.y + 1;
					x8 = m_min.y - 1;
					x5 = x3;
					while (1)
					{
						oldBr = m_pSource->getBrightness(*this->m_pLightLayer, TilePos(x, x7 - 1, z));
						x13 = m_pSource->getTile(TilePos(x, x7 - 1, z));
						x14 = Tile::lightBlock[x13];
						if (!x14)
							x14 = 1;
						if (m_pLightLayer == &LightLayer::Sky)
							break;
						if (m_pLightLayer != &LightLayer::Block)
							goto LABEL_30;
						x10 = Tile::lightEmission[x13];
						x11 = x10 == 0;
					LABEL_31:
						if (x14 > 14)
							v24 = x11;
						else
							v24 = 0;
						if (!v24)
						{
						LABEL_35:
							x10_1 = x10;
							x16 = m_pSource->getBrightness(*m_pLightLayer, TilePos(i, x7 - 1, z));
							x17 = m_pSource->getBrightness(*m_pLightLayer, TilePos(x1, x7 - 1, z));
							x7_1 = x7;
							x18 = m_pSource->getBrightness(*m_pLightLayer, TilePos(x, x8, z));
							x19 = m_pSource->getBrightness(*m_pLightLayer, TilePos(x, x7, z));
							x20 = m_pSource->getBrightness(*m_pLightLayer, TilePos(x, x7 - 1, x4));
							x21 = m_pSource->getBrightness(*m_pLightLayer, TilePos(x, x7 - 1, x3));
							x17_1 = x17;
							if (x17 < x16)
								x17_1 = x16;
							if (x17_1 < x18)
								x17_1 = x18;
							if (x17_1 < x19)
								x17_1 = x19;
							if (x17_1 < x20)
								x17_1 = x20;
							if (x17_1 < x21)
								v27 = x21 - x14;
							else
								v27 = x17_1 - x14;
							newBr = v27;
							if (newBr < 0)
								newBr = 0;
							if (newBr < x10_1)
								newBr = x10_1;
							goto LABEL_18;
						}
						newBr = 0;
						x7_1 = x7;
					LABEL_18:
						if (newBr != oldBr)
						{
							m_pSource->setBrightness(*m_pLightLayer, TilePos(x, x7 - 1, z), newBr);
							newBrN = newBr - 1;
							if (newBrN < 0)
								newBrN = 0;
							m_pSource->updateLightIfOtherThan(*m_pLightLayer, TilePos(i, x7 - 1, z), newBrN);
							m_pSource->updateLightIfOtherThan(*m_pLightLayer, TilePos(x, x8, z), newBrN);
							m_pSource->updateLightIfOtherThan(*m_pLightLayer, TilePos(x, x7 - 1, x4), newBrN);
							if (m_max.x <= x1)
								m_pSource->updateLightIfOtherThan(*m_pLightLayer, TilePos(x1, x7 - 1, z), newBrN);
							if (m_max.y <= x7)
								m_pSource->updateLightIfOtherThan(*m_pLightLayer, TilePos(x, x7, z), newBrN);
							if (m_max.z <= x3)
								m_pSource->updateLightIfOtherThan(*m_pLightLayer, TilePos(x, x7 - 1, x3), newBrN);
						}
						++x7;
						++x8;
						if (m_max.y < x7_1)
							goto LABEL_8;
					}
					x9 = m_pSource->isSkyLit(TilePos(x, x7 - 1, z));
					x10 = 15;
					if (x9)
						goto LABEL_35;
				LABEL_30:
					x11 = 1;
					x10 = 0;
					goto LABEL_31;
				}
				x5 = x3;
			}
		LABEL_8:
			++x3;
			++x4;
		} while (x5 <= m_max.z);
	LABEL_53:
		++x1;
		if (x1_1 > m_max.x)
			break;
	}
}

void LightUpdate::updateFast()
{
	if (!m_pSource)
	{
		LOG_E("LightUpdate doesn't have TileSource!");
		assert(false);
		return;
	}

	// TODO!!! currently use normal update
	update();
}

bool LightUpdate::expandIfCloseEnough(const TilePos& lowerPos, const TilePos& upperPos)
{
	if (m_min <= lowerPos && m_max >= upperPos)
		return true;
    
	if (lowerPos < m_min - 1) return false;
	if (upperPos > m_max + 1) return false;
    
	TilePos lp(lowerPos), up(upperPos);
	if (lp.y >= m_min.y) lp.y = m_min.y;
	if (lp.x >= m_min.x) lp.x = m_min.x;
	if (up.y < m_max.y)  up.y = m_max.y;
	if (lp.z >= m_min.z) lp.z = m_min.z;
	if (up.x < m_max.x)  up.x = m_max.x;
	if (up.z < m_max.z)  up.z = m_max.z;
    
	// If trying to add more than 2 tiles, we can't do that
	if ((up.z - lp.z) * (up.x - lp.x) * (up.y - lp.y) - (m_max.z - m_min.z) * (m_max.x - m_min.x) * (m_max.y - m_min.y) > 2)
		return false;
    
	m_min = lp;
	m_max = up;
    
	return true;
}

void LightUpdate::expandToContain(const TilePos& pos)
{
	m_min = m_min.min(pos);
	m_max = m_max.max(pos);
}

void LightUpdate::expandToContain(const TilePos& pos1, const TilePos& pos2)
{
	expandToContain(pos1);
	expandToContain(pos2);
}