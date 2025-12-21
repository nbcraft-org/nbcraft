#pragma once

#include "WaterAnimal.hpp"

class Squid : public WaterAnimal
{
public:
	Squid(Level* pLevel);

	std::string getAmbientSound() const override { return ""; };
	std::string getDeathSound() const override { return ""; }
	std::string getHurtSound() const override { return ""; }
	int getDeathLoot() const override { return 0; }
	void dropDeathLoot() override;
	int getMaxHealth() const override { return 10; }
	float getSoundVolume() const override { return 0.4f; }
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	bool interact(Player* player) override;
	bool isInWater() override;
	void aiStep() override;
	void travel(const Vec2&) override;
	void updateAi() override;

public:
	float m_xBodyRot = 0.0f;
	float m_xBodyRotO = 0.0f;
	float m_zBodyRot = 0.0f;
	float m_zBodyRotO = 0.0f;
	float m_tentacleMovement = 0.0f;
	float m_oldTentacleMovement = 0.0f;
	float m_tentacleAngle = 0.0f;
	float m_oldTentacleAngle = 0.0f;

private:
	float m_speed = 0.0f;
	float m_tentacleSpeed = 0.0f;
	float m_rotateSpeed = 0.0f;
	Vec3 m_tPos;
};
