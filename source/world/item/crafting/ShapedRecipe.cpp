#include "ShapedRecipe.hpp"

ShapedRecipe::ShapedRecipe(int width, int height, const ItemStack& result, const std::vector<ItemStack>& ingredients) :
    m_width(width),
    m_height(height),
    m_result(result),
    m_ingredients(ingredients)
{
}

ShapedRecipe::~ShapedRecipe()
{
}

bool ShapedRecipe::matches(Container* container)
{
    CraftingContainer* craftingContainer = (CraftingContainer*)container;
    if (!craftingContainer) return false;

    for (int x = 0; x <= 3 - m_width; ++x)
    {
        for (int y = 0; y <= 3 - m_height; ++y)
        {
            if (matches(craftingContainer, x, y, true))
                return true;

            if (matches(craftingContainer, x, y, false))
                return true;
        }
    }

    return false;
}

const std::vector<ItemStack>& ShapedRecipe::getIngredients()
{
    return m_ingredients;
}

const ItemStack& ShapedRecipe::getResultItem()
{
	return m_result;
}

int ShapedRecipe::size() const
{
	return m_width * m_height;
}

bool ShapedRecipe::matches(CraftingContainer* container, int i, int j, bool lenient)
{
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            int xd = x - i;
            int yd = y - j;
            const ItemStack* ing = &ItemStack::EMPTY;
            if (xd >= 0 && yd >= 0 && xd < m_width && yd < m_height)
            {
                if (lenient)
                    ing = &m_ingredients[m_width - xd - 1 + yd * m_width];
                else
                    ing = &m_ingredients[xd + yd * m_width];
            }

            if (!ing->sameIngredient(container->getItem(x, y))) return false;
        }
    }

    return true;
}

bool ShapedRecipe::isShaped() const
{
    return true;
}
