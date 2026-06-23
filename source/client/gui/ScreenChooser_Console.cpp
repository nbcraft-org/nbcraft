#include "ScreenChooser_Console.hpp"
#include "client/app/Minecraft.hpp"

#include "screens/StartMenuScreen_Console.hpp"
#include "screens/PauseScreen_Console.hpp"
#include "screens/inventory/CreativeScreen_Console.hpp"
#include "screens/inventory/CraftingScreen_Console.hpp"
#include "screens/inventory/ClassicCraftingScreen_Console.hpp"
#include "screens/OptionsScreen_Console.hpp"
#include "screens/CreateWorldScreen_Console.hpp"
#include "screens/ProgressScreen_Console.hpp"
#include "screens/DeathScreen_Console.hpp"

ScreenChooser_Console::ScreenChooser_Console(Minecraft* mc) : ScreenChooser(mc, UI_CONSOLE)
{
}

ScreenChooser_Console::~ScreenChooser_Console()
{
}

void ScreenChooser_Console::pushStartScreen()
{
	m_pMinecraft->setScreen(new StartMenuScreen_Console);
}

void ScreenChooser_Console::pushPauseScreen()
{
	m_pMinecraft->setScreen(new PauseScreen_Console);
}

void ScreenChooser_Console::pushOptionsScreen(Screen* parent)
{
	m_pMinecraft->setScreen(new OptionsScreen_Console(parent));
}

void ScreenChooser_Console::pushProgressScreen()
{
	m_pMinecraft->setScreen(new ProgressScreen_Console);
}

void ScreenChooser_Console::pushPlayerCraftingScreen(Player* player)
{
	if (m_pMinecraft->getOptions()->m_classicCrafting.get())
		ScreenChooser::pushPlayerCraftingScreen(player);
	else
		m_pMinecraft->setScreen(new CraftingScreen_Console(player->m_pInventory, player->m_pos, player->m_pLevel, true));
}

void ScreenChooser_Console::pushCreativeScreen(Player* player)
{
	m_pMinecraft->setScreen(new CreativeScreen_Console(player->m_pInventory));
}

void ScreenChooser_Console::pushCreateWorldScreen(Screen* parent)
{
	m_pMinecraft->setScreen(new CreateWorldScreen_Console(*m_pMinecraft->getOptions(), parent));
}

void ScreenChooser_Console::pushCraftingScreen(Player* player, const TilePos& pos)
{
	if (m_pMinecraft->getOptions()->m_classicCrafting.get())
		m_pMinecraft->setScreen(new ClassicCraftingScreen_Console(player->m_pInventory, pos, player->m_pLevel));
	else
		m_pMinecraft->setScreen(new CraftingScreen_Console(player->m_pInventory, pos, player->m_pLevel, false));
}

void ScreenChooser_Console::pushDeathScreen()
{
	m_pMinecraft->setScreen(new DeathScreen_Console);
}
