/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "SurvivalMode.hpp"
#include "GameMods.hpp"
#include "client/app/Minecraft.hpp"
#include "network/packets/RemoveBlockPacket.hpp"
#include "world/level/TileSource.hpp"

SurvivalMode::SurvivalMode(Minecraft* pMC)
	: GameMode(pMC)
	, m_destroyingPos(-1, -1, -1)
	, m_destroyProgress(0.0f)
	, m_lastDestroyProgress(0.0f)
	, m_destroyTicks(0)
	, m_destroyCooldown(0)
{
}

void SurvivalMode::_resetDestruction()
{
	m_destroyProgress = 0.0f;
	m_lastDestroyProgress = 0.0f;
	m_destroyTicks = 0;
}

void SurvivalMode::initPlayer(Player& p)
{
	p.m_rot.yaw = -180.0f;
	p.m_pInventory->prepareSurvivalInventory();
}

bool SurvivalMode::canHurtPlayer()
{
	return true;
}

bool SurvivalMode::startDestroyBlock(Player& player, const TilePos& pos, Facing::Name face)
{
	ItemStack& item = player.getSelectedItem();
	if (!item.isEmpty() && item.getItem() == Item::bow)
		return true;

	TileSource& source = player.getTileSource();

	TileID tile = source.getTile(pos);

	if (tile <= 0)
		return false;

	Level& level = player.getLevel();

	// @PARITY: This is in MultiPlayerGameMode on Java, but we aren't equipped to move to that at the moment. Also, is not sent on PE.
#if NETWORK_PROTOCOL_VERSION >= 6
	if (level.m_bIsClientSide)
	{
		level.m_pRakNetInstance->send(new PlayerActionPacket(player.m_EntityID, PlayerActionPacket::START_DESTROY_BLOCK, pos, face));
	}
#endif

	if (m_destroyProgress == 0.0f)
	{
		Tile::tiles[tile]->attack(pos, player);
	}

	if (Tile::tiles[tile]->getDestroyProgress(player) >= 1.0f)
	{
		return destroyBlock(player, pos, face);
	}

	return false;
}

bool SurvivalMode::destroyBlock(Player& player, const TilePos& pos, Facing::Name face)
{
	TileSource& source = player.getTileSource();

	TileID   tileId = source.getTile(pos);
	TileData data   = source.getData(pos);
	Tile*    pTile  = Tile::tiles[tileId];

	bool changed = GameMode::destroyBlock(player, pos, face);

	bool couldDestroy = player.canDestroy(pTile);
	ItemStack& item = player.getSelectedItem();
	if (!item.isEmpty())
	{
		item.mineBlock(pos, face, player);
		if (item.m_count == 0)
		{
			item.snap(player);
			player.removeSelectedItem();
		}
	}

	if (changed && couldDestroy)
	{
#ifdef MOD_POCKET_SURVIVAL
		ItemStack tileItem(tile, 1, data);
		if (tile == TILE_GRASS || !player.m_pInventory->hasUnlimitedResource(tileItem))
		{
			pTile->playerDestroy(player, pos, data);
		}
#else
		pTile->playerDestroy(player, pos, data);
#endif
	}

	return changed;
}

bool SurvivalMode::continueDestroyBlock(Player& player, const TilePos& pos, Facing::Name face)
{
	if (m_destroyCooldown > 0)
	{
		m_destroyCooldown--;
		return false;
	}

	if (m_destroyingPos != pos)
	{
		_resetDestruction();
		m_destroyingPos = pos;
		return false;
	}

	TileSource& source = player.getTileSource();

	TileID tile = source.getTile(m_destroyingPos);
	if (!tile)
		return false;

	Tile* pTile = Tile::tiles[tile];
	float destroyProgress = pTile->getDestroyProgress(*m_pMinecraft->m_pLocalPlayer);
	m_destroyProgress += getDestroyModifier() * destroyProgress;
	m_destroyTicks++;

	Level& level = player.getLevel();

	if ((m_destroyTicks & 3) == 1) // m_destroyTicks % 4.0f == 0.0f
	{
		level.playSound(pos + 0.5f, "step." + pTile->m_pSound->name,
			0.125f * (1.0f + pTile->m_pSound->volume), 0.5f * pTile->m_pSound->pitch);
	}

	if (m_destroyProgress >= 1.0f)
	{
		_resetDestruction();
		m_destroyCooldown = 5;

		// @PARITY: This is in MultiPlayerGameMode on Java, but we aren't equipped to move to that at the moment. Also, is not sent on PE.
#if NETWORK_PROTOCOL_VERSION >= 6
		if (level.m_bIsClientSide)
		{
			level.m_pRakNetInstance->send(new PlayerActionPacket(player.m_EntityID, PlayerActionPacket::STOP_DESTROY_BLOCK, pos, face));
		}
#endif
		return destroyBlock(player, m_destroyingPos, face);
	}

	return false;
}

void SurvivalMode::stopDestroyBlock()
{
	m_destroyProgress = 0.0f;
	m_destroyCooldown = 0;
}

void SurvivalMode::tick()
{
	m_lastDestroyProgress = m_destroyProgress;
	//m_pMinecraft->m_pSoundEngine->playMusicTick(); // also on MultiPlayerGameMode
	GameMode::tick();
}

void SurvivalMode::render(float f)
{
	const HitResult& hr = m_pMinecraft->m_hitResult;
	Gui& gui = *m_pMinecraft->m_pGui;
	LevelRenderer& levelRenderer = *m_pMinecraft->m_pLevelRenderer;

	// @HACK: fix destory progress carrying over for the first few frames of another Tile
	if (m_destroyProgress > 0.0f && hr.isHit() && m_destroyingPos != hr.m_tilePos)
	{
		_resetDestruction();
	}

	if (m_destroyProgress <= 0.0f)
	{
		gui.m_progress = 0.0f;
		gui.m_lastDestroyProgress = 0.0f;
		gui.m_destroyProgress = 0.0f;
		levelRenderer.m_destroyProgress = 0.0f;
	}
	else
	{
		float dp = m_lastDestroyProgress + (m_destroyProgress - m_lastDestroyProgress) * f;
		gui.m_progress = dp;
		gui.m_lastDestroyProgress = m_lastDestroyProgress;
		gui.m_destroyProgress = m_destroyProgress;
		levelRenderer.m_destroyProgress = dp;
	}
}

bool SurvivalMode::useItemOn(Player& player, ItemStack& item, const TilePos& pos, Facing::Name face)
{
#ifdef MOD_POCKET_SURVIVAL
	if (item.isEmpty())
		return GameMode::useItemOn(player, item, pos, face);

	bool unlimited = player.m_pInventory->hasUnlimitedResource(item);
	int oldCount = item.m_count;
	bool result = GameMode::useItemOn(player, item, pos, face);
	if (unlimited)
		item.m_count = oldCount;

	return result;
#else
	return GameMode::useItemOn(player, item, pos, face);
#endif
}
