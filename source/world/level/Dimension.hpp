/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "common/math/Color.hpp"
#include "world/phys/Vec3.hpp"
#include "DimensionId.hpp"

class Level; // if included from Level.hpp
class ChunkSource;
class BiomeSource;
class TileSource;
class Entity;

class Dimension
{
public:
	Dimension();
	virtual ~Dimension();
	static Dimension* createNew(DimensionId type);

	virtual bool isNaturalDimension() const;
	virtual void init();
	virtual bool mayRespawn() const;
    virtual bool isValidSpawn(const TilePos& pos) const;

	DimensionId getId() const { return m_id; }
	bool isDay() const;
	bool isFoggy() const { return m_bFoggy; }
	bool isUltraWarm() const { return m_bUltraWarm; }
	virtual Color getSkyColor(const Entity& entity, float f) const;
	virtual Color getFogColor(float f) const;
	virtual Color getSunriseColor(float, float) const;
	virtual Color getCloudColor(float f) const;
	virtual float getSunAngle(float f) const;
	virtual float getTimeOfDay(int32_t, float) const;
	virtual float getTimeOfDay(float f) const;
	virtual float getStarBrightness(float f) const;

	void init(Level* pLevel);
	void updateLightRamp();

	ChunkSource* createRandomLevelSource();

	ChunkSource* getChunkSource() const;
	TileSource* getTileSource() const;

public:
	Level* m_pLevel;
	BiomeSource* m_pBiomeSource;
	bool m_bFoggy;
	bool m_bUltraWarm;
	bool m_bHasCeiling;
	float m_brightnessRamp[16];
	DimensionId m_id;
};

