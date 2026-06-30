#pragma once

#include "world/inventory/Container.hpp"

class Recipe
{
public:
    virtual ~Recipe() {}

    virtual bool matches(Container* container) = 0;
    virtual const std::vector<ItemStack>& getIngredients() = 0;
    virtual const ItemStack& getResultItem() = 0;
    //@NOTE: This should be use for Custom Recipes, these have their result based on the items in the container
    virtual const ItemStack& assemble(Container* container) { return getResultItem(); }
    virtual int size() const = 0;
    virtual bool isShaped() const { return false; }
    //@NOTE: Special Recipes can't be displayed correctly, as their ingredients and result depends completely on the item in the container
    virtual bool isSpecial() const { return false; }
};
