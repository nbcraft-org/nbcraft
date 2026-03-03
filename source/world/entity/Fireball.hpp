#pragma once

#include "Entity.hpp"

class Fireball : public Entity
{
public:
	static const unsigned int ARROW_BASE_DAMAGE;

private:
	void _init();

public:
	Fireball(Level* pLevel);
	Fireball(Level* pLevel, Mob* pMob, Vec3 pos);

	bool shouldRenderAtSqrDistance(float distSqr) const;

	void tick() override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	bool isPickable() const override { return true; }
	float getPickRadius() const override { return 1.0f; }
	bool hurt(Entity* pCulprit, int damage) override;

	float getShadowHeightOffs() const override { return 0.0f; }

private:
	TilePos m_tilePos;
	TileID m_lastTile;
	int8_t m_lastTileData;
	bool m_bInGround;
	bool m_bIsPlayerOwned;
	int m_life;
	int m_flightTime;

public:
	int m_shakeTime;
	Mob* m_owner;
	float m_xPower;
	float m_yPower;
	float m_zPower;
};