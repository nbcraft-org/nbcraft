#include "ScreenChooser.hpp"
#include "client/app/Minecraft.hpp"

#include "screens/StartMenuScreen.hpp"
#include "screens/PauseScreen.hpp"
#include "screens/inventory/CraftingScreen.hpp"
#include "screens/inventory/ChestScreen.hpp"
#include "screens/inventory/FurnaceScreen.hpp"
#include "screens/inventory/CreativeScreen.hpp"
#include "screens/OptionsScreen.hpp"
#include "screens/CreateWorldScreen.hpp"
#include "screens/ProgressScreen.hpp"
#include "screens/CreditsScreen.hpp"
#include "screens/DeathScreen.hpp"
#include "ScreenChooser_Pocket.hpp"
#include "ScreenChooser_Console.hpp"

ScreenChooser::ScreenChooser(Minecraft* mc, UITheme uiTheme) :
	m_pMinecraft(mc)
	, m_uiTheme(uiTheme)
{
}

ScreenChooser::~ScreenChooser()
{
}

void ScreenChooser::pushStartScreen()
{
	m_pMinecraft->setScreen(new StartMenuScreen);
}

void ScreenChooser::pushPauseScreen()
{
	m_pMinecraft->setScreen(new PauseScreen);
}

void ScreenChooser::pushCreateWorldScreen(Screen* parent)
{
	m_pMinecraft->setScreen(new CreateWorldScreen(parent));
}

void ScreenChooser::pushOptionsScreen(Screen* parent)
{
	m_pMinecraft->setScreen(new OptionsScreen(parent));
}

void ScreenChooser::pushProgressScreen()
{
	m_pMinecraft->setScreen(new ProgressScreen);
}

void ScreenChooser::pushCreativeScreen()
{
	m_pMinecraft->setScreen(new CreativeScreen(m_pMinecraft->m_pLocalPlayer->m_pInventory));
}

void ScreenChooser::pushCraftingScreen(Player* player, const TilePos& pos)
{
	m_pMinecraft->setScreen(new CraftingScreen(player->m_pInventory, pos, player->m_pLevel));
}

void ScreenChooser::pushFurnaceScreen(Player* player, FurnaceTileEntity* furnace)
{
	m_pMinecraft->setScreen(new FurnaceScreen(player->m_pInventory, furnace));
}

void ScreenChooser::pushChestScreen(Player* player, Container* container)
{
	m_pMinecraft->setScreen(new ChestScreen(player->m_pInventory, container));
}

void ScreenChooser::pushCreditsScreen(Screen* parent)
{
	m_pMinecraft->setScreen(new CreditsScreen(parent));
}

void ScreenChooser::pushDeathScreen()
{
	m_pMinecraft->setScreen(new DeathScreen());
}

ScreenChooser* ScreenChooser::Create(Minecraft* mc)
{
	switch (mc->getOptions()->getUiTheme())
	{
	case UI_POCKET:
		return new ScreenChooser_Pocket(mc);
	case UI_CONSOLE:
		return new ScreenChooser_Console(mc);
	default:
		return new ScreenChooser(mc, mc->getOptions()->getUiTheme());
	}
}
