#include "LightUpdate.hpp"
#include "common/Logger.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/levelgen/chunk/ChunkConstants.hpp"

void LightUpdate::update()
{
	if (!m_pSource)
	{
		LOG_E("LightUpdate doesn't have TileSource!");
		assert(false);
		return;
	}

	// clamp y values to 0-127 for min and max
	if (m_min.y < 0)
		m_min.y = 0;
	if (m_max.y > (ChunkConstants::Y_SIZE - 1))
		m_max.y = ChunkConstants::Y_SIZE - 1;

	TilePos pos;
	for (pos.x = m_min.x; pos.x <= m_max.x; pos.x++)
	{
		for (pos.z = m_min.z; pos.z <= m_max.z; pos.z++)
		{
			pos.y = m_min.y;
			if (!m_pSource->hasChunksAt(pos, 1))
				continue;

			for (; pos.y <= m_max.y; pos.y++)
			{
				Brightness_t currentBrightness = m_pSource->getBrightness(*m_pLightLayer, pos);

				TileID currentTile = m_pSource->getTile(pos);
				Brightness_t lightBlockLevel = std::min(Tile::lightBlock[currentTile], static_cast<Brightness_t>(1));

				Brightness_t maxBrightness = Brightness::MIN;
				if (m_pLightLayer == &LightLayer::Sky)
				{
					if (m_pSource->canSeeSky(pos))
						maxBrightness = Brightness::MAX;
				}
				else if (m_pLightLayer == &LightLayer::Block)
				{
					maxBrightness = Tile::lightEmission[currentTile];
				}

				Brightness_t newBrightness;
				if (lightBlockLevel < Brightness::MAX || maxBrightness != Brightness::MIN)
				{
					Brightness_t brightestNeighbor = 0;
					for (int i = 0; i < Facing::COUNT; i++)
					{
						Brightness_t neighborBrightness = m_pSource->getBrightness(*m_pLightLayer, pos + Facing::DIRECTION[i]);
                        if (neighborBrightness > brightestNeighbor)
							brightestNeighbor = neighborBrightness;
					}

					Brightness_t b = std::min(static_cast<Brightness_t>(brightestNeighbor - lightBlockLevel), Brightness::MAX);
					newBrightness = std::min(b, maxBrightness);
				}
				else
				{
					newBrightness = Brightness::MIN;
				}

				if (currentBrightness != newBrightness)
				{
					m_pSource->setBrightness(*m_pLightLayer, pos, newBrightness);

					Brightness_t spreadBrightness = std::max(static_cast<Brightness_t>(newBrightness - 1), Brightness::MIN);
                    
					// why dont we check if we're in range here?
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.west(),  spreadBrightness);
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.below(), spreadBrightness);
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.north(), spreadBrightness);

					if ((pos.x + 1) <= m_max.x)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.east(),  spreadBrightness);
					if ((pos.y + 1) <= m_max.y)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.above(), spreadBrightness);
					if ((pos.z + 1) <= m_max.z)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.south(), spreadBrightness);
				}
			}
		}
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
	if (lowerPos >= m_min && upperPos <= m_max)
		return true;

	if (lowerPos < (m_min - 1) || upperPos > (m_max + 1))
		return false;

	TilePos newMin = m_min.min(lowerPos);
	TilePos newMax = m_max.max(upperPos);

    // @TODO: this logic is just completely busted and results in way too many LightingUpdates
	//if (Mth::abs((upperPos - lowerPos).volume() - (newMax - newMin).volume()) > 2)
	//	return false;
    
    // If trying to add more than 2 tiles, we can't do that
    if ((upperPos.z - lowerPos.z) * (upperPos.x - lowerPos.x) * (upperPos.y - lowerPos.y) - (m_max.z - m_min.z) * (m_max.x - m_min.x) * (m_max.y - m_min.y) > 2)
        return false;

	m_min = newMin;
	m_max = newMax;
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