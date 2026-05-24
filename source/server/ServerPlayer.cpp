#include "ServerPlayer.hpp"
#include "common/Logger.hpp"
#include "network/packets/SetHealthPacket.hpp"
#include "network/packets/TakeItemEntityPacket.hpp"
#include "network/packets/SendInventoryPacket.hpp"
#include "network/packets/ContainerOpenPacket.hpp"
#include "network/packets/ContainerClosePacket.hpp"
#include "network/packets/ContainerSetSlotPacket.hpp"
#include "network/packets/ContainerSetDataPacket.hpp"
#include "network/packets/ContainerSetContentPacket.hpp"
#include "network/RakNetInstance.hpp"
#include "world/inventory/CraftingMenu.hpp"
#include "world/inventory/FurnaceMenu.hpp"
#include "world/inventory/ChestMenu.hpp"
#include "world/level/Level.hpp"
#include "world/tile/entity/FurnaceTileEntity.hpp"
#include "world/inventory/Slot.hpp"

ServerPlayer::ServerPlayer(Level* pLevel, GameType playerGameType)
	: Player(pLevel, playerGameType)
{
	m_lastHealth = -999; // -99999999 on Java
	m_containerId = 0;

	m_pInventoryMenu->addSlotListener(this);
}

ServerPlayer::~ServerPlayer()
{
	doCloseContainer();
}

void ServerPlayer::_nextContainerCounter()
{
	m_containerId++;

	if (m_containerId >= 100)
		m_containerId = 0;
}

void ServerPlayer::tick()
{
	Player::tick();

	if (m_pContainerMenu)
		m_pContainerMenu->broadcastChanges();

	if (m_health != m_lastHealth)
	{
		m_pLevel->m_pRakNetInstance->send(m_guid, new SetHealthPacket(m_health));
		m_lastHealth = m_health;
	}
}

void ServerPlayer::startCrafting(const TilePos& pos)
{
	_nextContainerCounter();

#if NETWORK_PROTOCOL_VERSION >= 5
	m_pLevel->m_pRakNetInstance->send(m_guid, new ContainerOpenPacket(m_containerId, Container::CRAFTING, "Crafting", 9));
#endif

	setContainerMenu(new CraftingMenu(m_pInventory, pos, m_pLevel));
}

void ServerPlayer::openContainer(Container* container)
{
	//LOG_I("Client is opening a container");

	_nextContainerCounter();

#if NETWORK_PROTOCOL_VERSION >= 5
	m_pLevel->m_pRakNetInstance->send(
		m_guid,
		new ContainerOpenPacket(
			m_containerId, Container::CONTAINER,
			container->getName(), container->getContainerSize()
		)
	);
#endif

	setContainerMenu(new ChestMenu(m_pInventory, container));
}

void ServerPlayer::closeContainer()
{
	//LOG_I("Client is closing a container");

#if NETWORK_PROTOCOL_VERSION >= 5
	m_pLevel->m_pRakNetInstance->send(m_guid, new ContainerClosePacket(m_pContainerMenu->m_containerId));
#endif

	doCloseContainer();
}

void ServerPlayer::openFurnace(FurnaceTileEntity* furnace)
{
	//LOG_I("Client is opening a furnace");

	_nextContainerCounter();

#if NETWORK_PROTOCOL_VERSION >= 5
	m_pLevel->m_pRakNetInstance->send(
		m_guid,
		new ContainerOpenPacket(
			m_containerId, Container::FURNACE,
			furnace->getName(), furnace->getContainerSize()
		)
	);
#endif

	setContainerMenu(new FurnaceMenu(m_pInventory, furnace));
}

void ServerPlayer::take(Entity* pEnt, int count)
{
	m_pLevel->m_pRakNetInstance->send(m_guid,
		new TakeItemEntityPacket(pEnt->m_EntityID, m_EntityID));

	Player::take(pEnt, count);
}

void ServerPlayer::checkFallDamage(float ya, bool onGround)
{
}

void ServerPlayer::refreshContainer(ContainerMenu* menu, const std::vector<ItemStack>& items)
{
#if NETWORK_PROTOCOL_VERSION >= 5
	m_pLevel->m_pRakNetInstance->send(m_guid, new ContainerSetContentPacket(menu->m_containerId, items));
	// @PARITY-JAVA: Not called on MCPE
	//m_pLevel->m_pRakNetInstance->send(m_guid, new ContainerSetSlotPacket(-1, -1, m_pInventory->getCarried()));
#endif
}

void ServerPlayer::slotChanged(ContainerMenu* menu, Container::SlotID slotId, Slot* slot, ItemStack& item, bool isResultSlot)
{
#if NETWORK_PROTOCOL_VERSION >= 5
	if (!isResultSlot)
	{
#ifndef FEATURE_SERVER_INVENTORIES
		// @TODO: See my gripes in ContainerMenu::slotChanged
		// But ultimately this is a bandaid for the fact that the client has authority over the inventory in PE
		if (slot->m_group == Slot::INVENTORY || slot->m_group == Slot::HOTBAR)
			return;
#endif

		m_pLevel->m_pRakNetInstance->send(m_guid, new ContainerSetSlotPacket(menu->m_containerId, slotId, item));
	}
#endif
}

void ServerPlayer::setContainerData(ContainerMenu* menu, int id, int value)
{
#if NETWORK_PROTOCOL_VERSION >= 5
	m_pLevel->m_pRakNetInstance->send(m_guid, new ContainerSetDataPacket(menu->m_containerId, id, value));
#endif
}

void ServerPlayer::doCloseContainer()
{
	if (m_pContainerMenu)
		m_pContainerMenu->removed(this);
	else
		LOG_W("Container is missing @ doCloseContainer!");

	setContainerMenu(m_pInventoryMenu); // m_pInventoryMenu on Java, nullptr on Pocket
}

void ServerPlayer::setContainerMenu(ContainerMenu* menu)
{
	if (m_pContainerMenu == menu)
		return;

	if (m_pContainerMenu != m_pInventoryMenu)
		SAFE_DELETE(m_pContainerMenu);

	m_pContainerMenu = menu;

	// we shouldn't be changing the containerId of the InventoryMenu
	if (menu && menu != m_pInventoryMenu)
	{
		m_pContainerMenu->m_containerId = m_containerId;
		m_pContainerMenu->addSlotListener(this);
		refreshContainer(m_pContainerMenu, m_pContainerMenu->cloneItems());
		m_pContainerMenu->broadcastChanges();
	}
}

void ServerPlayer::doCheckFallDamage(float ya, bool onGround)
{
	Player::checkFallDamage(ya, onGround);
}
