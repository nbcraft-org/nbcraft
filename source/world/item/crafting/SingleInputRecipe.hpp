#pragma once

#include "Recipe.hpp"

class SingleInputRecipe : public Recipe
{
public:
    SingleInputRecipe(const ItemStack& ingredient, const ItemStack& result);
    virtual ~SingleInputRecipe();

    bool matches(Container* container) override;
    bool matches(const ItemStack& input);

    const std::vector<ItemStack>& getIngredients() override;
    const ItemStack& getResultItem() override;
    int size() const override;

private:
    ItemStack m_result;
    std::vector<ItemStack> m_ingredients;
};
