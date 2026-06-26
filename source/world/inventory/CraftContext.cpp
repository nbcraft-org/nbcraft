#include "CraftContext.hpp"
#include "world/item/crafting/ShapedRecipe.hpp"
#include "world/item/Inventory.hpp"

void handleCompactItemStackListAdd(std::vector<ItemStack>& compactList, const ItemStack& item)
{
	for (size_t i = 0; i < compactList.size(); ++i)
	{
		ItemStack& itemStack = compactList[i];
		if (itemStack.getId() == item.getId() && itemStack.getAuxValue() == item.getAuxValue())
		{
			itemStack.m_count += item.m_count;
			return;
		}
	}
	compactList.push_back(item);
}

void CraftContext::HandleCompactItemStackList(std::vector<ItemStack>& compactList, const std::vector<ItemStack>& items)
{
	for (size_t i = 0; i < items.size(); ++i)
	{
		const ItemStack& item = items[i];
		if (item.isEmpty()) continue;
		handleCompactItemStackListAdd(compactList, item);
	}
}

void CraftContext::HandleCompactInventoryList(std::vector<ItemStack>& compactList, Inventory* inventory, const ItemStack& carriedItem)
{
	HandleCompactItemStackList(compactList, inventory->getItems());
	if (!carriedItem.isEmpty()) handleCompactItemStackListAdd(compactList, carriedItem);
}

std::vector<ItemStack> CraftContext::getShapedIngredients(Recipe* recipe)
{
	if (!recipe) return std::vector<ItemStack>();

	if (!recipe->isShaped())
		return recipe->getIngredients();

	ShapedRecipe* shapedRecipe = (ShapedRecipe*)recipe;

	const int gridDimension = bIs2x2 ? 2 : 3;
	const int rcpWidth = shapedRecipe->getWidth();
	const int rcpHeight = shapedRecipe->getHeight();

	std::vector<ItemStack> ingredientsGrid(gridDimension * gridDimension);

	int rcpDimension = Mth::Max(rcpWidth, rcpHeight);
	if (rcpDimension > gridDimension) return ingredientsGrid;
	std::vector<ItemStack> ingredients(rcpDimension * rcpDimension);
	for (size_t i = 0; i < recipe->getIngredients().size(); i++)
		ingredients[rcpWidth < rcpHeight ? (i / rcpWidth) * rcpHeight + (i % rcpWidth) : i] = recipe->getIngredients()[i];
	for (size_t i = 0; i < ingredients.size(); i++)
		ingredientsGrid[i > 1 && gridDimension > rcpDimension ? i + 1 : i] = ingredients[i];

	return ingredientsGrid;
}

bool CraftContext::craft(Recipe* recipe)
{
	if (!canCraft(recipe)) return false;

	const std::vector<ItemStack>& invList = recipe->getIngredients();

	for (std::vector<ItemStack>::const_iterator it = invList.begin(); it != invList.end(); ++it)
	{
		const ItemStack& ing = *it;
		if (ing.isEmpty()) continue;
		pInventory->removeIngredient(ing);
	}

	ItemStack resultItem = recipe->getResultItem();
	pInventory->add(resultItem);

	return true;
}

bool CraftContext::canCraft(Recipe* recipe)
{
	if (!recipe) return false;

	bool canCraft = true;
	bool isAllEmpty = true;
	bool setsWarning = shouldSetWarning(recipe);
	const std::vector<ItemStack>& ings = getShapedIngredients(recipe);
	std::vector<ItemStack> invList;
	HandleCompactInventoryList(invList, pInventory, pInventory->getCarried());

	for (size_t i1 = 0; i1 < ings.size(); i1++)
	{
		const ItemStack& ing = ings[i1];
		if (ing.isEmpty()) continue;
		isAllEmpty = false;
		bool foundIng = false;
		for (std::vector<ItemStack>::iterator it = invList.begin(); it != invList.end(); ++it)
		{
			ItemStack& itemStack = *it;
			if (!itemStack.isEmpty() && ing.sameIngredient(itemStack))
			{
				itemStack.shrink(1);
				if (setsWarning)
				{
					setWarning(i1 + 1, false);
					setWarning(0, false);
				}
				foundIng = true;
				break;
			}
		}

		if (foundIng) continue;

		canCraft = false;
		if (!setsWarning) return canCraft && !isAllEmpty;
		else
		{
			setWarning(i1 + 1, true);
			setWarning(0, true);
		}
	}
	return canCraft && !isAllEmpty;
}
