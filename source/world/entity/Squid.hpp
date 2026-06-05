#pragma once

#include "WaterAnimal.hpp"

class Squid : public WaterAnimal
{
//private:
//	static ItemStack inkSack;
//
public:
	Squid(Level* pLevel);

	std::string getAmbientSound() const override { return ""; }
	std::string getDeathSound() const override { return ""; }
	std::string getHurtSound() const override { return ""; }
	int getDeathLoot() const override { return 0; }
	void dropDeathLoot() override;
	int getMaxHealth() const override { return 10; }
	float getSoundVolume() const override { return 0.4f; }
	bool interact(Player* player) override;
	bool isInWater() override;
	void aiStep() override;
	void travel(const Vec2&) override;
	void updateAi() override;

public:
	float m_xBodyRot;
	float m_xBodyRotO;
	float m_zBodyRot;
	float m_zBodyRotO;
	float m_tentacleMovement;
	float m_oldTentacleMovement;
	float m_tentacleAngle;
	float m_oldTentacleAngle;

private:
	float m_speed;
	float m_tentacleSpeed;
	float m_rotateSpeed;
	Vec3 m_tPos;
};
