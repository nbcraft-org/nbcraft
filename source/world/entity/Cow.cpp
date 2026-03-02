/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#include "Cow.hpp"
#include "world/entity/Player.hpp"

Cow::Cow(Level* pLevel) : Animal(pLevel)
{
	m_pDescriptor = &EntityTypeDescriptor::cow;
	m_renderType = RENDER_COW;
	m_texture = "mob/cow.png";
	setSize(0.9f, 1.3f);
}

void Cow::addAdditionalSaveData(CompoundTag& tag) const
{
	Animal::addAdditionalSaveData(tag);
}

void Cow::readAdditionalSaveData(const CompoundTag& tag)
{
	Animal::readAdditionalSaveData(tag);
}

bool Cow::interact(Player* player)
{
	ItemStack& var2 = player->m_pInventory->getSelected();
	if (!var2.isEmpty() && var2.getId() == Item::bucket_empty->m_itemID)
	{
		player->m_pInventory->setItem(player->m_pInventory->m_selectedSlot, ItemStack(Item::milk));
		return true;
	}
	else
	{
		return false;
	}
}