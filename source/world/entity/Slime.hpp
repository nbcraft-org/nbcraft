#pragma once

#include "Mob.hpp"

class Slime : public Mob
{
public:
	Slime(Level* pLevel);

	void setSize(int);
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	void tick() override;
	void updateAi() override;
	void remove() override;
	void playerTouch(Player*) override;
	std::string getHurtSound() const override { return "mob.slime"; }
	std::string getDeathSound() const override { return "mob.slime"; }
	int getDeathLoot() const override { return m_size == 1 ? Item::slimeBall->m_itemID : 0; }
	bool canSpawn() override;
	float getSoundVolume() const override { return 0.6f; }

	float m_squish;
	float m_oSquish;
	int m_size;

private:
	int m_jumpDelay;
};