#include "ClientCraftContext.hpp"
#include "client/gui/screens/inventory/ContainerScreen.hpp"

bool ClientCraftContext::craft(Recipe* recipe)
{
	if (CraftContext::craft(recipe)) 
	{
		// this feels very wrong...
		Minecraft* mc = ((LocalPlayer*)pInventory->m_pPlayer)->m_pMinecraft;
		mc->m_pSoundEngine->playUI("random.pop");
		return true;
	}

	return false;
}

void ClientCraftContext::setWarning(int id, bool warning)
{
	slotDisplays->at(id).bIsWarning = warning;
}
