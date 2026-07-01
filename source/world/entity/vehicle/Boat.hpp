#pragma once

#include "world/entity/Entity.hpp"

class Level;
class Player;

class Boat : public Entity
{
private:
	void _init();

public:
	Boat(Level*);
	Boat(Level*, const Vec3& pos);

	const AABB* getCollideBox() const override;
	AABB* getCollideAgainstBox(Entity* ent) const override;
	float getShadowHeightOffs() const override { return 0.0f; }
	bool interact(Player* player) override;
	bool isPickable() const override { return !m_bRemoved; }
	bool isPushable() const override { return true; }
	void tick() override;
	float getRideHeight() const override;
	void positionRider() override;
	bool hurt(Entity*, int) override;
	void animateHurt() override;
	void lerpTo(const Vec3& pos, const Rot2& rot, int steps) override;
	void lerpMotion(const Vec3& vel) override;

public:
	int m_damage;
	int m_hurtTime;
	int m_hurtDir;

private:
	int m_lSteps;
	Vec3 m_lPos;
	Vec3 m_lPosD;
	Rot2 m_lRot;
};

