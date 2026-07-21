#include "LightUpdate.hpp"
#include "common/Logger.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/levelgen/chunk/ChunkConstants.hpp"
#include "world/level/levelgen/chunk/LevelChunk.hpp"

void LightUpdate::update()
{
	if (!m_pSource)
	{
		LOG_E("LightUpdate doesn't have TileSource!");
		assert(false);
		return;
	}

	//LOG_I("min: (%d, %d, %d) max: (%d, %d, %d)", m_min.x, m_min.y, m_min.z, m_max.x, m_max.y, m_max.z);

	// From Java/early PE, removed in 0.12.1, useless math imo
	/*if ((m_max.z - m_min.z + 1) * (m_max.x + 1 - m_min.x + (m_max.y - m_min.y) * (m_max.x + 1 - m_min.x)) > 32768)
	{
		LOG_W("Light too large, skipping!");
		return;
	}*/

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

			LevelChunk* pChunk = m_pSource->getChunk(pos);
			if (!pChunk || pChunk->isEmpty())
				continue;

			for (; pos.y <= m_max.y; pos.y++)
			{
				Brightness_t currentBrightness = pChunk->getBrightness(*m_pLightLayer, pos);

				TileID currentTile = pChunk->getTile(pos);
				Brightness_t lightBlockLevel = std::max(Tile::lightBlock[currentTile], Brightness_t(1));

				Brightness_t maxBrightness = Brightness::MIN;
				if (m_pLightLayer == &LightLayer::Sky)
				{
					if (pChunk->isSkyLit(pos))
						maxBrightness = Brightness::MAX;
				}
				else if (m_pLightLayer == &LightLayer::Block)
				{
					maxBrightness = Tile::lightEmission[currentTile];
				}

				Brightness_t newBrightness;

				// If light can pass through, or the block itself emits light
				if (lightBlockLevel < Brightness::MAX || maxBrightness != Brightness::MIN)
				{
					Brightness_t brightestNeighbor = Brightness::MIN;

					for (int i = 0; i < Facing::COUNT; i++)
					{
						Brightness_t neighborBrightness = m_pSource->getBrightness(*m_pLightLayer, pos + Facing::DIRECTION[i]);
                        if (neighborBrightness > brightestNeighbor)
							brightestNeighbor = neighborBrightness;
					}

					Brightness_t b = Mth::clamp(brightestNeighbor - lightBlockLevel, (int)Brightness::MIN, (int)Brightness::MAX);
					newBrightness = std::max(b, maxBrightness);
				}
				else
				{
					newBrightness = Brightness::MIN;
				}

				if (currentBrightness != newBrightness)
				{
					// Don't use pChunk here, we probably need to be firing off events to the LevelListeners
					m_pSource->setBrightness(*m_pLightLayer, pos, newBrightness);

					Brightness_t spreadBrightness = std::max(newBrightness - 1, (int)Brightness::MIN);
                    
					// why dont we check if we're in range here?
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.west(),  spreadBrightness);
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.below(), spreadBrightness);
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.north(), spreadBrightness);

					if ((pos.x + 1) >= m_max.x)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.east(),  spreadBrightness);
					if ((pos.y + 1) >= m_max.y)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.above(), spreadBrightness);
					if ((pos.z + 1) >= m_max.z)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.south(), spreadBrightness);
				}
			}
		}
	}
}

static int _indexXY(const ChunkTilePos& ctp)
{
	return (ctp.x << 11) | (ctp.z << 7);
}

// Everything here is the same as ::update() unless otherwise noted
void LightUpdate::updateFast()
{
	if (!m_pSource)
	{
		LOG_E("LightUpdate doesn't have TileSource!");
		assert(false);
		return;
	}

	// "fast"-specific
	if (m_min.x > m_max.x || m_min.y >= ChunkConstants::Y_SIZE)
		return;

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

			LevelChunk* pChunk = m_pSource->getChunk(pos);
			if (!pChunk || pChunk->isEmpty())
				continue;

			// Start fast stuff

			// Only fetch from TileSource if we're on a chunk boundary and will therefore be crossing it
			LevelChunk* pChunkWest  = ((pos.x & 0xF) == 0)                ? m_pSource->getChunk(pos.west() ) : pChunk;
			LevelChunk* pChunkEast  = ((pos.x & 0xF) == C_MAX_CHUNKS_X-1) ? m_pSource->getChunk(pos.east() ) : pChunk;
			LevelChunk* pChunkNorth = ((pos.z & 0xF) == 0)                ? m_pSource->getChunk(pos.north()) : pChunk;
			LevelChunk* pChunkSouth = ((pos.z & 0xF) == C_MAX_CHUNKS_Z-1) ? m_pSource->getChunk(pos.south()) : pChunk;

			LevelChunk::NibbleTileArray& light       =                    pChunk->getLight(*m_pLightLayer);
			LevelChunk::NibbleTileArray* pLightWest  = pChunkWest  ? &pChunkWest->getLight(*m_pLightLayer)  : nullptr;
			LevelChunk::NibbleTileArray* pLightEast  = pChunkEast  ? &pChunkEast->getLight(*m_pLightLayer)  : nullptr;
			LevelChunk::NibbleTileArray* pLightNorth = pChunkNorth ? &pChunkNorth->getLight(*m_pLightLayer) : nullptr;
			LevelChunk::NibbleTileArray* pLightSouth = pChunkSouth ? &pChunkSouth->getLight(*m_pLightLayer) : nullptr;

			int idxCenterBase = _indexXY(pos);
			int idxWestBase   = _indexXY(pos.west());
			int idxEastBase   = _indexXY(pos.east());
			int idxNorthBase  = _indexXY(pos.north());
			int idxSouthBase  = _indexXY(pos.south());

			for (; pos.y <= m_max.y; pos.y++)
			{
				// Assemble 1D offsets for current Y
				int idx      = idxCenterBase | pos.y;
				int idxWest  = idxWestBase   | pos.y;
				int idxEast  = idxEastBase   | pos.y;
				int idxNorth = idxNorthBase  | pos.y;
				int idxSouth = idxSouthBase  | pos.y;

				Brightness_t currentBrightness = light.get(idx);

			// End fast stuff
					
				TileID currentTile = pChunk->getTile(pos);
				Brightness_t lightBlockLevel = std::max(Tile::lightBlock[currentTile], Brightness_t(1));

				Brightness_t maxBrightness = Brightness::MIN;
				if (m_pLightLayer == &LightLayer::Sky)
				{
					if (pChunk->isSkyLit(pos))
						maxBrightness = Brightness::MAX;
				}
				else if (m_pLightLayer == &LightLayer::Block)
				{
					maxBrightness = Tile::lightEmission[currentTile];
				}

				Brightness_t newBrightness;

				// If light can pass through, or the block itself emits light
				if (lightBlockLevel < Brightness::MAX || maxBrightness != Brightness::MIN)
				{
					Brightness_t brightestNeighbor = Brightness::MIN;

					// Start fast stuff
					#define DONEYET if (brightestNeighbor >= Brightness::MAX) goto KEEP_GOING

					if (pLightWest)  brightestNeighbor = std::max(brightestNeighbor, pLightWest->get(idxWest)); DONEYET;
					if (pLightEast)  brightestNeighbor = std::max(brightestNeighbor, pLightEast->get(idxEast)); DONEYET;
					if (pLightNorth) brightestNeighbor = std::max(brightestNeighbor, pLightNorth->get(idxNorth)); DONEYET;
					if (pLightSouth) brightestNeighbor = std::max(brightestNeighbor, pLightSouth->get(idxSouth)); DONEYET;

					if (pos.y < C_MAX_Y - 1) brightestNeighbor = std::max(brightestNeighbor, light.get(idx + 1)); DONEYET;
					if (pos.y > C_MIN_Y)     brightestNeighbor = std::max(brightestNeighbor, light.get(idx - 1));
					// End fast stuff

					KEEP_GOING:
					Brightness_t b = brightestNeighbor > lightBlockLevel ? brightestNeighbor - lightBlockLevel : Brightness::MIN;
					newBrightness = std::max(b, maxBrightness);
				}
				else
				{
					newBrightness = Brightness::MIN;
				}

				if (currentBrightness != newBrightness)
				{
					// Don't use pChunk here, we probably need to be firing off events to the LevelListeners
					m_pSource->setBrightness(*m_pLightLayer, pos, newBrightness);

					Brightness_t spreadBrightness = std::max(newBrightness - 1, (int)Brightness::MIN);

					// why dont we check if we're in range here?
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.west(),  spreadBrightness);
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.below(), spreadBrightness);
					m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.north(), spreadBrightness);

					if ((pos.x + 1) >= m_max.x)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.east(),  spreadBrightness);
					if ((pos.y + 1) >= m_max.y)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.above(), spreadBrightness);
					if ((pos.z + 1) >= m_max.z)
						m_pSource->updateLightIfOtherThan(*m_pLightLayer, pos.south(), spreadBrightness);
				}
			}
		}
	}
}

bool LightUpdate::expandIfCloseEnough(const TilePos& lowerPos, const TilePos& upperPos)
{
	if (lowerPos >= m_min && upperPos <= m_max)
		return true;

	// Must use strict component-wise axis checking, not lexicographical operators.
	if (lowerPos.x < m_min.x - 1 || lowerPos.y < m_min.y - 1 || lowerPos.z < m_min.z - 1 ||
		upperPos.x > m_max.x + 1 || upperPos.y > m_max.y + 1 || upperPos.z > m_max.z + 1)
		return false;

	TilePos newMin = m_min.min(lowerPos);
	TilePos newMax = m_max.max(upperPos);

    // Prevent the LightUpdate bounds from expanding beyond the 2nd dimension
	if (Mth::abs((newMax - newMin).volume() - (m_max - m_min).volume()) > 2)
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