#pragma once

#include "client/renderer/Textures.hpp"
#include "world/inventory/Container.hpp"
#include "ScrollingPane.hpp"

class CItem
{
public:
	ItemStack* m_pItemStack;
	std::string m_subtext;
	int m_unknown;
	int m_count;
};

class ItemPane : public ScrollingPane
{
public:
	class ICallback
	{
	public:
		virtual void onItemSelected(const ItemPane& itemPane, Container::StackID stackId) = 0;
		virtual std::vector<CItem*>& getItems(const ItemPane& itemPane) = 0;
	};

public:
	ItemPane(ICallback& callback, Minecraft& mc, const IntRectangle& areaRect, int itemCount, int bottom, int screenHeight, bool isVertical);

protected:
	void _drawScrollBar(ScrollBar& scrollBar);

public:
	void renderBatch(std::vector<GridItem>& items, float a) override;
	void onSelect(int id, bool isSelected) override;

protected:
	Minecraft& m_minecraft;
	ICallback& m_callback;
	int m_screenHeight;
	bool m_bVertical;
	TextureAtlasSprite m_sprite;
	TextureAtlasSprite m_spriteSelected;
};
