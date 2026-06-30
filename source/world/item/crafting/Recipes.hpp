#include <vector>
#include <map>
#include <string>
#include "Recipe.hpp"
#include "world/inventory/CraftingContainer.hpp"
#include "ShapedRecipe.hpp"
#include "ShapelessRecipe.hpp"
#include "world/tile/Tile.hpp"

class ItemStack;

class RecipeBuilder
{
public:
    virtual Recipe* build() = 0;
    virtual ~RecipeBuilder() {}
};

class ShapelessRecipeBuilder : public RecipeBuilder
{
public:
    ShapelessRecipeBuilder(const ItemStack& result) : m_result(result)
    {
    }

    ShapelessRecipeBuilder& add(Item* item)
    {
        return add(ItemStack(item));
    }

    ShapelessRecipeBuilder& add(Tile* tile)
    {
        return add(ItemStack(tile));
    }

    ShapelessRecipeBuilder& add(const ItemStack& ingredient)
    {
        m_ingredients.push_back(ingredient);
        return *this;
    }

    ShapelessRecipe* build()
    {
        return new ShapelessRecipe(m_result, m_ingredients);
    }

private:
    ItemStack m_result;
    std::vector<ItemStack> m_ingredients;
};

class ShapedRecipeBuilder : public RecipeBuilder
{
public:
    ShapedRecipeBuilder(const std::string& line1, const ItemStack& result) :
        m_result(result)
    {
        m_pattern.push_back(line1);
    }

    ShapedRecipeBuilder(const std::string& line1, const std::string& line2, const ItemStack& result) :
        m_result(result)
    {
        m_pattern.push_back(line1);
        m_pattern.push_back(line2);
    }

    ShapedRecipeBuilder(const std::string& line1, const std::string& line2, const std::string& line3, const ItemStack& result) :
        m_result(result)
    {
        m_pattern.push_back(line1);
        m_pattern.push_back(line2);
        m_pattern.push_back(line3);
    }

    ShapedRecipeBuilder& add(char character, Item* item)
    {
        return add(character, ItemStack(item));
    }

    ShapedRecipeBuilder& add(char character, Tile* tile)
    {
        return add(character, ItemStack(tile, 1, -1));
    }

    ShapedRecipeBuilder& add(char character, const ItemStack& ingredient)
    {
        m_ingredients[character] = ingredient;
        return *this;
    }

    ShapedRecipe* build()
    {
        std::vector<ItemStack> ingredients;
        size_t width = 0;
        int height = int(m_pattern.size());

        for (size_t i = 0; i < m_pattern.size(); ++i)
        {
            if (m_pattern[i].size() > width)
                width = m_pattern[i].size();
        }

        for (size_t row = 0; row < m_pattern.size(); ++row)
        {
            const std::string& rowStr = m_pattern[row];

            for (size_t col = 0; col < rowStr.size(); ++col)
            {
                char character = rowStr[col];

                if (character == ' ')
                    ingredients.push_back(ItemStack::EMPTY);
                else if (m_ingredients.find(character) != m_ingredients.end())
                    ingredients.push_back(m_ingredients[character]);
                else
                    ingredients.push_back(ItemStack::EMPTY);
            }

            for (size_t i = rowStr.size(); i < width; ++i)
                ingredients.push_back(ItemStack::EMPTY);
        }

        return new ShapedRecipe(width, height, m_result, ingredients);
    }

private:
    std::vector<std::string> m_pattern;
    ItemStack m_result;
    std::map<char, ItemStack> m_ingredients;
};

class Recipes
{
public:
    typedef std::vector<Recipe*> RecipeList;

    enum Group_Console
    {
        GC_STRUCTURES,
        GC_TOOLS,
        GC_FOOD,
        GC_ARMOUR,
        GC_MECHANISM,
        GC_TRANSPORT,
        GC_DECORATION,
        GC_COUNT
    };

public:
    Recipes();
    ~Recipes();

    static Recipes& singleton()
    {
        if (!instance)
        {
            instance = new Recipes;
        }
        return *instance;
    }

    //@NOTE: -1 as the group index just adds to the end of the recipe list
    void add(Recipe* recipe, Group_Console group = GC_DECORATION, int slotIndex = -1)
    {
        m_recipes.push_back(recipe);

        if (group >= GC_COUNT) return;

        std::vector<RecipeList>& recipeLists = m_recipesByConsoleGroup[group];

        if (slotIndex >= 0)
        {
            while (recipeLists.size() <= size_t(slotIndex))
            {
                recipeLists.push_back(RecipeList());
            }
            RecipeList& recipeList = recipeLists[slotIndex];
            recipeList.push_back(recipe);
        }
        else
        {
            RecipeList recipeList;
            recipeList.push_back(recipe);
            recipeLists.push_back(recipeList);
        }
    }

    void add(RecipeBuilder& recipe, Group_Console group = GC_DECORATION, int slotIndex = -1)
    {
        add(recipe.build(), group, slotIndex);
    }

    void addTools(const ItemStack& material, Item* sword, Item* pickaxe, Item* axe, Item* shovel, Item* hoe);
    void addArmor(const ItemStack& material, Item* helmet, Item* chestplate, Item* leggings, Item* boots, Group_Console = GC_ARMOUR);
    void addOre(const ItemStack& material, Tile* block, int unprocessGroup = 3);

    const ItemStack& getItemFor(CraftingContainer* container)
    {
        for (size_t i = 0; i < m_recipes.size(); ++i)
        {
            Recipe* recipe = m_recipes[i];
            if (recipe->matches(container))
            {
                return recipe->assemble(container);
            }
        }
        return ItemStack::EMPTY;
    }

private:
    static Recipes* instance;

public:
    RecipeList m_recipes;
    std::vector<RecipeList> m_recipesByConsoleGroup[GC_COUNT];
};
