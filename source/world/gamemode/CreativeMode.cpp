/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "CreativeMode.hpp"
#include "client/app/Minecraft.hpp"

CreativeMode::CreativeMode(Minecraft* pMC)
	: GameMode(pMC)
	, m_destroyCooldown(0)
{
}

bool CreativeMode::destroyBlock(Player& player, const TilePos& pos, Facing::Name face)
{
	Level& level = player.getLevel();
	TileSource& tileSource = player.getTileSource();

	level.extinguishFire(tileSource, pos, face);
	return GameMode::destroyBlock(player, pos, face);
}

bool CreativeMode::startDestroyBlock(Player& player, const TilePos& pos, Facing::Name face)
{
	ItemStack& item = player.getSelectedItem();
	if (item && item.getItem() == Item::bow)
		return true;

	m_destroyCooldown = 5;
	return destroyBlock(player, pos, face);
}

bool CreativeMode::continueDestroyBlock(Player& player, const TilePos& pos, Facing::Name face)
{
	if (m_destroyCooldown - 1 > 0)
	{
		m_destroyCooldown--;
		return false;
	}

	m_destroyCooldown = 5;
	return destroyBlock(player, pos, face);
}

void CreativeMode::stopDestroyBlock()
{
	m_destroyCooldown = 0;
}

void CreativeMode::tick()
{
	GameMode::tick();
}

void CreativeMode::render(float f)
{
	GameMode::render(f);
}

void CreativeMode::initPlayer(Player& p)
{
	p.m_rot.yaw = -180.0f;
}
