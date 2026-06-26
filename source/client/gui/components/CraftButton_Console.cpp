#include "CraftButton_Console.hpp"
#include "client/renderer/entity/ItemRenderer.hpp"
#include "renderer/ShaderConstants.hpp"

CraftButton_Console::CraftButton_Console(int groupIndex, ClientCraftContext* craftContext, int size)
	: Button(0, 0, size, size, "")
	, m_slotIndex(groupIndex)
	, m_pCraftContext(craftContext)
{
}

void CraftButton_Console::_renderResultItem(Minecraft* mc, Recipe* recipe, int x, int y)
{
	MatrixStack::Ref matrix = MatrixStack::World.push();
	float off = 3 * m_width / 50.0f;
	matrix->translate(Vec3(x + off, y + off, 0.0f));
	matrix->scale(m_width / 18);
	ItemRenderer::singleton().renderGuiItem(*mc, recipe->getResultItem(), 0, 0, Color(1.0f, 1.0f, 1.0f, m_pCraftContext->canCraft(recipe) ? 1.0f : 0.5f));
}

bool CraftButton_Console::_canScroll()
{
	return isSelected() && m_pCraftContext->selectedRecipes.size() >= 3;
}

void CraftButton_Console::renderBg(Minecraft* pMinecraft, const MenuPointer& pointer)
{
	if (!isSelected()) return;

	Recipes::RecipeList& recipeList = m_pCraftContext->selectedRecipes;

	if (recipeList.size() > 2)
	{
		blitSprite(*pMinecraft->m_pTextures, "gui/console/CraftingPanels/Crafting_3SlotLargeV.png", m_xPos - 13, m_yPos - 102, 80, 258);
		_renderResultItem(pMinecraft, recipeList[recipeList.size() - 1], m_xPos, m_yPos - m_height);
		_renderResultItem(pMinecraft, recipeList[1], m_xPos, m_yPos + m_height);
	}
	else if (recipeList.size() > 1)
	{
		blitSprite(*pMinecraft->m_pTextures, "gui/console/CraftingPanels/Crafting_2SlotLargeV.png", m_xPos - 13, m_yPos - 48, 80, 204);
		_renderResultItem(pMinecraft, recipeList[1], m_xPos, m_yPos + m_height);
	}

	blitSprite(*pMinecraft->m_pTextures, "gui/console/CraftingPanels/Craft_Highlight_L_Small.png", m_xPos - 9, m_yPos - 9 + (m_pCraftContext->selectionOffset * m_height), 72, 72);
}

void CraftButton_Console::render(Minecraft* pMinecraft, const MenuPointer& pointer)
{
	if (!isVisible()) return;

	if (m_color.a == 0.0f)
		return;

	currentShaderColor = m_color;

	renderBg(pMinecraft, pointer);

	Recipe* recipe = getRecipe();

	if (recipe)
		_renderResultItem(pMinecraft, recipe, m_xPos, m_yPos);
}

bool CraftButton_Console::isHovered(Minecraft* mc, const MenuPointer& pointer)
{
	if (Button::isHovered(mc, pointer)) return true;

	if (!isSelected()) return false;

	if (pointer.x < m_xPos) return false;
	if (pointer.x >= m_xPos + m_width) return false;

	Recipes::RecipeList& recipeList = m_pCraftContext->selectedRecipes;

	if (recipeList.size() >= 2 && pointer.y >= m_yPos + m_height && pointer.y < m_yPos + m_height * 2)
		return true;
	else if (recipeList.size() >= 3 && pointer.y >= m_yPos - m_height && pointer.y < m_yPos)
		return true;

	return false;
}

void CraftButton_Console::pressed(Minecraft*, const MenuPointer& pointer)
{
	if (pointer.x < m_xPos) return;
	if (pointer.x >= m_xPos + m_width) return;

	Recipes::RecipeList& recipeList = m_pCraftContext->selectedRecipes;

	if (recipeList.size() >= 2 && pointer.y >= m_yPos + m_height && pointer.y < m_yPos + m_height * 2)
		m_pCraftContext->selectionOffset = 1;
	else if (recipeList.size() >= 3 && pointer.y >= m_yPos - m_height && pointer.y < m_yPos)
		m_pCraftContext->selectionOffset = -1;
	else
		m_pCraftContext->selectionOffset = 0;

	if (isSelected())
		m_pCraftContext->craft(m_pCraftContext->getSelectedRecipe());
}

void CraftButton_Console::handleScroll(float force)
{
	if (!isSelected()) return;

	if (_canScroll())
		m_pCraftContext->scrollSelectedRecipes(Mth::signum(force));
}

bool CraftButton_Console::areaNavigation(Minecraft* mc, AreaNavigation::Direction dir)
{
	if (!isSelected() || (dir != AreaNavigation::DOWN && dir != AreaNavigation::UP)) return false;

	Recipes::RecipeList& recipeList = m_pCraftContext->selectedRecipes;

	int oldSelection = m_pCraftContext->selectionOffset;

	if ((dir == AreaNavigation::DOWN && recipeList.size() >= 2) ||
		(dir == AreaNavigation::UP && (recipeList.size() >= 3 || m_pCraftContext->selectionOffset == 1)))
		m_pCraftContext->selectionOffset = Mth::clamp(m_pCraftContext->selectionOffset + (dir == AreaNavigation::DOWN ? 1 : -1), -1, 1);

	if (oldSelection != m_pCraftContext->selectionOffset || _canScroll())
	{
		if (oldSelection == m_pCraftContext->selectionOffset && m_pCraftContext->selectionOffset != 0)
			m_pCraftContext->scrollSelectedRecipes(dir == AreaNavigation::UP ? 1 : -1);
		return true;
	}

	return false;
}
