#pragma once

#include "world/item/crafting/Recipes.hpp"
#include "world/inventory/CraftContext.hpp"

struct SlotDisplay;

struct ClientCraftContext : public CraftContext
{
	std::vector<SlotDisplay>* slotDisplays;
	std::vector<Recipes::RecipeList> groupRecipes;
	Recipes::RecipeList selectedRecipes;
	int8_t selectionOffset;

	ClientCraftContext(Inventory* inv, std::vector<SlotDisplay>* slotDisplays, bool is2x2) 
		: CraftContext(inv, is2x2)
		, slotDisplays(slotDisplays)
		, selectionOffset(0)
	{
	}

	bool craft(Recipe*) override;

	Recipe* getSelectedRecipe()
	{
		int selectedIndex = selectionOffset == 1 ? 1 : selectionOffset == -1 ? selectedRecipes.size() - 1 : 0;
		return selectedIndex >= 0 && size_t(selectedIndex) < selectedRecipes.size() ? selectedRecipes[selectedIndex] : nullptr;
	};

	void scrollSelectedRecipes(int offset)
	{
		if (selectedRecipes.empty()) return;

		int n = int(selectedRecipes.size());

		offset %= n;
		if (offset < 0)
			offset += n;

		std::rotate(selectedRecipes.begin(), selectedRecipes.end() - offset, selectedRecipes.end());
	}

	bool shouldSetWarning(Recipe* recipe) override { return getSelectedRecipe() == recipe; }
	void setWarning(int, bool) override;
};
