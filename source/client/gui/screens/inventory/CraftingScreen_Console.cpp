#include "CraftingScreen_Console.hpp"
#include "world/item/Inventory.hpp"
#include "world/entity/Player.hpp"
#include "world/inventory/CraftingMenu.hpp"
#include "client/gui/components/TabButton.hpp"
#include "client/locale/Language.hpp"
#include "renderer/ShaderConstants.hpp"

CraftingScreen_Console::CraftingScreen_Console(Inventory* inventory, const TilePos& tilePos, Level* level, bool is2x2) : ContainerScreen(new CraftingMenu(inventory, tilePos, level, is2x2))
    , m_tabLayout(this)
    , m_craftLayout(this)
    , m_craftContext(inventory, &m_slotDisplays, is2x2)
{
    m_uiTheme = UI_CONSOLE;
    m_imageWidth = is2x2 ? 591 : 689;
    m_imageHeight = 490;
    m_bRenderPointer = false;

    _addTab("gui/console/icon_structures.png", "container.tab.structures");
    _addTab("gui/console/icon_tools.png", "container.tab.tools");
    _addTab("gui/console/icon_food.png", "container.tab.food");
    if (!is2x2)
        _addTab("gui/console/icon_armour.png", "container.tab.armour");
    _addTab("gui/console/icon_mechanisms.png", "container.tab.mechanisms");
    _addTab("gui/console/icon_transport.png", "container.tab.transport");
    _addTab("gui/console/icon_decoration.png", "container.tab.decoration");

    for (int i = 0; i < (is2x2 ? 10 : 12); ++i)
    {
        m_craftLayout.m_elements.push_back(new CraftButton_Console(i, &m_craftContext));
    }
}

void CraftingScreen_Console::_renderFg(float partialTicks)
{
    ContainerScreen::_renderFg(partialTicks);
    m_tabLayout.renderSelected(m_pMinecraft, m_menuPointer);
    m_craftLayout.renderSelected(m_pMinecraft, m_menuPointer);
}

void CraftingScreen_Console::renderBackground()
{
}

void CraftingScreen_Console::onClose()
{
    const int gridSize = ((CraftingMenu*)m_pMenu)->m_pCraftSlots->getContainerSize();

    for (int i = 1; i <= gridSize; ++i)
    {
        Slot* slot = m_pMenu->getSlot(i);

        // just to be sure
        if (!getSlotDisplay(i).isInteractable)
            slot->set(ItemStack::EMPTY);
    }

    ContainerScreen::onClose();
}

void CraftingScreen_Console::handleUserAction(const ActionInfo& action)
{
    if (m_pMinecraft->getOptions()->isAction(AID_MENU_OK, action))
    {
        m_craftContext.craft(m_craftContext.getSelectedRecipe());
    }

    ContainerScreen::handleUserAction(action);
}

void CraftingScreen_Console::init()
{
    ContainerScreen::init();
    m_tabLayout.init(m_leftPos - 3, m_topPos - 5, -9);
    m_craftLayout.init(m_leftPos + (m_craftContext.bIs2x2 ? 26 : 21), m_topPos + 140);
}

void CraftingScreen_Console::_addTab(const std::string& sprite, const std::string& name)
{
    m_tabLayout.m_elements.push_back(new TabButton(107, 85, name, SpriteDef(m_tabLayout.m_elements.empty() ? "gui/console/Graphics/Tab_Left.png" : m_tabLayout.m_elements.size() == (m_craftContext.bIs2x2 ? 5 : 6) ? "gui/console/Graphics/Tab_Right.png" : "gui/console/Graphics/Tab_Middle.png", IntRectangle()), SpriteDef(), SpriteDef(sprite, IntRectangle(0, 0, 48, 48))));
}

bool CraftingScreen_Console::_areaNavigation(AreaNavigation::Direction dir)
{
    if (m_craftLayout.areaNavigation(m_pMinecraft, dir))
    {
        _playSelectSound();
        return true;
    }

    return ContainerScreen::_areaNavigation(dir);
}

bool CraftingScreen_Console::_nextTab()
{
    m_tabLayout.areaNavigation(AreaNavigation::RIGHT);
    return true;
}

bool CraftingScreen_Console::_prevTab()
{
    m_tabLayout.areaNavigation(AreaNavigation::LEFT);
    return true;
}

void CraftingScreen_Console::_renderLabels()
{
    if (m_tabLayout.m_pSelectedElement)
    {
        const std::string& message = Language::get(m_tabLayout.m_pSelectedElement->getMessage());
        m_pFont->drawScalable(message, m_imageWidth / 2 - m_pFont->width(message), 100, Color::TEXT_GREY);
    }

    const std::string& invName = m_pMinecraft->m_pLocalPlayer->m_pInventory->getName();

    m_pFont->drawScalable(invName, (m_craftContext.bIs2x2 ? 270 + 306 / 2 : 313 + 361 / 2) - m_pFont->width(invName), 294, Color::TEXT_GREY);

    const int gridSize = ((CraftingMenu*)m_pMenu)->m_pCraftSlots->getContainerSize();

    std::vector<ItemStack> ings = m_craftContext.getShapedIngredients(m_craftContext.getSelectedRecipe());

    for (int i = 0; i <= gridSize; ++i)
    {
        Slot* slot = m_pMenu->getSlot(i);
        getSlotDisplay(i).isInteractable = false;

        if (i == 0) continue;

        if (size_t(i - 1) >= ings.size())
        {
            slot->set(ItemStack::EMPTY);
            continue;
        }

        ItemStack& ing = ings[i - 1];

        if (ing.getAuxValue() == -1)
            ing.setAuxValue(0);

        slot->set(ing);
    }

    if (!m_pMenu->getSlot(0)->hasItem()) return;

    std::string resultName = Language::get(m_pMenu->getSlot(0)->getItem().getHovertextName());

    m_pFont->drawScalable(resultName, 15 + (m_craftContext.bIs2x2 ? 250 : 293) / 2 - m_pFont->width(resultName), 294, Color::TEXT_GREY);
}

void CraftingScreen_Console::_renderBg(float partialTicks)
{
    currentShaderColor = Color::WHITE;
    blitTexture(*m_pMinecraft->m_pTextures, m_craftContext.bIs2x2 ? "gui/console/Graphics/Crafting_Panel2x2.png" : "gui/console/Graphics/Crafting_Panel.png", m_leftPos, m_topPos, 0, 0, m_imageWidth, m_imageHeight, m_imageWidth, m_imageHeight);
    blitSprite(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Arrow_Small_Off.png", m_leftPos + (m_craftContext.bIs2x2 ? 137 : 183), m_topPos + (m_craftContext.bIs2x2 ? 373 : 380), 32, 32);
}

SlotDisplay CraftingScreen_Console::_createSlotDisplay(const Slot& slot)
{
    constexpr int invSlotSize = 32;
    constexpr int craftingSlotSize = 48;
    int dim = m_craftContext.bIs2x2 ? 2 : 3;

    switch (slot.m_group)
    {
    case Slot::OUTPUT:
        return SlotDisplay((m_craftContext.bIs2x2 ? 183 : 233), (m_craftContext.bIs2x2 ? 357 : 366), 72, true);
    case Slot::INPUT:
        return SlotDisplay((m_craftContext.bIs2x2 ? 32 : 24) + (slot.m_stackId % dim) * craftingSlotSize, (m_craftContext.bIs2x2 ? 346 : 327) + (slot.m_stackId / dim) * craftingSlotSize, craftingSlotSize, true);
    case Slot::INVENTORY:
        return SlotDisplay((m_craftContext.bIs2x2 ? 280 : 353) + (slot.m_stackId % 9) * invSlotSize, 326 + ((slot.m_stackId / 9) - 1) * invSlotSize, invSlotSize, true);
    case Slot::HOTBAR:
        return SlotDisplay((m_craftContext.bIs2x2 ? 280 : 353) + (slot.m_stackId % 9) * invSlotSize, 438, invSlotSize, true);
    default:
        return SlotDisplay();
    }
}

CraftingScreen_Console::CraftingTabLayout::CraftingTabLayout(CraftingScreen_Console* screen) : TabLayout(screen)
{
}

void CraftingScreen_Console::CraftingTabLayout::onSelectElement(GuiElement* element)
{
    CraftingScreen_Console* screen = getScreen();

    if (&screen->m_tabLayout == this)
    {
        int index = screen->m_tabLayout.getIndex();

        if (screen->m_craftContext.bIs2x2 && index >= Recipes::GC_ARMOUR) index++;

        screen->m_craftContext.groupRecipes = Recipes::singleton().m_recipesByConsoleGroup[index];


        if (screen->m_craftContext.bIs2x2)
        {
            for (std::vector<Recipes::RecipeList>::iterator it = screen->m_craftContext.groupRecipes.begin(); it != screen->m_craftContext.groupRecipes.end(); )
            {
                Recipes::RecipeList& recipeList = *it;

                for (Recipes::RecipeList::iterator it2 = recipeList.begin(); it2 != recipeList.end(); )
                {
                    Recipe* recipe = *it2;

                    if (recipe->size() > 4 || (recipe->isShaped() && (((ShapedRecipe*)recipe)->getWidth() > 2 || ((ShapedRecipe*)recipe)->getHeight() > 2)))
                        it2 = recipeList.erase(it2);
                    else
                        it2++;
                }

                if (recipeList.empty())
                    it = screen->m_craftContext.groupRecipes.erase(it);
                else
                    it++;
            }
        }

        if (screen->m_craftLayout.m_pSelectedElement)
        {
            screen->m_craftLayout.selectElement(nullptr);
            screen->m_craftLayout.startNavigation();
        }
    }

    screen->m_craftContext.selectionOffset = 0;

    int selectedIndex = screen->m_craftLayout.getIndex();
    std::vector<Recipes::RecipeList>& recipeLists = screen->m_craftContext.groupRecipes;

    screen->m_craftContext.selectedRecipes = size_t(selectedIndex) < recipeLists.size() ? recipeLists[selectedIndex] : Recipes::RecipeList();
}
