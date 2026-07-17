/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#pragma once

#include "Animal.hpp"

#define DATA_SADDLE (16)

class Pig : public Animal
{
public:
	Pig(Level* pLevel);

public:
	std::string getAmbientSound() override { return "mob.pig"; };
	std::string getDeathSound() const override { return "mob.pigdeath"; }
	std::string getHurtSound() const override { return "mob.pig"; }
	int getDeathLoot() const override;
	int getMaxHealth() const override { return 10; }
	bool interact(Player*) override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;

	bool hasSaddle() const;
	void setSaddle(bool b);
	Entity* getBreedOffspring(Animal* pOther) { return new Pig(m_pLevel); }

private:
	void _defineEntityData();
};
