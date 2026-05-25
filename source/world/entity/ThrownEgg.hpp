#pragma once

#include "Entity.hpp"


class ThrownEgg : public Entity
{
private:
	void _init();
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
public:
	ThrownEgg(Level* pLevel);
	ThrownEgg(Level* pLevel, Mob* pMob);
	ThrownEgg(Level* pLevel, const Vec3& pos);
private:
	void _lerpMotion(const Vec3& vel);
	void _lerpMotion2(const Vec3& vel);
public:
	bool shouldRenderAtSqrDistance(float distSqr) const override;
	void shoot(float x, float y, float z, float speed, float r) { shoot(Vec3(x, y, z), speed, r); };
	void shoot(Vec3 pos, float speed, float r);
	void lerpMotion(float x, float y, float z) { lerpMotion(Vec3(x, y, z)); };
	void lerpMotion(const Vec3& vel) override;
	void tick() override;
	virtual void addAdditionalSaveData(CompoundTag& tag) const override;
	virtual void readAdditionalSaveData(const CompoundTag& tag) override;
	float getShadowHeightOffs() const override { return 0.0f; }
	AuxValue getAuxValue() const override;
	void setAuxValue(AuxValue value) override;
};
