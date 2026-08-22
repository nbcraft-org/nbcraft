#pragma once

#include "world/entity/Entity.hpp"
#include "world/inventory/SimpleContainer.hpp"
#include "world/level/TilePos.hpp"

class Level;
class Player;

class Minecart : public Entity, SimpleContainer
{
private:
	void _init();
	void _adjustVelocity(const TilePos& tp, bool& canPush);
	void _adjustRotation();

public:
	enum Type
	{
		TYPE_DEFAULT,
		TYPE_CHEST,
		TYPE_FURNACE
	};


	Minecart(TileSource&);
	Minecart(TileSource&, Type, const Vec3& pos = Vec3::ZERO);

	const AABB* getCollideBox() const override;
	AABB* getCollideAgainstBox(Entity* ent) const override;
	float getShadowHeightOffs() const override { return 0.0f; }
	bool interact(Player* player) override;
	bool isPickable() const override { return !m_bRemoved; }
	bool isPushable() const override { return true; }
	void tick() override;
	float getRideHeight() const override;
	bool hurt(Entity*, int) override;
	void animateHurt() override;
	void remove() override;
	Vec3* getPosOffs(const Vec3&, float) const;
	Vec3* getOnRailPos(const Vec3&) const;
	void push(Entity*) override;
	void lerpTo(const Vec3& pos, const Rot2& rot, int steps) override;
	void lerpMotion(const Vec3& vel) override;
	bool stillValid(Player& player) const override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;

	float getLootContent();

private:
	static const TilePos EXITS[10][2];
	bool m_bFlipped;
	int m_lSteps;
	Vec3 m_lPos;
	Vec3 m_lPosD;
	Rot2 m_lRot;

public:
	int m_damage;
	int m_hurtTime;
	int m_hurtDir;
	Type m_type;
	int m_fuel;
	float m_pushX;
	float m_pushZ;
};
