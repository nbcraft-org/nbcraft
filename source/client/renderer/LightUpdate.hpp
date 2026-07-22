#pragma once
#include "client/renderer/LightLayer.hpp"
#include "world/level/TilePos.hpp"

class TileSource;

class LightUpdate
{
private:
	const LightLayer* m_pLightLayer; // non-static reference member can't use copy assignment operator
	TileSource* m_pSource;
	TilePos m_min;
	TilePos m_max;

public:
	LightUpdate(TileSource& source, const LightLayer& lightLayer, const TilePos& min, const TilePos& max)
		: m_pLightLayer(&lightLayer)
		, m_pSource(&source)
		, m_min(min)
		, m_max(max)
	{
	}

public:
	void update();
	void updateFast();

	bool expandIfCloseEnough(const TilePos& lowerPos, const TilePos& upperPos);
	void expandToContain(const TilePos& pos);
	void expandToContain(const TilePos& pos1, const TilePos& pos2);

	// @HACK: for Level::updateLight()
	friend class Level;
};
