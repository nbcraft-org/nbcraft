#pragma once

#include "client/renderer/Textures.hpp"
#include "world/inventory/Container.hpp"
#include "world/inventory/ContainerMenu.hpp"
#include "ScrollingPane.hpp"

class InventoryPane_Pocket : public ScrollingPane
{
public:
	using GuiElement::tick;

public:
	static const Color BACKGROUND_COLOR;

protected:
	class Materials
	{
	public:
		mce::MaterialPtr ui_item; // only supposed to be in TileRenderer
		mce::MaterialPtr ui_item_glint;

		Materials();
	};

public:
	class ICallback
	{
	public:
		virtual bool addItem(const InventoryPane_Pocket& pane, Container::SlotID slotId) = 0;
		virtual bool isAllowed(Container::SlotID slotId) = 0;
		virtual ContainerMenu* getContainerMenu(const InventoryPane_Pocket& pane) = 0;
	};

public:
	InventoryPane_Pocket(ICallback& callback, Minecraft& mc, const IntRectangle& areaRect, int width, float paddingH, int itemCount);
	~InventoryPane_Pocket();

protected:
	void _drawScrollBar(ScrollBar& scrollBar);

public:
	void tick();

public:
	void renderBatch(std::vector<GridItem>& items, float a) override;
	void onSelect(int id, bool isSelected) override;

protected:
	Materials m_itemMaterials;
	int m_widthOrSomething;
	RectangleArea* m_pRectArea;
	ICallback& m_callback;
	Minecraft& m_minecraft;
	Container::SlotID m_startingSlotId;
public:
	int m_paddingX;
	int m_paddingY;
	int m_selectedId;
	int m_clickAnimTick;
};
