#include "TabLayout.hpp"

TabLayout::TabLayout(Screen* screen)
	: m_pScreen(screen)
	, m_pSelectedElement(nullptr)
	, m_pClickedElement(nullptr)
	, m_spacing(0)
	, m_organizeDir(AreaNavigation::RIGHT)
	, m_bCyclic(false)
{
}

TabLayout::~TabLayout()
{
	clear();
}

GuiElement* TabLayout::getElement(ID index) const
{
	if (index >= 0 && index < ID(m_elements.size()))
		return m_elements[index];

	return nullptr;
}

bool TabLayout::selectElementById(ID id, bool sound)
{
	GuiElement* element = getElement(id);
	if (!element) return false;

	selectElement(element);
	if (sound)
		m_pScreen->_playSelectSound();

	return true;
}

bool TabLayout::selectElement(GuiElement* element)
{
	if (element == m_pSelectedElement) return false;

	if (m_pSelectedElement)
		m_pSelectedElement->setSelected(false);
	m_pSelectedElement = element;
	if (m_pSelectedElement)
	{
		m_pSelectedElement->setSelected(true);
		onSelectElement(element);
	}

	return true;
}

GuiElement::ID TabLayout::getIndex() const
{
	return m_pSelectedElement ? m_pSelectedElement->getId() : 0;
}

void TabLayout::onSelectElement(GuiElement*)
{
}

void TabLayout::init(int x, int y, int spacing, AreaNavigation::Direction organizeDir, bool cyclic)
{
	m_xPos = x;
	m_yPos = y;
	m_spacing = spacing;
	m_organizeDir = organizeDir;
	m_bCyclic = cyclic;

	m_pScreen->_addElement(*this, false);

	organize();
}

void TabLayout::organize()
{
	for (size_t i = 0; i < m_elements.size(); ++i)
	{
		GuiElement* element = m_elements[i];
		element->setId(i);
		if (i)
		{
			GuiElement* last = m_elements[i - 1];
			element->m_xPos = last->m_xPos;
			element->m_yPos = last->m_yPos;
			switch (m_organizeDir)
			{
			case AreaNavigation::UP:
				element->m_yPos -= element->m_height + m_spacing;
			case AreaNavigation::DOWN:
				element->m_yPos += last->m_height + m_spacing;
			case AreaNavigation::LEFT:
				element->m_xPos -= element->m_width + m_spacing;
			case AreaNavigation::RIGHT:
				element->m_xPos += last->m_width + m_spacing;
			}
		}
		else
		{
			element->m_xPos = m_xPos;
			element->m_yPos = m_yPos;
		}
	}

	if (!m_pSelectedElement)
		startNavigation();
}

void TabLayout::clear()
{
	if (m_elements.empty()) return;

	m_pSelectedElement = nullptr;
	m_pClickedElement = nullptr;

	for (GuiElementList::iterator it = m_elements.begin(); it != m_elements.end(); it = m_elements.erase(it))
	{
		delete (*it);
	}
}

void TabLayout::startNavigation()
{
	AreaNavigation::ID id = Navigation(this).navigate(AreaNavigation::RIGHT, 0, 0);
	if (id >= 0)
		selectElementById(id, false);
}

bool TabLayout::areaNavigation(Minecraft* mc, AreaNavigation::Direction dir)
{
	if (m_pSelectedElement && m_pSelectedElement->areaNavigation(mc, dir)) return true;

	return areaNavigation(dir);
}

bool TabLayout::areaNavigation(AreaNavigation::Direction dir, bool cyclic)
{
	GuiElement* element = m_pSelectedElement;

	if (!element) return false;

	AreaNavigation::ID id;
	if (m_bCyclic && cyclic)
		id = Navigation(this).navigateCyclic(dir, element->m_xPos + element->m_width / 2, element->m_yPos + element->m_height / 2);
	else
		id = Navigation(this).navigate(dir, element->m_xPos + element->m_width / 2, element->m_yPos + element->m_height / 2);

	if (id >= 0)
		return selectElementById(id);

	return false;
}

void TabLayout::setSelected(bool b)
{
	GuiElement::setSelected(b);

	if (b && !m_pSelectedElement && m_pScreen->_useController())
		startNavigation();
}

bool TabLayout::isHovered(Minecraft* mc, const MenuPointer& pointer)
{
	for (size_t i = 0; i < m_elements.size(); ++i)
	{
		if (m_elements[i]->isHovered(mc, pointer))
			return true;
	}

	return false;
}

void TabLayout::pressed(Minecraft* mc, const MenuPointer& pointer)
{
	for (size_t i = 0; i < m_elements.size(); ++i)
	{
		GuiElement* element = m_elements[i];
		if (element->isHovered(mc, pointer)) 
		{
			m_pClickedElement = element;
			element->pressed(mc, pointer);
			selectElement(element);
			return;
		}
	}
}

void TabLayout::released(const MenuPointer& pointer)
{
	if (m_pClickedElement)
	{
		m_pClickedElement->released(pointer);
		m_pClickedElement = nullptr;
	}
}

void TabLayout::pressed(Minecraft* mc)
{
	if (m_pSelectedElement)
		m_pSelectedElement->pressed(mc);
}

void TabLayout::handleScroll(float force)
{
	if (m_pSelectedElement)
		m_pSelectedElement->handleScroll(force);
}

void TabLayout::render(Minecraft* mc, const MenuPointer& pointer)
{
	for (size_t i = 0; i < m_elements.size(); ++i)
	{
		if (!m_pSelectedElement || i != size_t(m_pSelectedElement->getId()))
			m_elements[i]->render(mc, pointer);
	}
}

void TabLayout::renderSelected(Minecraft* mc, const MenuPointer& pointer)
{
	if (m_pSelectedElement)
		m_pSelectedElement->render(mc, pointer);
}

TabLayout::Navigation::Navigation(TabLayout* layout) : m_pLayout(layout)
{
	m_area.w = layout->m_pScreen->m_width;
	m_area.h = layout->m_pScreen->m_height;
}

bool TabLayout::Navigation::next(int& x, int& y, bool cycle)
{
	while (++m_index < ID(m_pLayout->m_elements.size()))
	{
		GuiElement* element = m_pLayout->m_elements[m_index];

		if (!element->isVisible() || !element->isNavigable() || !(cycle || isValid(m_index))) continue;

		x = element->m_xPos + element->m_width / 2;
		y = element->m_yPos + element->m_height / 2;

		return true;
	}

	return false;
}

bool TabLayout::Navigation::isValid(ID id)
{
	return !m_pLayout->m_pSelectedElement || m_pLayout->m_pSelectedElement->getId() != id;
}
