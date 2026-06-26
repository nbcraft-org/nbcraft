#pragma once

#include <vector>

class Inventory;
class Recipe;
class ItemStack;

struct CraftContext
{
	Inventory* pInventory;
	bool bIs2x2;

	CraftContext(Inventory* inv, bool is2x2)
		: pInventory(inv)
		, bIs2x2(is2x2)
	{
	}

	static void HandleCompactItemStackList(std::vector<ItemStack>& compactList, const std::vector<ItemStack>& items);
	static void HandleCompactInventoryList(std::vector<ItemStack>& compactList, Inventory* inventory, const ItemStack& carriedItem);

	std::vector<ItemStack> getShapedIngredients(Recipe* recipe);

	virtual bool craft(Recipe*);

	bool canCraft(Recipe* recipe);

	//Display methods, overriden by ClientCraftContext

	virtual bool shouldSetWarning(Recipe*) { return false; }
	virtual void setWarning(int, bool) {}
};
