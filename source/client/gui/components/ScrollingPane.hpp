#pragma once

#include <stdint.h>
#include <vector>

#include "common/Timer.hpp"
#include "client/gui/GuiElement.hpp"
#include "client/player/input/RectangleArea.hpp"

struct ScrollBar
{
	enum FadeState
	{
		FADE_STOPPED,
		FADE_DISAPPEARING,
		FADE_APPEARING,
	};

	ScrollBar()
		: alpha(0.0f)
		, fadeState(FADE_STOPPED)
	{
	}

	Vec2 pos;
	Vec2 size;
	float alpha;
	FadeState fadeState;
};

class ScrollingPane : public GuiElement
{
protected:
	typedef uint8_t Flags;
	enum Flag
	{
		FLAG_NOSCROLLH   = 1<<0,
		FLAG_NOSCROLLV   = 1<<1,
		FLAG_WRAPH       = 1<<2,
		FLAG_WRAPV       = 1<<3,
		FLAG_NOBOUNCE    = 1<<4,
		FLAG_MULTISELECT = 1<<5,
		FLAG_SCROLLFADE  = 1<<6
	};

public:
	struct GridItem
	{
		GridItem()
			: id(0)
			, x(0)
			, y(0)
			, isSelected(false)
		{
		}

		int id;
		int x;
		int y;
		Vec2 screenPos;
		bool isSelected;
	};

public:
	ScrollingPane(Flags flags, const IntRectangle& areaRect, const IntRectangle& a4, int columns, int itemCount, float scale, const IntRectangle& itemRect);
	~ScrollingPane();

protected:
	void _addFlags(Flags flags) { m_flags |= flags; }
	bool _hasFlag(Flag flag) const { return (m_flags & flag) != 0; }
	void _hideScrollIndicators();
	void _onSelect(int id);
	void _onHoldItem();
	void _updateScrollFade(ScrollBar& scrollBar);
	void _updateVerticalScrollIndicator();
	void _updateHorizontalScrollIndicator();
	void _setContentOffsetWithAnimation(const Vec2& offset, bool ignoreScrollbars);
	void _setContentOffset(const Vec2& offset);
	void _adjustContentSize();
	void _snapContentOffsetToBounds(bool ignoreScrollbars);
	void _updateTrackingStartPos();
	void _beginTracking(int time);
	void _startDecelerationAnimation(bool isSubstep);
	void _stopDecelerationAnimation();
	void _stepThroughDecelerationAnimation(bool isSubstep);
	void _touchesBegan(int time);
	void _touchesEnded(int time);
	void _touchesMoved(int time);
	void _touchesCancelled(int time);
	GridItem _getItemForPos(const Vec2& pos, bool b);
	void _updateHighlightItem(const Vec2& pos);
	void _handleUserInput(const MenuPointer& pointer);

public:
	void tick();
	void setSelected(int id, bool isSelected);
	void translate(const Vec2& t);
	bool getGridItemFor_slow(int id, GridItem& result);
	void render(Minecraft* pMinecraft, const MenuPointer& pointer) override;

	bool hasHorizontalScrolling() const { return !_hasFlag(FLAG_NOSCROLLH); }
	bool hasVerticalScrolling() const { return !_hasFlag(FLAG_NOSCROLLV); }
	bool hasHorizontalWrapping() const { return _hasFlag(FLAG_WRAPH); }
	bool hasVerticalWrapping() const { return _hasFlag(FLAG_WRAPV); }
	bool hasBounce() const { return !_hasFlag(FLAG_NOBOUNCE); }
	bool isMultiselect() const { return _hasFlag(FLAG_MULTISELECT); }
	bool hasFade() const { return _hasFlag(FLAG_SCROLLFADE); }
	bool isSelected(int id) const { return isMultiselect() ? m_selectedById[id] : m_selectedItemId == id; }

public:
	virtual void renderBatch(std::vector<GridItem>& items, float a);
	virtual void renderItem(GridItem& item, float a);
	virtual void didEndDragging() {}
	virtual void didEndDecelerating() {}
	virtual void willBeginDecelerating() {}
	virtual void willBeginDragging() {}
	virtual void onSelect(int id, bool isSelected) {}

protected:
	Flags m_flags;
	int m_columns;
	int m_rows;
	int m_itemCount;
	Vec2 m_inverseContentOffset;
	float m_scale;
	//float m_guiRatio;
	IntRectangle m_itemRect;
	RectangleArea m_area;
	RectangleArea m_interactArea;
	bool m_bDragging;
	bool m_bDecelerating;
	bool m_bTracking;
	bool m_bPagingEnabled;
	Vec2 m_contentOffset;
	Vec2 m_touchesEndedContentOffset;
	int m_touchesMovedTime;
	Vec2 m_scrollVelocity;
	Vec2 m_decelLimitMin;
	Vec2 m_decelLimitMax;
	float m_bounceTension;
	float m_bounceDamping;
	Vec2 m_minContentOffset;
	Vec2 m_trackingStartContentOffset;
	Vec2 m_trackingStartPos;
	Vec2 m_trackingStartContentOffset2;
	bool m_bTrackingStartPosNeedsUpdate;
	float m_trackingStartTime;
	IntRectangle m_bounds;
	int m_timeMs;
	bool m_bCanInteract;
	bool m_bTouchesMoved;
	int m_touch2;
	int m_touch1;
	int m_touchBeganTime;
	int m_touchedId;
	bool* m_selectedById;
	int m_selectedItemId;
	ScrollBar m_scrollBarV;
	ScrollBar m_scrollBarH;
	int m_contentWidth;
	int m_contentHeight;
	Timer m_timer;
	bool m_bAnimating;
	bool m_bPressed;
	Vec2 m_pointerPos;
};

