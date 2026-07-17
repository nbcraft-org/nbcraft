/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#include "Pig.hpp"
#include "Player.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

Pig::Pig(Level* pLevel) : Animal(pLevel)
{
	m_pDescriptor = &EntityTypeDescriptor::pig;
	m_renderType = RENDER_PIG;
	m_texture = "mob/pig.png";
	setSize(0.9f, 0.9f);
	
	_defineEntityData();
}

void Pig::_defineEntityData()
{
	m_entityData.define<int8_t>(DATA_SADDLE, 0);
}

int Pig::getDeathLoot() const
{
	return (isOnFire()) ? 
		Item::porkChop_cooked->m_itemID :
		Item::porkChop_raw->m_itemID;
}

bool Pig::interact(Player* pPlayer)
{
	if (m_pLevel->m_bIsClientSide || !hasSaddle())
		return false;

	Entity* rider = getRider();

	// already being ridden by someone else
	if (rider && rider != pPlayer)
	{
		return false;
	}

	pPlayer->ride(this);
	return true;
}

void Pig::addAdditionalSaveData(CompoundTag& tag) const
{
	Animal::addAdditionalSaveData(tag);
	tag.putBoolean("Saddle", hasSaddle());
}

void Pig::readAdditionalSaveData(const CompoundTag& tag)
{
	Animal::readAdditionalSaveData(tag);
	setSaddle(tag.getBoolean("Saddle"));
}

bool Pig::hasSaddle() const
{
	return m_entityData.get<int8_t>(DATA_SADDLE) & 1;
}

void Pig::setSaddle(bool b)
{
	m_entityData.set<int8_t>(DATA_SADDLE, b);
}
