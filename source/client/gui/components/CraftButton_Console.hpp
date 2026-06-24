#pragma once

#include "Button.hpp"
#include "client/gui/screens/inventory/ClientCraftContext.hpp"

class CraftButton_Console : public Button
{
public:
	CraftButton_Console(int slotIndex, ClientCraftContext* craftContext, int size = 54);

protected:
	void _renderResultItem(Minecraft*, Recipe*, int x, int y);
	bool _canScroll();

public:
	void renderBg(Minecraft*, const MenuPointer& pointer) override;
	void render(Minecraft* pMinecraft, const MenuPointer& pointer) override;
	bool isHovered(Minecraft*, const MenuPointer& pointer) override;
	void pressed(Minecraft*, const MenuPointer& pointer) override;
	void handleScroll(float force) override;
	bool areaNavigation(Minecraft*, AreaNavigation::Direction) override;

	Recipe* getRecipe()
	{
		if (isSelected())
			return m_pCraftContext->selectedRecipes.empty() ? nullptr : m_pCraftContext->selectedRecipes[0];

		std::vector<Recipes::RecipeList>& recipeLists = m_pCraftContext->groupRecipes;
		return size_t(m_slotIndex) < recipeLists.size() ? recipeLists[m_slotIndex][0] : nullptr;
	}

public:
	int m_slotIndex;
	ClientCraftContext* m_pCraftContext;
};

