#include "ClientCraftContext.hpp"
#include "client/gui/screens/inventory/ContainerScreen.hpp"

bool ClientCraftContext::craft(Recipe* recipe)
{
	Minecraft& mc = Minecraft::singleton();
	if (CraftContext::craft(recipe)) 
	{
		mc. m_pSoundEngine->playUI("random.pop");
		return true;
	}
	else
		mc.m_pSoundEngine->playUI(C_SOUND_UI_CRAFTFAIL);

	return false;
}

void ClientCraftContext::setWarning(int id, bool warning)
{
	slotDisplays->at(id).isWarning = warning;
}
