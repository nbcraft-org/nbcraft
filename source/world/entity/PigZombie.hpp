#pragma once

#include "world/entity/Zombie.hpp"

class PigZombie : public Zombie
{
public:
	PigZombie(Level* pLevel);

private:
	void _alert(Entity*);

public:
	std::string getAmbientSound() override { return "mob.zombiepig.zpig"; }
	std::string getHurtSound() const override { return "mob.zombiepig.zpighurt"; }
	std::string getDeathSound() const override { return "mob.zombiepig.zpigdeath"; }
	int getDeathLoot() const override { return ITEM_PORKCHOP_COOKED; }

	void tick() override;
	bool canSpawn() override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	Entity* findAttackTarget() override;
	void aiStep() override;
	bool hurt(Entity*, int) override;
	ItemStack& getCarriedItem() const override;

private:
	int m_angerTime;
	int m_playAngrySoundIn;
};
