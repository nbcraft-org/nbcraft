#include "ScreenChooser_Pocket.hpp"
#include "client/app/Minecraft.hpp"

#include "screens/StartMenuScreen_Pocket.hpp"

ScreenChooser_Pocket::ScreenChooser_Pocket(Minecraft* mc) : ScreenChooser(mc, UI_POCKET)
{
}

ScreenChooser_Pocket::~ScreenChooser_Pocket()
{
}

void ScreenChooser_Pocket::pushStartScreen()
{
	m_pMinecraft->setScreen(new StartMenuScreen_Pocket);
}
