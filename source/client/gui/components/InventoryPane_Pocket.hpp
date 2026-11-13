#pragma once

#include "client/renderer/Textures.hpp"
#include "world/inventory/Container.hpp"
#include "ScrollingPane.hpp"

class InventoryPane_Pocket : public ScrollingPane
{
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
		virtual bool addItem(const InventoryPane_Pocket& pane, int id) = 0;
		virtual bool isAllowed(int id) = 0;
		virtual std::vector<const ItemStack*> getItems(const InventoryPane_Pocket& pane) = 0;
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
	//IntRectangle m_rect;
	int m_widthOrSomething;
	RectangleArea* m_pRectArea;
	ICallback& m_callback;
	Minecraft& m_minecraft;
public:
	int m_paddingX;
	int m_paddingY;
	int m_selectedId;
	int m_clickAnimTick;
	//int m_paneLeftX;
	//int m_paneLeftY;
	//int m_paneRightX;
	//int m_paneRightY;
};
