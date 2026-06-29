#pragma once

#include "Entity.hpp"

class FishingHook : public Entity
{
public:
	static const unsigned int ARROW_BASE_DAMAGE;

private:
	void _init();

public:
	FishingHook(Level* pLevel);
	FishingHook(Level* pLevel, const Vec3& pos);
	FishingHook(Level* pLevel, Player* pMob);

private:
	void _lerpMotion(const Vec3& vel);
	void _lerpMotion2(const Vec3& vel);

public:
	bool shouldRenderAtSqrDistance(float distSqr) const override;
	void shoot(float x, float y, float z, float speed, float r) { shoot(Vec3(x, y, z), speed, r); };
	void shoot(Vec3 pos, float speed, float r);

	void lerpTo(const Vec3& pos, const Vec2& rot, int steps) override;
	void lerpMotion(float x, float y, float z) { lerpMotion(Vec3(x, y, z)); };
	void lerpMotion(const Vec3& vel) override;

	void tick() override;
	void remove() override;

	float getShadowHeightOffs() const override { return 0.0f; }
	AuxValue getAuxValue() const override;
	void setAuxValue(AuxValue value) override;
	int retrieve();

private:
	TilePos m_tilePos;
	TileID m_lastTile;
	int8_t m_lastTileData;
	bool m_bInGround;
	bool m_bIsPlayerOwned;
	int m_life;
	int m_flightTime;
	int m_nibble;
	int m_lSteps;
	Vec3 m_lPos;
	Vec2 m_lRot;
	Vec3 m_lVel;

public:
	int m_shakeTime;
	Player* m_owner;
	Entity* m_hookedIn;
};
