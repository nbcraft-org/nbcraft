#pragma once

#include "world/entity/Zombie.hpp"

class PigZombie : public Zombie
{
public:
	PigZombie(Level* pLevel);

	std::string getAmbientSound() const override { return "mob.zombiepig.zpig"; }
	std::string getHurtSound() const override { return "mob.zombiepig.zpighurt"; }
	std::string getDeathSound() const override { return "mob.zombiepig.zpigdeath"; }
	int getDeathLoot() const override { return ITEM_PORKCHOP_COOKED; }
	//ItemInstance* getCarriedItem() const override { return m_sword; }

	void tick() override;
	bool canSpawn() override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	Entity* findAttackTarget() override;
	void aiStep() override;
	bool hurt(Entity*, int) override;
	ItemStack& getCarriedItem() const override;

private:
	void alert(Entity*);

	int m_angerTime = 0;
	int m_playAngrySoundIn = 0;
	static ItemStack m_sword;
};