/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "ServerSideNetworkHandler.hpp"
#include "common/Logger.hpp"
#include "GameMods.hpp"
#include "network/MinecraftPackets.hpp"
#include "world/entity/MobFactory.hpp"
#include "ServerPlayer.hpp"

// How frequently SetTimePackets are sent, in seconds.
// b1.3 sends every second. 0.2.1 seems to send every 12.
#define NETWORK_TIME_SEND_FREQUENCY 12

// This lets you make the server shut up and not log events in the debug console.
//#define VERBOSE_SERVER

#if defined(ORIGINAL_CODE) || defined(VERBOSE_SERVER)
#define puts_ignorable(str) LOG_I(str)
#define printf_ignorable LOG_I
#else
static inline void _do_nothing(...) {}
#define puts_ignorable(str)
#define printf_ignorable _do_nothing
#endif

#define VALIDATE_PLAYER_ACTION(entityId)                   \
	Player* _pPlayer = _getVerifiedPlayer(guid, entityId); \
	if (!_pPlayer)                                         \
		return;                                            \
	Player& player = *_pPlayer;                            \
	(void)player                                           \

ServerSideNetworkHandler::ServerSideNetworkHandler(Minecraft* minecraft, RakNetInstance* rakNetInstance)
{
	m_pMinecraft = minecraft;
	m_pLevel = nullptr;
	m_pRakNetInstance = rakNetInstance;
	allowIncomingConnections(false);
	m_pRakNetPeer = m_pRakNetInstance->getPeer();
	m_bAllowIncoming = false;

	m_bStrictPlayerMovement = false;

	setupCommands();
}

ServerSideNetworkHandler::~ServerSideNetworkHandler()
{
	if (m_pLevel)
		m_pLevel->removeListener(this);

	for (OnlinePlayerMap::iterator it = m_onlinePlayers.begin(); it != m_onlinePlayers.end(); ++it)
		delete it->second;

	m_onlinePlayers.clear();
}

Packet* _getPacketForEntity(Entity& entity)
{
	if (entity.getDescriptor().isType(EntityType::ITEM))
	{
#if NETWORK_PROTOCOL_VERSION >= 2
		return new AddItemEntityPacket((ItemEntity&)entity);
#endif
	}
	else if (entity.isMob())
	{
		//LOG_I("add mob packet!");
		return new AddMobPacket((Mob&)entity);
	}
	else
	{
#if NETWORK_PROTOCOL_VERSION >= 6
		return new AddEntityPacket(entity);
#endif
	}

	return nullptr;
}

Player* ServerSideNetworkHandler::_getVerifiedPlayer(const RakNet::RakNetGUID& guid, Entity::ID entityId) const
{
	Entity* pEntity = m_pLevel->getEntity(entityId);
	if (!pEntity || !pEntity->isPlayer())
		return nullptr;

	Player* pPlayer = (Player*)pEntity;
	if (pPlayer->m_guid != guid)
		return nullptr;

	return pPlayer;
}

// Java player movement handling from Beta 1.3, rubberbanding included
void ServerSideNetworkHandler::_handleMovePlayer(Player& player, MovePlayerPacket* packet)
{
	// Initial positioning
	Vec3 oPos = player.m_pos;
	Vec3 pos;
	Vec2 rot;

	// Handle the player riding state
	if (player.isRiding())
	{
		Entity* pRiding = player.getRiding();

		if (pRiding)
		{
			pRiding->positionRider();
		}

		//if (packet->hasRot) {
		rot = packet->m_rot;
		//}

		//if (packet->hasPos && packet->m_pos.y == -999.0f && packet->yView == -999.0f) {
		pos.x = packet->m_pos.x;
		pos.z = packet->m_pos.z;
		//}

		//player.m_bOnGround = packet->onGround;
		//player.doChunkSendingTick(true);
		player.move(pos);
		player.absMoveTo(oPos, rot);
		player.m_vel.x = pos.x;
		player.m_vel.z = pos.z;
		/*if (pRiding)
		{
			m_pLevel->forceTick(player.getRiding(), true);
		}*/

		if (pRiding)
		{
			pRiding->positionRider();
		}

		//this.server.playerList.move(player); // playerChunkMap.move(player)
		//m_pLevel->tick(player);
		return;
	}

	/*if (packet->hasPos && packet->m_pos.y == -999.0f && packet->yView == -999.0f) {
		packet->hasPos = false;
	}*/

	//if (packet->hasPos) {
	pos = packet->m_pos;
	// Stance validation
	/*float stanceHeight = packet->yView - packet->m_pos.y;
	if (stanceHeight > 1.65f || stanceHeight < 0.1f)
	{
		LOG_W("%s had an illegal stance: %f", player.m_name, stanceHeight);
	}*/
	//}

	//if (packet->hasRot) {
	rot = packet->m_rot;
	//}

	//player.doChunkSendingTick(true);
	player.m_ySlideOffset = 0.0f;
	player.absMoveTo(oPos, rot);

	// Calculate spatial differences
	Vec3 delta = pos - player.m_pos;
	constexpr float shrinkAmount = 1.0f / 16.0f;

	AABB aabb(player.m_hitbox);
	aabb.shrink(shrinkAmount, shrinkAmount, shrinkAmount);
	bool hasNoCollisionBefore = m_pLevel->getCubes(&player, aabb)->size() == 0;

	// Apply initial movement
	player.move(delta);

	// Recalculate differences post-movement
	Vec3 postDelta = pos - player.m_pos;

	// b1.3, this logic is accurate but always evaluates to true
	/*if (postDelta.y > -0.5f || postDelta.y < 0.5f)
	{*/
		postDelta.y = 0.0f;
	//}

	float sqDistanceDiff = postDelta.lengthSqr();

	bool movedWrongly = false;
	if (sqDistanceDiff > shrinkAmount /*&& !player.isSleeping()*/)
	{
		movedWrongly = true;
		LOG_W("%s moved wrongly!", player.m_name.c_str());
		LOG_I("Got position %f, %f, %f", pos.x, pos.y, pos.z);
		LOG_I("Expected %f, %f, %f", player.m_pos.x, player.m_pos.y, player.m_pos.z);
	}

	player.absMoveTo(pos, rot);

	aabb = player.m_hitbox;
	aabb.shrink(shrinkAmount, shrinkAmount, shrinkAmount);
	bool hasNoCollisionAfter = m_pLevel->getCubes(&player, aabb)->size() == 0;

	// Revert movement if illegal collisions or invalid movement occur
	if (hasNoCollisionBefore && (movedWrongly || !hasNoCollisionAfter) /*&& !player.isSleeping()*/)
	{
		player.absMoveTo(oPos, rot);
		return;
	}

	//player.m_bOnGround = packet->onGround;
	//this.server.playerList.move(player); // playerChunkMap.move(player)
	ServerPlayer& serverPlayer = (ServerPlayer&)player;
	serverPlayer.doCheckFallDamage(player.m_pos.y - oPos.y, player.m_bOnGround/*packet->onGround*/);

	redistributePacket(packet, player.m_guid);
}

void ServerSideNetworkHandler::levelGenerated(Level* level)
{
	m_pLevel = level;

	if (m_pMinecraft->m_pLocalPlayer)
	{
		m_pMinecraft->m_pLocalPlayer->m_guid = m_pRakNetPeer->GetMyGUID();
	}

	level->addListener(this);

	allowIncomingConnections(m_pMinecraft->getOptions()->m_serverVisibleDefault.get());

	m_onlinePlayers[m_pMinecraft->m_pLocalPlayer->m_guid] = new OnlinePlayer(m_pMinecraft->m_pLocalPlayer, m_pMinecraft->m_pLocalPlayer->m_guid);
}

void ServerSideNetworkHandler::onNewClient(const RakNet::RakNetGUID& guid)
{
	printf_ignorable("onNewClient, client guid: %s", guid.ToString());
}

void ServerSideNetworkHandler::onDisconnect(const RakNet::RakNetGUID& guid)
{
	puts_ignorable("onDisconnect");

	Player* pPlayer = nullptr;

	OnlinePlayer* pOnlinePlayer = getOnlinePlayerByGUID(guid);
	if (pOnlinePlayer)
	{
		// Player was in-game
		pPlayer = pOnlinePlayer->m_pPlayer;

		m_onlinePlayers.erase(guid);

		// delete the online player's entry.
		delete pOnlinePlayer;
		// pPlayer is managed by Level

		// tell everyone that they left the game
		displayGameMessage(pPlayer->m_name + " disconnected from the game");

		m_pRakNetInstance->send(new RemoveEntityPacket(pPlayer->m_EntityID));

		pPlayer->m_bForceRemove = true;

#ifdef ENH_SAVE_REMOTE_PLAYERS
		LevelStorage* pLevelStorage = m_pLevel->getLevelStorage();
		pLevelStorage->save(*pPlayer);
#endif

		// remove it from our world
		m_pLevel->removeEntity(pPlayer);
	}
	else if ((pPlayer = getPendingPlayerByGUID(guid)))
	{
		// Player was still loading
		m_pendingPlayers.erase(guid);

		// remove "it" from our heap
		delete pPlayer;
	}
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, LoginPacket* packet)
{
	if (!m_pLevel || !m_bAllowIncoming)
		return;

	puts_ignorable("LoginPacket");

	// if they're already online, fail
	if (getOnlinePlayerByGUID(guid))
	{
		LOG_E("That player is already in the world!");
		return;
	}

	RakNet::BitStream bs;

#if NETWORK_PROTOCOL_VERSION >= 3
	LoginStatusPacket::LoginStatus loginStatus = LoginStatusPacket::STATUS_SUCCESS;
	if (packet->m_clientNetworkVersion < NETWORK_PROTOCOL_VERSION_MIN)
	{
		loginStatus = LoginStatusPacket::STATUS_CLIENT_OUTDATED;
	}
	else if (packet->m_clientNetworkVersionMin > NETWORK_PROTOCOL_VERSION)
	{
		loginStatus = LoginStatusPacket::STATUS_SERVER_OUTDATED;
	}

	if (loginStatus != LoginStatusPacket::STATUS_SUCCESS)
	{
		m_pRakNetInstance->send(guid, bs, new LoginStatusPacket(loginStatus));
		return;
	}
#endif

	ServerPlayer* pPlayer = new ServerPlayer(m_pLevel, m_pLevel->getLevelData()->getGameType());
	pPlayer->m_guid = guid;
	pPlayer->m_name = std::string(packet->m_userName.C_String());

	GameMode* pGameMode = m_pMinecraft->getPlayerGameMode(*pPlayer);
	pGameMode->initPlayer(pPlayer);

#ifdef ENH_SAVE_REMOTE_PLAYERS
	LevelStorage* pLevelStorage = m_pLevel->getLevelStorage();
	pLevelStorage->load(*pPlayer);
#endif

	m_pendingPlayers[guid] = pPlayer;

	// Ensure Player is spawned above-ground
	{
		Vec3& pos = pPlayer->m_pos;
		while (pos.y > 0)
		{
			pPlayer->setPos(pos);
			if (pPlayer->canSpawn())
				break;
			pos.y++;
		}

		Vec3 pos2 = pos;
		pos2.y -= pPlayer->m_heightOffset;
		pPlayer->moveTo(pos2, pPlayer->m_rot);
	}

	// Send StartGamePacket
	{
		StartGamePacket sgp;
		sgp.m_seed = m_pLevel->getSeed();
		sgp.m_levelVersion = m_pLevel->getLevelData()->getStorageVersion();
		sgp.m_gameType = pPlayer->getPlayerGameType();
		sgp.m_entityId = pPlayer->m_EntityID;
		sgp.m_time = m_pLevel->getTime();
		sgp.m_pos = pPlayer->m_pos;
		sgp.m_pos.y -= pPlayer->m_heightOffset;

		sgp.m_reliability = RELIABLE_ORDERED;
		m_pRakNetInstance->send(guid, bs, sgp);
	}

#if NETWORK_PROTOCOL_VERSION <= 2
	// emulate a ReadyPacket being received
	handle(guid, (ReadyPacket*)nullptr);
#endif
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, ReadyPacket* packet)
{
	if (!m_pLevel)
		return;

	if (packet)
	{
		// nullptr is emulated
		puts_ignorable("ReadyPacket");
	}

	Player* pPlayer = popPendingPlayer(guid);
	if (!pPlayer)
	{
		LOG_E("We don't have a user associated with this player!");
		return;
	}

	RakNet::BitStream bs;

#if NETWORK_PROTOCOL_VERSION >= 3
	{
		SetTimePacket packet(m_pLevel->getTime());
		m_pRakNetInstance->send(guid, bs, packet);
	}
#endif

	// send the connecting player info about all other players in the world
	for (size_t i = 0; i < m_pLevel->m_players.size(); i++)
	{
		Player* player = m_pLevel->m_players[i];
		AddPlayerPacket app(player);
		bs.Reset();
		m_pRakNetInstance->send(guid, bs, app);
	}

	m_pLevel->addEntity(pPlayer);

	m_pMinecraft->m_pGui->addMessage(pPlayer->m_name + " joined the game");

#if NETWORK_PROTOCOL_VERSION >= 3
	// send the connecting player info about all entities in the world
	for (EntityMap::iterator it = m_pLevel->m_entities.begin(); it != m_pLevel->m_entities.end(); ++it)
	{
		Entity* entity = it->second;
		if (canReplicateEntity(entity))
		{
			Packet* packet = _getPacketForEntity(*entity);
			if (!packet)
				continue;
			bs.Reset();
			m_pRakNetInstance->send(guid, bs, packet);
		}
	}
#endif

	// was actually added in 11 (0.7.0), but I don't think anything's stopping us from doing this for older clients
#if NETWORK_PROTOCOL_VERSION >= 6 && defined(ENH_SAVE_REMOTE_PLAYERS)
	// send the connecting player's inventory
	if (!pPlayer->isCreative())
	{
		std::vector<ItemStack> items = pPlayer->m_pInventoryMenu->cloneItems(true);
		ContainerSetContentPacket cscp(0, items);
		bs.Reset();
		m_pRakNetInstance->send(guid, bs, cscp);
	}
#endif

	AddPlayerPacket app(pPlayer);
	bs.Reset();
	app.write(bs);
	// specifying GUID to avoid sending it to the connecting player
	m_pRakNetPeer->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, guid, true);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, MessagePacket* packet)
{
	OnlinePlayer* pOP = getOnlinePlayerByGUID(guid);
	if (!pOP)
	{
		LOG_W("MessagePacket: That jerk %s doesn't actually exist", guid.ToString());
		return;
	}

	// don't let players send empty messages
	std::string msg(packet->m_str.C_String());
	if (msg.empty())
		return;

	if (msg[0] == '/')
	{
		LOG_I("CMD: %s: %s", pOP->m_pPlayer->m_name.c_str(), msg.c_str());

		std::stringstream ss(msg);
		ss.get(); // skip the /
		std::vector<std::string> parms;
		std::string currparm, cmdname;
		// get cmd name
		std::getline(ss, cmdname, ' ');
		// get cmd parms
		while (std::getline(ss, currparm, ' '))
			parms.push_back(currparm);

		CommandFunction func;
		CommandMap::iterator iter = m_commands.find(cmdname);
		if (iter == m_commands.end())
		{
			sendMessage(pOP, "Unknown command. Type /? for a list of commands.");
			return;
		}

		func = iter->second;

		(this->*func)(pOP, parms);

		return;
	}

	LOG_I("MSG: <%s> %s", pOP->m_pPlayer->m_name.c_str(), msg.c_str());

	// send everyone the message
	std::string gameMessage = "<" + pOP->m_pPlayer->m_name + "> " + msg;
	displayGameMessage(gameMessage);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, MovePlayerPacket* packet)
{
	if (!m_pLevel) return;
	//puts_ignorable("MovePlayerPacket");
	VALIDATE_PLAYER_ACTION(packet->m_id);

	if (m_bStrictPlayerMovement)
	{
		_handleMovePlayer(player, packet);
	}
	else
	{
		player.lerpTo(packet->m_pos, packet->m_rot);
		redistributePacket(packet, guid);
	}
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, PlaceBlockPacket* packet)
{
	if (!m_pLevel) return;
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	TilePos pos = packet->m_pos;

	printf_ignorable("PlaceBlockPacket @ %d, %d, %d", pos.x, pos.y, pos.z);

	player.swing();

	TileID tileId = Tile::TransformToValidBlockId(packet->m_tileTypeId);
	Facing::Name face = (Facing::Name)packet->m_face;
	TileData data = packet->m_data;

	if (!m_pLevel->mayPlace(tileId, pos, true))
		return;

	if (m_pLevel->setTileAndData(pos, tileId, data))
	{
		Tile* pTile = Tile::tiles[tileId];
		pTile->setPlacedOnFace(m_pLevel, pos, face);
		pTile->setPlacedBy(m_pLevel, pos, &player);

		const Tile::SoundType* pSound = pTile->m_pSound;
		m_pLevel->playSound(pos + 0.5f, "step." + pSound->name, 0.5f * (pSound->volume + 1.0f), pSound->pitch * 0.8f);
	}

	redistributePacket(packet, guid);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, RemoveBlockPacket* packet)
{
	if (!m_pLevel) return;
	puts_ignorable("RemoveBlockPacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	player.swing();

	TilePos pos = packet->m_pos;
	Tile* pTile = Tile::tiles[m_pLevel->getTile(pos)];
	int auxValue = m_pLevel->getData(pos);

	m_pMinecraft->m_pParticleEngine->destroyEffect(pos);

	bool setTileResult = m_pLevel->setTile(pos, TILE_AIR);
	if (pTile && setTileResult)
	{
		const Tile::SoundType* pSound = pTile->m_pSound;
		m_pLevel->playSound(pos + 0.5f, "step." + pSound->name, 0.5f * (pSound->volume + 1.0f), pSound->pitch * 0.8f);

		if (player.isSurvival())
		{
#ifdef MOD_POCKET_SURVIVAL
			// 0.2.1
			ItemStack tileItem(pTile, 1, auxValue);
			if (pTile == Tile::grass || !player.m_pInventory->hasUnlimitedResource(tileItem))
			{
				pTile->spawnResources(m_pLevel, pos, auxValue);
			}
#else
			pTile->spawnResources(m_pLevel, pos, auxValue);
#endif
		}

		pTile->destroy(m_pLevel, pos, auxValue);

		// redistribute the packet only if needed
		redistributePacket(packet, guid);
	}
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, PlayerEquipmentPacket* packet)
{
	if (!m_pLevel) return;
	//puts_ignorable("PlayerEquipmentPacket");
	VALIDATE_PLAYER_ACTION(packet->m_playerID);

#ifdef FEATURE_SERVER_INVENTORIES
	// will need to be reworked for proper server-sided inventory support, pick the proper slot, not just any item
	player.m_pInventory->pickItem(packet->m_itemID, packet->m_itemAuxValue, C_MAX_HOTBAR_ITEMS);
#else
	player.m_pInventory->setSelectedItem(ItemStack(packet->m_itemID, 1, packet->m_itemAuxValue));
#endif

	redistributePacket(packet, guid);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, InteractPacket* packet)
{
	if (!m_pLevel) return;
	//puts_ignorable("InteractPacket");
	VALIDATE_PLAYER_ACTION(packet->m_sourceId);

	Entity* pTarget = m_pLevel->getEntity(packet->m_targetId);
	if (!pTarget)
		return;

	switch (packet->m_actionType)
	{
	case InteractPacket::INTERACT:
		player.swing();
		m_pMinecraft->getPlayerGameMode(player)->interact(&player, pTarget);
		break;
	case InteractPacket::ATTACK:
		player.swing();
		m_pMinecraft->getPlayerGameMode(player)->attack(&player, pTarget);
		break;
	default:
		LOG_W("Received unkown action in InteractPacket: %d", packet->m_actionType);
		break;
	}

	redistributePacket(packet, guid);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, UseItemPacket* packet)
{
	if (!m_pLevel) return;
	//puts_ignorable("UseItemPacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	bool onTile = packet->m_tileFace != 255;

	if (onTile)
	{
		Tile* pTile = Tile::tiles[m_pLevel->getTile(packet->m_tilePos)];
		if (pTile)
		{
			if (pTile == Tile::invisible_bedrock)
				return;

			// Interface with tile instead of using item
			if (pTile->use(m_pLevel, packet->m_tilePos, &player))
			{
				player.swing();
				return;
			}
		}
	}

	ItemStack& item = packet->m_item;

	// if we're getting a bad state from the client, odds are we finished off a consumable, so just do what Java does
	if (item.isEmpty())
		item = player.m_pInventory->getSelected();

	if (item.isEmpty())
		return;

	if (onTile)
	{
		item.useOn(&player, m_pLevel, packet->m_tilePos, (Facing::Name)packet->m_tileFace);
	}
	else
	{
		item.use(m_pLevel, &player);
	}

	player.swing();
}

// added specifically to allow Noteblocks to work, but ideally should just be a part of ServerPlayerGameMode
bool _startDestroyBlock(Level& level, Player& player, const TilePos& pos, Facing::Name face)
{
	ItemStack& item = player.getSelectedItem();
	if (!item.isEmpty() && item.getItem() == Item::bow)
		return true;

	TileID tile = level.getTile(pos);

	if (tile <= 0)
		return false;
	
	Tile::tiles[tile]->attack(&level, pos, &player);

	return true;
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, PlayerActionPacket* packet)
{
	if (!m_pLevel) return;
	puts_ignorable("PlayerActionPacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	switch (packet->m_action)
	{
	case PlayerActionPacket::START_DESTROY_BLOCK:
		_startDestroyBlock(*m_pLevel, player, packet->m_tilePos, packet->m_tileFace);
		//m_pMinecraft->getPlayerGameMode(player)->startDestroyBlock(&player, packet->m_tilePos, packet->m_tileFace);
		break;
	case PlayerActionPacket::STOP_DESTROY_BLOCK:
		//m_pMinecraft->getPlayerGameMode(player)->stopDestroyBlock(&player, packet->m_tilePos, packet->m_tileFace);
		break;
	case PlayerActionPacket::STOP_USING_ITEM:
		player.releaseUsingItem();
		break;
	default:
		LOG_W("Unsupported PlayerAction: %d", packet->m_action);
		break;
	}
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, RequestChunkPacket* packet)
{
	//puts_ignorable("RequestChunkPacket");

	if (packet->m_chunkPos.x == -9999)
	{
		m_pRakNetInstance->send(guid, new LevelDataPacket(m_pLevel));
		return;
	}

	LevelChunk* pChunk = m_pLevel->getChunk(packet->m_chunkPos);
	if (!pChunk)
	{
		LOG_E("No chunk at %d, %d", packet->m_chunkPos.x, packet->m_chunkPos.z);
		return;
	}

	// @NOTE: this allows the client to request empty chunks. Is that okay?
	ChunkDataPacket cdp(pChunk->m_chunkPos, pChunk);
	m_pRakNetInstance->send(guid, cdp);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, AnimatePacket* packet)
{
	if (!m_pLevel) return;
	//puts_ignorable("AnimatePacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	switch (packet->m_actionId)
	{
		case AnimatePacket::SWING:
		{
			player.swing();
			break;
		}
		case AnimatePacket::HURT:
		{
			player.animateHurt();
			break;
		}
		default:
		{
			LOG_W("Received unkown action in AnimatePacket: %d", packet->m_actionId);
			break;
		}
	}

	redistributePacket(packet, guid);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, RespawnPacket* packet)
{
	if (!m_pLevel) return;
	puts_ignorable("RespawnPacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	NetEventCallback::handle(*m_pLevel, guid, packet);

	redistributePacket(packet, guid);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, SendInventoryPacket* packet)
{
#ifndef FEATURE_SERVER_INVENTORIES
	if (!m_pLevel) return;
	puts_ignorable("SendInventoryPacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	player.m_pInventory->replace(packet->m_items);

	if (packet->m_extra == SendInventoryPacket::EXTRA_DROP_ALL)
		player.m_pInventory->dropAll();
#endif
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, DropItemPacket* packet)
{
	if (!m_pLevel) return;
	puts_ignorable("DropItemPacket");
	VALIDATE_PLAYER_ACTION(packet->m_entityId);

	player.drop(packet->m_item, packet->m_bRandomly);
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, ContainerClosePacket* packet)
{
	if (!m_pLevel) return;
	puts_ignorable("ContainerClosePacket");
	
	Player* pPlayer = _findPlayer(*m_pLevel, guid);
	if (!pPlayer /* || pPlayer->isLocalPlayer()*/) // assume all connected players are ServerPlayers
		return;

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	pServerPlayer->doCloseContainer();
}

void ServerSideNetworkHandler::handle(const RakNet::RakNetGUID& guid, ContainerSetSlotPacket* packet)
{
	if (!m_pLevel) return;
	puts_ignorable("ContainerSetSlotPacket");
	
	Player* pPlayer = _findPlayer(*m_pLevel, guid);
	if (!pPlayer)
		return;

	ContainerMenu* pContainerMenu = pPlayer->m_pContainerMenu;
	bool isInventory = false;

	// @HACK: LocalPlayer inventory seems to always have Container ID 0
	if (packet->m_containerId == 0)
	{
		pContainerMenu = pPlayer->m_pInventoryMenu;
		isInventory = true;
	}

	if (!pContainerMenu)
		return;

	if (pContainerMenu->m_containerId == packet->m_containerId
		|| (isInventory && packet->m_containerId == 0)) 
	{
		switch (pContainerMenu->m_containerType)
		{
		case Container::FURNACE:
		case Container::CONTAINER:
			pContainerMenu->setItem(packet->m_slotId, packet->m_item);
			break;
		default:
			break;
		}
	}
}

void ServerSideNetworkHandler::tileBrightnessChanged(const TilePos& pos)
{
}

void ServerSideNetworkHandler::tileChanged(const TilePos& pos)
{
	UpdateBlockPacket ubp;
	ubp.m_pos = pos;
	ubp.m_tileTypeId = m_pLevel->getTile(pos);
	ubp.m_data = m_pLevel->getData(pos);

	ubp.m_reliability = RELIABLE_ORDERED;
	ubp.m_channel = CHANNEL_TILE_EVENTS;
	m_pRakNetInstance->send(ubp);
}

void ServerSideNetworkHandler::timeChanged(uint32_t time)
{
	if ((time % (20 * NETWORK_TIME_SEND_FREQUENCY)) == 0)
	{
		m_pRakNetInstance->send(new SetTimePacket(time));
	}
}

void ServerSideNetworkHandler::entityAdded(Entity* entity)
{
	if (!canReplicateEntity(entity))
		return;

	Packet* packet = _getPacketForEntity(*entity);
	if (!packet)
		return;

	// @PARITY-PE: AddMobPacket is sent as RELIABLE_ORDERED in PE using redistributePacket. Any reason for this?
	m_pRakNetInstance->send(packet);
}

void ServerSideNetworkHandler::entityRemoved(Entity* entity)
{
	RemoveEntityPacket packet(entity->m_EntityID);
	redistributePacket(&packet, m_pRakNetInstance->m_guid);
}

void ServerSideNetworkHandler::levelEvent(const LevelEvent& event)
{
	LevelEventPacket pkt(event.id, event.pos, event.data);

	if (event.pPlayer)
	{
		redistributePacket(&pkt, event.pPlayer->m_guid);
	}
	else
	{
		redistributePacket(&pkt, m_pMinecraft->m_pLocalPlayer->m_guid);
	}
}

void ServerSideNetworkHandler::tileEvent(const TileEvent& event)
{
#if NETWORK_PROTOCOL_VERSION >= 5
	m_pRakNetInstance->send(new TileEventPacket(event.pos, event.b0, event.b1));
#endif
}

void ServerSideNetworkHandler::allowIncomingConnections(bool b)
{
	if (b)
	{
		m_pRakNetInstance->announceServer(m_pMinecraft->getOptions()->m_playerName.get());
	}
	else
	{
		m_pRakNetInstance->announceServer("");
	}

	m_bAllowIncoming = b;
}

Player* ServerSideNetworkHandler::popPendingPlayer(const RakNet::RakNetGUID& guid)
{
	if (!m_pLevel)
	{
		LOG_E("Could not add player since Level is NULL!");
		return nullptr;
	}

	Player* pPlayer = getPendingPlayerByGUID(guid);
	if (pPlayer)
	{
		m_pendingPlayers.erase(guid);
		m_onlinePlayers[guid] = new OnlinePlayer(pPlayer, guid);
	}

	return pPlayer;
}

void ServerSideNetworkHandler::displayGameMessage(const std::string& msg)
{
	m_pMinecraft->m_pGui->addMessage(msg);
	m_pRakNetInstance->send(new MessagePacket(msg));
}

void ServerSideNetworkHandler::sendMessage(const RakNet::RakNetGUID& guid, const std::string& msg)
{
	if (m_pRakNetPeer->GetMyGUID() == guid)
	{
		m_pMinecraft->m_pGui->addMessage(msg);
		return;
	}

	m_pRakNetInstance->send(guid, new MessagePacket(msg));
}

void ServerSideNetworkHandler::sendMessage(OnlinePlayer* player, const std::string& msg)
{
	sendMessage(player->m_guid, msg);
}

void ServerSideNetworkHandler::redistributePacket(Packet* packet, const RakNet::RakNetGUID& source)
{
	RakNet::BitStream bs;
	packet->write(bs);

	m_pRakNetPeer->Send(&bs, packet->m_priority, packet->m_reliability, packet->m_channel, source, true);
}

OnlinePlayer* ServerSideNetworkHandler::getOnlinePlayerByGUID(const RakNet::RakNetGUID& guid)
{
	OnlinePlayerMap::iterator iter = m_onlinePlayers.find(guid);
	if (iter == m_onlinePlayers.end())
		return nullptr;

	return iter->second;
}

Player* ServerSideNetworkHandler::getPendingPlayerByGUID(const RakNet::RakNetGUID& guid)
{
	PlayerMap::iterator iter = m_pendingPlayers.find(guid);
	if (iter == m_pendingPlayers.end())
		return nullptr;

	return iter->second;
}

bool ServerSideNetworkHandler::canReplicateEntity(const Entity* pEntity) const
{
	if (!pEntity || pEntity->isPlayer())
		return false;

#if NETWORK_PROTOCOL_VERSION <= 5
	if (!pEntity->isMob() && !pEntity->getDescriptor().isType(EntityType::ITEM))
		return false;
#endif

	if (pEntity->getDescriptor().isType(EntityType::FALLING_TILE))
		return false;

	// All clients on V3 will just crash if an unknown 
	// EntityType ID is replicated in an AddMobPacket.
	// V4 and above has the neccessary nullptr check to not need this.
#if NETWORK_PROTOCOL_VERSION <= 3
	EntityType::ID entityTypeId = pEntity->getDescriptor().getEntityType().getId();
	switch (entityTypeId)
	{
	case EntityType::SHEEP:
	case EntityType::ZOMBIE:
	case EntityType::PIG:
		return true;
	default:
		return false;
	}
#endif

	return true;
}

///////////////// In-Game Commands /////////////////

void ServerSideNetworkHandler::setupCommands()
{
	m_commands["?"]        = &ServerSideNetworkHandler::commandHelp;
	m_commands["help"]     = &ServerSideNetworkHandler::commandHelp;
	m_commands["stats"]    = &ServerSideNetworkHandler::commandStats;
	m_commands["time"]     = &ServerSideNetworkHandler::commandTime;
	m_commands["seed"]     = &ServerSideNetworkHandler::commandSeed;
	m_commands["tp"]       = &ServerSideNetworkHandler::commandTP;
	m_commands["summon"]   = &ServerSideNetworkHandler::commandSummon;
	m_commands["gamemode"] = &ServerSideNetworkHandler::commandGamemode;
	m_commands["give"]     = &ServerSideNetworkHandler::commandGive;
	m_commands["clear"]    = &ServerSideNetworkHandler::commandClear;
}

bool ServerSideNetworkHandler::_checkPermissions(OnlinePlayer* player)
{
	if (player->m_pPlayer != m_pMinecraft->m_pLocalPlayer)
	{
		sendMessage(player, "Sorry, only the host can use this command at the moment");
		return false;
	}

	return true;
}

bool ServerSideNetworkHandler::_validateNum(OnlinePlayer* player, int value, int min, int max)
{
	if (value < min)
	{
		sendMessage(player, Util::format("The number you have entered (%d) is too small, it must be at least %d", value, min));
		return false;
	}
	else if (value > max)
	{
		sendMessage(player, Util::format("The number you have entered (%d) is too big, it must be at most %d", value, max));
		return false;
	}

	return true;
}

void ServerSideNetworkHandler::commandHelp(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	std::stringstream ss;
	ss << ">> Available commands:";

	for (CommandMap::iterator it = m_commands.begin(); it != m_commands.end(); ++it)
	{
		ss << " /" << it->first;
	}

	sendMessage(player, ss.str());
}

void ServerSideNetworkHandler::commandStats(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;

	std::stringstream ss;
	ss << "Server uptime: " << getTimeS() << " seconds.\n";
	ss << "Host's name: " << m_pMinecraft->m_pUser->m_name << "\n";

	size_t nPlayers = m_onlinePlayers.size();
	if (nPlayers == 1)
		ss << "There is 1 player online.";
	else
		ss << "There are " << nPlayers << " players online.";

	sendMessage(player, ss.str());
}

void ServerSideNetworkHandler::commandTime(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;

	if (!parms.empty())
	{
		int t = 0;
		if (!sscanf(parms[0].c_str(), "%d", &t))
		{
			sendMessage(player, "Usage: /time <value>");
			return;
		}

		if (!_checkPermissions(player)) return;

		m_pLevel->setTime(t);

		sendMessage(player, "Time has been set to " + parms[0]);

		return;
	}

	std::stringstream ss;
	ss << "In-game time: ";
	ss << m_pLevel->getTime();
	ss << ". Day " << m_pLevel->getTime() / 24000;
	sendMessage(player, ss.str());
}

void ServerSideNetworkHandler::commandSeed(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;

	std::stringstream ss;
	ss << "World generation seed: ";
	ss << m_pLevel->getSeed();
	sendMessage(player, ss.str());
}

void ServerSideNetworkHandler::commandTP(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;
    
	if (parms.size() != 3)
	{
		sendMessage(player, "Usage: /tp <x> <y> <z>");
		return;
	}
    
	if (!_checkPermissions(player)) return;
    
	Vec3 pos = player->m_pPlayer->getPos(1.0f);
    
	std::stringstream ss;
	if (parms[0] != "~")
	{
		ss.str(parms[0]);
		ss.clear();
		ss >> pos.x;
	}
	if (parms[1] != "~")
	{
		ss.str(parms[1]);
		ss.clear();
		ss >> pos.y;
	}
	if (parms[2] != "~")
	{
		ss.str(parms[2]);
		ss.clear();
		ss >> pos.z;
	}
    
	ss.clear();
    
	player->m_pPlayer->setPos(pos);
	pos = player->m_pPlayer->getPos(1.0f);

	ss << "Teleported to " << pos.x << ", " << pos.y << ", " << pos.z;
    
	sendMessage(player, ss.str());
}

void ServerSideNetworkHandler::commandSummon(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;

	const int parmsSize = parms.size();

	if (parmsSize != 1 && parmsSize != 4 && parmsSize != 5)
	{
		sendMessage(player, "Usage: /summon <entity> [x] [y] [z] [amount]");
		return;
	}

	if (!_checkPermissions(player)) return;

	std::string entityName;
	std::stringstream ss;
	ss.str(parms[0]);
	ss >> entityName;

	const EntityTypeDescriptor* descriptor = EntityTypeDescriptor::GetByEntityTypeName(entityName);
	if (descriptor != nullptr)
	{
		Vec3 pos = player->m_pPlayer->getPos(1.0f);
		pos.y -= player->m_pPlayer->m_heightOffset + player->m_pPlayer->m_ySlideOffset;

		if (parmsSize >= 4)
		{
			if (parms[1] != "~")
			{
				ss.str(parms[1]);
				ss.clear();
				ss >> pos.x;
			}
			if (parms[2] != "~")
			{
				ss.str(parms[2]);
				ss.clear();
				ss >> pos.y;
			}
			if (parms[3] != "~")
			{
				ss.str(parms[3]);
				ss.clear();
				ss >> pos.z;
			}
		}

		int amount = 1;
		if (parmsSize >= 5)
		{
			ss.str(parms[4]);
			ss.clear();
			ss >> amount;
		}

		ss.clear();
		if (descriptor->getEntityType() != EntityType::UNKNOWN)
		{
			bool success = false;

			for (int i = 0; i++ < amount;)
			{
				Mob* mob = MobFactory::CreateMob(descriptor->getEntityType().getId(), m_pLevel);
				if (mob == nullptr)
				{
					ss << "Unable to summon object";
					break;
				}
				mob->setPos(pos);
				m_pLevel->addEntity(mob);
				if (!success) success = true;
			}

			if (success)
			{
				ss << "Object successfully summoned";
			}
		}
		else
		{
			ss << "Unable to summon unknown entity type";
		}
	}
	else
	{
		ss.clear();
		ss << "Unable to summon invalid entity type";
	}

	sendMessage(player, ss.str());
}

void ServerSideNetworkHandler::commandGamemode(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;
    
	if (parms.size() != 1)
	{
		sendMessage(player, "Usage: /gamemode <mode>");
		return;
	}
	/*if (parms.size() < 1 || parms.size() > 2)
	{
		sendMessage(player, "Usage: /gamemode <mode> [player]");
		return;
	}*/
    
	if (!_checkPermissions(player)) return;
    
	GameType gameMode;
	std::stringstream ss;
	ss.str(parms[0]);
	int tmp;
	ss >> tmp;
	gameMode = static_cast<GameType>(tmp);

	if (!_validateNum(player, gameMode, GAME_TYPES_MIN, GAME_TYPES_MAX))
		return;
    
	player->m_pPlayer->setPlayerGameType(gameMode);
    
	sendMessage(player, "Your game mode has been updated");
}

void ServerSideNetworkHandler::commandGive(OnlinePlayer * player, const std::vector<std::string>&parms)
{
	const std::string usage = "Usage: /give <item> [amount] [data]";

	if (!m_pLevel)
		return;
	if (parms.size() < 1 || parms.size() > 3)
	{
		sendMessage(player, usage);
		return;
	}

	if (!_checkPermissions(player)) return;

	int id = 0;
	int amount = 1;
	int auxValue = 0;
	if (sscanf(parms[0].c_str(), "%d", &id))
	{
		if (!_validateNum(player, id, 1, C_MAX_ITEMS-1))
			return;
	}
	else
	{
		sendMessage(player, usage);
		return;
	}
	if (parms.size() >= 2 && sscanf(parms[1].c_str(), "%d", &amount))
	{
		if (!_validateNum(player, amount, 1, 64))
			return;
	}
	if (parms.size() >= 3 && sscanf(parms[2].c_str(), "%d", &auxValue))
	{
		if (!_validateNum(player, auxValue, 0, 255))
			return;
	}
	Item* item = Item::items[id];
	if (!item || id >= 512 || id < 0)
	{
		sendMessage(player, "There is no such item with ID " + parms[0]);
		return;
	}

	Inventory* pInventory = player->m_pPlayer->m_pInventory;

	pInventory->addTestItem(item->m_itemID, amount, auxValue);

	sendMessage(player, Util::format("Given %s (ID %d) * %d to %s", item->getName().c_str(), item->m_itemID, amount, player->m_pPlayer->m_name.c_str()));
	return;
}

void ServerSideNetworkHandler::commandClear(OnlinePlayer* player, const std::vector<std::string>& parms)
{
	if (!m_pLevel)
		return;
		
	if (!_checkPermissions(player)) return;

	Inventory* pInventory = player->m_pPlayer->m_pInventory;

	pInventory->clear(); // calling "clear" will delete all of our slots

	sendMessage(player, "Your inventory has been cleared.");
	return;
}
