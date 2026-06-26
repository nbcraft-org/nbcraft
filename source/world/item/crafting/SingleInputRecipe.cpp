#include "SingleInputRecipe.hpp"

SingleInputRecipe::SingleInputRecipe(const ItemStack& ingredient, const ItemStack& result) :
    m_result(result)
{
    m_ingredients.push_back(ingredient);
}

SingleInputRecipe::~SingleInputRecipe()
{
}

bool SingleInputRecipe::matches(Container* container)
{
    return matches(container->getItem(0));
}

bool SingleInputRecipe::matches(const ItemStack& input)
{
    return input.getId() == m_ingredients[0].getId() && (m_ingredients[0].getAuxValue() == -1 || input.getAuxValue() == m_ingredients[0].getAuxValue());
}

const std::vector<ItemStack>& SingleInputRecipe::getIngredients()
{
    return m_ingredients;
}

const ItemStack& SingleInputRecipe::getResultItem()
{
    return m_result;
}

int SingleInputRecipe::size() const
{
	return 1;
}
