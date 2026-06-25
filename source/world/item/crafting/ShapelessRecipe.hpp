#pragma once

#include <vector>

#include "Recipe.hpp"

class ShapelessRecipe : public Recipe
{
public:
    ShapelessRecipe(const ItemStack& result, const std::vector<ItemStack>& ingredients);
    virtual ~ShapelessRecipe();

    bool matches(Container* container) override;
    const std::vector<ItemStack>& getIngredients() override;
    const ItemStack& getResultItem() override;
    int size() const override;

private:
    ItemStack m_result;
    std::vector<ItemStack> m_ingredients;
};
