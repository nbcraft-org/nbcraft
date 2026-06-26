#include "ScrollingPane.hpp"
#include "common/Utils.hpp"

#define C_SCROLLBAR_WIDTH 2.3333f
#define C_SCROLLBAR_HEIGHT 2.3333f
#define C_HOLD_DURATION_MS 150

ScrollingPane::ScrollingPane(Flags flags, const IntRectangle& areaRect, const IntRectangle& a4, int columns, int itemCount, float scale, const IntRectangle& itemRect)
    : m_flags(flags)
    , m_scale(scale)
    , m_itemCount(itemCount)
    , m_guiRatio(1.0f / scale)
    , m_areaRect(areaRect)
    , m_area(m_areaRect.x, m_areaRect.x + m_areaRect.w, m_areaRect.y, m_areaRect.y + m_areaRect.h)
    , m_interactArea(m_area)
    , m_bounds(areaRect)
    , m_itemRect(itemRect.w <= 0 ? a4 : itemRect)
    , m_columns(columns)
{
    m_bDragging = false;
    m_bDecelerating = false;
    m_bTracking = false;
    m_bPagingEnabled = false;
    m_touchesMovedTime = 0;
    m_bounceTension = 0.0f;
    m_bounceDamping = 0.0f;
    m_bTrackingStartPosNeedsUpdate = false;
    m_trackingStartTime = 0;
    m_timeMs = 0;
    m_bCanInteract = true;
    m_bTouchesMoved = false;
    m_touch2 = 0;
    m_touch1 = 1;
    m_touchBeganTime = -1;
    m_touchedId = -1;
    m_selectedItemId = -1;
    m_contentWidth = 1;
    m_contentHeight = 1;
    m_bAnimating = false;
    m_bPressed = false;
    m_pointerPos = Vec2(-9999.0f, -9999.0f);

    if (m_columns <= 0)
    {
        m_columns = m_areaRect.w / m_itemRect.w;
        if (m_columns <= 0)
        {
            LOG_W("Columns are 0! Area width is smaller than item width. Setting columns to 1.");
            m_columns = 1;
        }
    }

    m_rows = (m_itemCount - 1) / m_columns + 1;

    m_selectedById = new bool[m_itemCount];
    for (size_t i = 0; i < m_itemCount; i++)
    {
        m_selectedById[i] = false;
    }

    m_scrollBarV.size = Vec2(C_SCROLLBAR_WIDTH, C_SCROLLBAR_HEIGHT);
    m_scrollBarV.pos.x = (float)(m_areaRect.x + m_areaRect.w) - C_SCROLLBAR_WIDTH;
    m_scrollBarV.pos.y = 0.0f;

    m_scrollBarH.size = Vec2(C_SCROLLBAR_WIDTH, C_SCROLLBAR_HEIGHT);
    m_scrollBarH.pos.x = 0.0f;
    m_scrollBarH.pos.y = (float)(m_areaRect.y + m_areaRect.h) - C_SCROLLBAR_HEIGHT;
}

ScrollingPane::~ScrollingPane()
{
    SAFE_DELETE_ARRAY(m_selectedById);
}

void ScrollingPane::_hideScrollIndicators()
{
    m_scrollBarV.fadeState = ScrollBar::FADE_DISAPPEARING;
    m_scrollBarH.fadeState = ScrollBar::FADE_DISAPPEARING;
}

void ScrollingPane::_stopDecelerationAnimation()
{
    m_bDecelerating = false;
    m_bAnimating = false;
}

void ScrollingPane::_onSelect(int id)
{
    if (isMultiselect())
    {
        bool& state = m_selectedById[id];
        onSelect(id, !state);
        state = !state;
    }
    else
    {
        onSelect(id, true);
        if (m_selectedItemId != id)
        {
            // I guess items in single-select mode don't stay selected
            onSelect(m_selectedItemId, false);
            m_selectedItemId = id;
        }
    }
}

void ScrollingPane::_onHoldItem()
{
    if (m_touchedId != -1 && !m_bDragging && m_bTracking)
        _onSelect(m_touchedId);
    m_touchBeganTime = -1;
}

void ScrollingPane::_updateScrollFade(ScrollBar& scrollBar)
{
    switch (scrollBar.fadeState)
    {
    case ScrollBar::FADE_STOPPED:
        break;
    case ScrollBar::FADE_APPEARING:
        scrollBar.alpha += 0.33f;
        if (scrollBar.alpha >= 1.0f)
        {
            scrollBar.alpha = 1.0f;
            scrollBar.fadeState = ScrollBar::FADE_STOPPED;
        }
        break;
    case ScrollBar::FADE_DISAPPEARING:
        scrollBar.alpha -= 0.1f;
        if (scrollBar.alpha <= 0.0f)
        {
            scrollBar.alpha = 0.0;
            scrollBar.fadeState = ScrollBar::FADE_STOPPED;
        }
        break;
    }
}

void ScrollingPane::_updateVerticalScrollIndicator()
{
    float v3;
    if (hasVerticalScrolling() && hasFade())
        v3 = 6.0f;
    else
        v3 = 1.0f;

    float v4 = floorf(((float)(m_bounds.h / m_contentHeight) * ((float)m_bounds.h - v3)) + 0.5f);
    float v5 = Mth::Max(11.333f, v4);
    float v9 = v5;
    float v11;
    if (m_contentOffset.y > 0.0f)
    {
        v11 = 1.0f;
        float v15 = Mth::Max(v5 - m_contentOffset.y, 2.3333f);
        v9 = floorf(v15 + 0.5f);
    }
    else
    {
        if (m_contentOffset.y < (float)(m_bounds.h - m_contentHeight))
        {
            float v14 = Mth::Max((v5 + (float)m_contentHeight - m_bounds.h) + m_contentOffset.y, 2.3333f);
            v9 = floorf(v14 + 0.5f);
            v11 = (float)m_bounds.h - v9 - v3;
        }
        else
        {
            v11 = (-m_contentOffset.y / (float)(m_contentHeight - m_bounds.h)) * (((float)m_bounds.h - v3) - v5);
        }
    }

    m_scrollBarV.size.y = v9;
    m_scrollBarV.pos.y = (float)m_areaRect.y + v11;
}

void ScrollingPane::_updateHorizontalScrollIndicator()
{
    float v3;
    if (hasVerticalScrolling() && hasFade())
        v3 = 6.0f;
    else
        v3 = 1.0f;

    float v4 = floorf(((float)(m_bounds.w / m_contentWidth) * ((float)m_bounds.w - v3)) + 0.5f);
    float v5 = Mth::Max(11.333f, v4);
    float v9 = v5;
    float v11;
    if (m_contentOffset.x > 0.0)
    {
        v11 = 1.0f;
        float v15 = Mth::Max(v5 - m_contentOffset.x, 2.3333f);
        v9 = floorf(v15 + 0.5f);
    }
    else
    {
        if (m_contentOffset.x < (float)(m_bounds.w - m_contentWidth))
        {
            float v14 = Mth::Max(((v5 + m_contentWidth) - m_bounds.w) + m_contentOffset.x, 2.3333f);
            v9 = floorf(v14 + 0.5f);
            v11 = ((float)m_bounds.w - v9) - v3;
        }
        else
        {
            v11 = (-m_contentOffset.x / (float)(m_contentWidth - m_bounds.w)) * ((float)(m_bounds.w - v3) - v5);
        }
    }

    m_scrollBarH.size.x = v9;
    m_scrollBarH.pos.x = (float)m_areaRect.x + v11;
}

void ScrollingPane::_setContentOffsetWithAnimation(const Vec3& contentOffset, bool ignoreScrollbars)
{
    m_inverseContentOffset.x = -m_contentOffset.x;
    m_inverseContentOffset.y = -m_contentOffset.y;

    if (!ignoreScrollbars)
    {
        if (hasFade())
        {
            if (hasHorizontalScrolling())
                _updateHorizontalScrollIndicator();

            if (hasVerticalScrolling())
                _updateVerticalScrollIndicator();
        }
    }
}

void ScrollingPane::_setContentOffset(const Vec3& contentOffset)
{
    _setContentOffsetWithAnimation(contentOffset, false);
}

void ScrollingPane::_setContentOffset(const Vec2& offset)
{
    _setContentOffsetWithAnimation(Vec3(offset.x, offset.y, 0.0f), false);
}

void ScrollingPane::_adjustContentSize()
{
    m_contentWidth  = Mth::Max(m_itemRect.w * m_columns, m_areaRect.w);
    m_contentHeight = Mth::Max(m_itemRect.h * m_rows,    m_areaRect.h);
}

void ScrollingPane::_snapContentOffsetToBounds(bool ignoreScrollbars)
{
    Vec3 offset;

    if (m_bPagingEnabled)
    {
        offset.x = (float)m_bounds.w * floorf((m_contentOffset.x / m_bounds.w) + 0.5f);
        offset.y = (float)m_bounds.h * floorf((m_contentOffset.y / m_bounds.h) + 0.5f);
    }
    else if (hasBounce())
    {
        float v7  = Mth::Min(Mth::Max(m_minContentOffset.x, m_contentOffset.x), 0.0f);
        float v11 = Mth::Min(Mth::Max(m_minContentOffset.y, m_contentOffset.y), 0.0f);
        if (v7 == m_contentOffset.x && v11 == m_contentOffset.y)
            return;
    }

    _setContentOffsetWithAnimation(offset, ignoreScrollbars);
}

void ScrollingPane::_beginTracking(const Vec2& pos, int time)
{
    if (m_bTracking)
        return;

    _stopDecelerationAnimation();
    _adjustContentSize();

    m_minContentOffset.x = (float)(m_bounds.w - m_contentWidth);
    m_minContentOffset.y = m_bounds.h - m_contentHeight;
    m_minContentOffset.z = 0.0f;

    _snapContentOffsetToBounds(false);

    m_trackingStartContentOffset = m_contentOffset;

    m_trackingStartPos = Vec3(pos.x, pos.y, 0.0f);
    m_trackingStartTime = (float)time;

    m_trackingStartContentOffset2 = m_contentOffset;

    m_bDragging = false;
    m_bTracking = true;
    m_bTouchesMoved = false;
}

void ScrollingPane::_stepThroughDecelerationAnimation(bool isSubstep)
{
    if (!m_bDecelerating)
        return;

    int currentTime = getTimeMs();

    // Catch-up frame-stepping if not currently in a recursive substep
    if (!isSubstep)
    {
        float elapsedFrames = ((float)(currentTime - m_timeMs) / 16.667f) + 0.5f;
        int substeps = floorf(elapsedFrames) - 1.0f;
        for (int i = 0; i < substeps; ++i)
        {
            _stepThroughDecelerationAnimation(true);
        }
    }

    // Calculate new position based on current velocity
    Vec3 nextOffset = m_contentOffset + m_scrollVelocity;

    // Handle clamping when bounce limits are disabled
    if (!hasBounce())
    {
        float clampedX = Mth::Min(Mth::Max(m_minContentOffset.x, nextOffset.x), 0.0f);
        if (nextOffset.x != clampedX)
        {
            nextOffset.x = clampedX;
            m_scrollVelocity.x = 0.0f;
        }

        float clampedY = Mth::Min(Mth::Max(m_minContentOffset.y, nextOffset.y), 0.0f);
        if (nextOffset.y != clampedY)
        {
            nextOffset.y = clampedY;
            m_scrollVelocity.y = 0.0f;
        }
    }

    if (isSubstep)
    {
        m_contentOffset = nextOffset;
    }
    else
    {
        _setContentOffset(Vec2(nextOffset.x, nextOffset.y));
    }

    // Apply friction/damping to velocity if paging is not enabled
    if (!m_bPagingEnabled)
    {
        m_scrollVelocity.x *= 0.95f;
        m_scrollVelocity.y *= 0.95f;
    }

    float absVelX = Mth::abs(m_scrollVelocity.x);
    float absVelY = Mth::abs(m_scrollVelocity.y);

    // Determine whether to continue animating
    bool continueAnimation = isSubstep;
    if (!isSubstep)
    {
        if (absVelX > 0.05f || absVelY > 0.05f)
        {
            continueAnimation = true;
        }
        else
        {
            _hideScrollIndicators();
            if (absVelX > 0.01f || absVelY > 0.01f)
            {
                continueAnimation = true;
            }
        }
    }

    if (!continueAnimation)
    {
        // Stop deceleration and call completion handler
        m_bDecelerating = false;
        didEndDecelerating();
        return;
    }

    // Apply spring tension/damping forces when outside deceleration limits
    if (hasBounce())
    {
        float diffX = 0.0f;
        if (nextOffset.x < m_decelLimitMin.x)
            diffX = m_decelLimitMin.x - nextOffset.x;
        else if (nextOffset.x > m_decelLimitMax.x)
            diffX = m_decelLimitMax.x - nextOffset.x;

        float diffY = 0.0f;
        if (nextOffset.y < m_decelLimitMin.y)
            diffY = m_decelLimitMin.y - nextOffset.y;
        else if (nextOffset.y > m_decelLimitMax.y)
            diffY = m_decelLimitMax.y - nextOffset.y;

        if (diffX != 0.0f)
        {
            if (m_scrollVelocity.x * diffX > 0.0f)
                m_scrollVelocity.x = m_bounceDamping * diffX;
            else
                m_scrollVelocity.x += diffX * m_bounceTension;
        }

        if (diffY != 0.0f)
        {
            if (m_scrollVelocity.y * diffY > 0.0f)
                m_scrollVelocity.y = m_bounceDamping * diffY;
            else
                m_scrollVelocity.y += diffY * m_bounceTension;
        }
    }

    if (!isSubstep)
    {
        m_timeMs = currentTime;
    }
}

void ScrollingPane::_touchesMoved(const Vec2& pos, int time)
{
    m_bTouchesMoved = true;

    Vec2 diff(pos.x - m_trackingStartPos.x, pos.y - m_trackingStartPos.y);

    if (!m_bDragging)
    {
        bool canScrollH = hasHorizontalScrolling();
        bool canScrollV = hasVerticalScrolling();

        bool exceededThresholdH = canScrollH && (Mth::abs(diff.x) >= 5.0f);
        bool exceededThresholdV = canScrollV && (Mth::abs(diff.y) >= 5.0f);

        if (exceededThresholdH || exceededThresholdV)
        {
            willBeginDragging();
            m_bDragging = true;
            m_bTrackingStartPosNeedsUpdate = true;

            if (hasFade())
            {
                if (canScrollH && m_contentWidth > m_bounds.w)
                    m_scrollBarH.fadeState = ScrollBar::FADE_APPEARING;
                if (canScrollV && m_contentHeight > m_bounds.h)
                    m_scrollBarV.fadeState = ScrollBar::FADE_APPEARING;
            }
        }

        // Do not update offsets if we haven't triggered a dragging state yet
        if (!m_bDragging)
            return;
    }

    // Determine raw target offsets based on axis lock flags
    Vec2 target;
    target.x = !hasHorizontalScrolling() ? m_contentOffset.x : (diff.x + m_trackingStartContentOffset.x);
    target.y = !hasVerticalScrolling()   ? m_contentOffset.y : (diff.y + m_trackingStartContentOffset.y);

    Vec2 offset;

    // Apply strict clamping (No Bounce) or elastic boundary stretching (Bounce)
    if (!hasBounce())
    {
        offset.x = Mth::Min(Mth::Max(m_minContentOffset.x, target.x), 0.0f);
        offset.y = Mth::Min(Mth::Max(m_minContentOffset.y, target.y), 0.0f);
    }
    else
    {
        // Horizontal elastic boundary stretch (0.5 rubberband factor)
        if (target.x < m_minContentOffset.x)
        {
            offset.x = m_minContentOffset.x + (target.x - m_minContentOffset.x) * 0.5f;
        }
        else if (target.x > 0.0f)
        {
            offset.x = target.x * 0.5f;
        }
        else
        {
            offset.x = target.x;
        }

        // Vertical elastic boundary stretch (0.5 rubberband factor)
        if (target.y < m_minContentOffset.y)
        {
            offset.y = m_minContentOffset.y + (target.y - m_minContentOffset.y) * 0.5f;
        }
        else if (target.y > 0.0f)
        {
            offset.y = target.y * 0.5f;
        }
        else
        {
            offset.y = target.y;
        }
    }

    if (m_bTrackingStartPosNeedsUpdate)
    {
        m_bTrackingStartPosNeedsUpdate = false;
        m_trackingStartPos.x = pos.x;
        m_trackingStartPos.y = pos.y;
        m_trackingStartPos.z = 0.0f;
    }
    else
    {
        _setContentOffset(offset);
        m_touchesMovedTime = time;
    }
}

void ScrollingPane::_startDecelerationAnimation(bool isSubstep)
{
    // Calculate distance traveled during tracking
    Vec2 delta(m_contentOffset.x - m_trackingStartContentOffset2.x,
               m_contentOffset.y - m_trackingStartContentOffset2.y);

    // Calculate tracking velocity
    int currentTime = getTimeMs();
    float timeFactor = ((float)currentTime - m_trackingStartTime) / 15.0f;

    Vec2 velocity = delta / timeFactor;

    m_scrollVelocity.x = velocity.x;
    m_scrollVelocity.y = velocity.y;
    m_scrollVelocity.z = 0.0f;

    // Initialize default deceleration boundaries
    m_decelLimitMin = m_minContentOffset;
    m_decelLimitMax = Vec3::ZERO;

    // Calculate clamped page boundaries if paging is enabled
    if (m_bPagingEnabled)
    {
        float w = m_bounds.w;
        float h = m_bounds.h;

        m_decelLimitMin.x = Mth::Max(m_minContentOffset.x, w * floorf(m_touchesEndedContentOffset.x / w));
        m_decelLimitMin.y = Mth::Max(m_minContentOffset.y, h * floorf(m_touchesEndedContentOffset.y / h));

        m_decelLimitMax.x = Mth::Min(0.0f, w * ceilf(m_touchesEndedContentOffset.x / w));
        m_decelLimitMax.y = Mth::Min(0.0f, h * ceilf(m_touchesEndedContentOffset.y / h));
    }

    // Set up bounce mechanics variables
    float thresholdVelocity = 0.33333f;
    m_bounceTension = 0.03f;
    m_bounceDamping = 0.08f;

    if (m_bPagingEnabled)
    {
        thresholdVelocity = 1.3333f;
        m_bounceTension = 0.15f;
    }

    // Decelerate if we are in a substep, or if movement speed exceeds the idle threshold
    bool shouldDecelerate = isSubstep ||
        (Mth::abs(m_scrollVelocity.x) > thresholdVelocity) ||
        (Mth::abs(m_scrollVelocity.y) > thresholdVelocity);

    if (shouldDecelerate)
    {
        m_bDecelerating = true;
        m_bAnimating = true;
        m_timeMs = getTimeMs();

        willBeginDecelerating();
    }
}

void ScrollingPane::_touchesEnded(const Vec2& pos, int time)
{
    m_touch1 = 0;

    m_bTracking = false;

    if (m_bDragging)
    {
        m_touchesMovedTime = m_touchesMovedTime;
        m_bDragging = false;

        if ((time - m_touchesMovedTime) <= 100.0f)
        {
            m_touchesEndedContentOffset = m_contentOffset;
            _startDecelerationAnimation(false);
        }
        didEndDragging();
    }

    if (!m_bDecelerating)
    {
        float inverseY = m_inverseContentOffset.y;
        if (inverseY >= 0.0f && inverseY <= m_areaRect.h)
        {
            _snapContentOffsetToBounds(true);
            _hideScrollIndicators();
        }
        else
        {
            m_touchesEndedContentOffset = m_contentOffset;
            _startDecelerationAnimation(true);
        }
    }

    if (sqrtf(
            (
                ((m_trackingStartPos.y - pos.y) * (m_trackingStartPos.y - pos.y)) +
                ((m_trackingStartPos.x - pos.x) * (m_trackingStartPos.x - pos.x))
            )
            + (m_trackingStartPos.z * m_trackingStartPos.z)
        ) <= 6.0f)
    {
        if (m_touchedId >= 0)
            _onSelect(m_touchedId);
    }

    m_touch2 = 0;
}

void ScrollingPane::_touchesCancelled(const Vec2& pos, int time)
{
    _touchesEnded(pos, time);
}

ScrollingPane::GridItem ScrollingPane::_getItemForPos(const Vec2& pos, bool b)
{
    Vec2 nPos = pos;

    if (b)
    {
        nPos.x = pos.x - m_areaRect.x;
        nPos.y = pos.y - m_areaRect.y;
    }

    nPos += m_inverseContentOffset;

    if (hasHorizontalWrapping())
        nPos.x = fmodf(nPos.x, m_itemRect.w * m_columns);

    if (hasVerticalWrapping())
        nPos.y = fmodf(nPos.y, m_itemRect.h * m_rows);

    GridItem item;
    item.screenPos.x = nPos.x / m_itemRect.w;
    item.screenPos.y = nPos.y / m_itemRect.h;
    item.x = item.screenPos.x;
    item.y = item.screenPos.y;
    item.id = (int)item.screenPos.x + m_columns * (int)item.screenPos.y;

    return item;
}

void ScrollingPane::_updateHighlightItem(const Vec2& pos)
{
    GridItem item = _getItemForPos(pos, true);
    if (item.id >= 0 && item.id < m_itemCount)
    {
        m_touchedId = item.id;
        if (!m_interactArea.isInside(pos.x, pos.y))
            m_touchedId = -1;

        if (m_interactArea.isInside(pos.x, pos.y))
            m_touchBeganTime = getTimeMs();
        else
            m_touchBeganTime = -1;
    }
    else
    {
        m_touchedId = -1;
        m_touchBeganTime = -1;
    }
}

void ScrollingPane::_touchesBegan(const Vec2& pos, int time)
{
    if (!m_bCanInteract)
        return;

    m_touch1 = 1;
    _beginTracking(pos, time);
    m_touch2 = 2;

    _updateHighlightItem(pos);
}

void ScrollingPane::_handleUserInput(const MenuPointer& pointer)
{
    Vec2 pointerPos((float)pointer.x * m_guiRatio, (float)pointer.y * m_guiRatio);
    int currentTime = getTimeMs();

    bool hasPointerMoved = m_pointerPos != pointerPos;
    m_pointerPos = pointerPos;

    bool handledTransition = false;

    // Handle touch phase transitions
    if (m_touch1 > 0 && m_bPressed)
    {
        if (!pointer.isPressed)
        {
            _touchesEnded(pointerPos, currentTime);
            handledTransition = true;
        }
    }
    else if (pointer.isPressed && !m_bPressed && m_area.isInside(pointerPos.x, pointerPos.y))
    {
        _touchesBegan(pointerPos, currentTime);
        handledTransition = true;
    }

    // Handle continuous movement phase (dragging) if no transition occurred
    if (!handledTransition)
    {
        if (m_touch2 > 0 && hasPointerMoved && pointer.isPressed)
        {
            _touchesMoved(pointerPos, currentTime);
        }
    }

    // Check for hold
    if (m_touchBeganTime >= 0 && (getTimeMs() - m_touchBeganTime) >= C_HOLD_DURATION_MS)
    {
        _onHoldItem();
    }

    m_bPressed = pointer.isPressed;
}

void ScrollingPane::tick()
{
    if (hasFade())
    {
        _updateScrollFade(m_scrollBarV);
        _updateScrollFade(m_scrollBarH);
    }
}

void ScrollingPane::setSelected(int id, bool isSelected)
{
    if (isMultiselect())
    {
        m_selectedById[id] = isSelected;
    }
    else if (isSelected)
    {
        m_selectedItemId = id;
    }
    else if (m_selectedItemId == id)
    {
        // deselect
        m_selectedItemId = -1;
    }
}

void ScrollingPane::translate(const Vec2& t)
{
    m_areaRect.x += t.x;
    m_areaRect.y += t.y;
    /*m_area.left += x;
    m_area.right += x;
    m_area.top += y;
    m_area.bottom += y;
    m_interactArea.left += x;
    m_interactArea.right += x;
    m_interactArea.top += y;
    m_interactArea.bottom += y;*/
    m_scrollBarH.pos += t;
    m_scrollBarV.pos += t;
}

bool ScrollingPane::getGridItemFor_slow(int id, GridItem& result)
{
    GridItem topLeftItem = _getItemForPos(Vec2::ZERO, false);

    GridItem v15 = _getItemForPos(Vec2((float)m_areaRect.w - 1.0f, (float)m_areaRect.h - 1.0f), false);
    
    float v9  = ((float)m_areaRect.x - m_inverseContentOffset.x) + (topLeftItem.screenPos.x - topLeftItem.x);
    float v10 = ((float)m_areaRect.y - m_inverseContentOffset.y) + (topLeftItem.screenPos.y - topLeftItem.y);

    int width = id % m_columns;
    int height = id / m_columns;

    result.id = id;
    result.x = width;
    result.y = height;
    result.screenPos.x = (float)(m_itemRect.w * width)  + v9;
    result.screenPos.y = (float)(m_itemRect.h * height) + v10;

    return width >= topLeftItem.x
        && width <= v15.x
        && height >= topLeftItem.y
        && height <= v15.y;
}

void ScrollingPane::render(const MenuPointer& pointer, float a)
{
    _handleUserInput(pointer);

    m_timer.advanceTime(false);

    if (m_bAnimating)
    {
        // Only step deceleration when no active multitouch is present
        if (m_touch2 == 0)
        {
            for (int i = 0; i < m_timer.m_ticks; ++i)
            {
                _stepThroughDecelerationAnimation(false);
            }
            m_timeMs = getTimeMs();
        }
    }

    std::vector<GridItem> items;

    // Retrieve active grid item positions for rendering bounds
    GridItem startItem = _getItemForPos(Vec2::ZERO, false);
    GridItem endItem   = _getItemForPos(Vec2(m_areaRect.w - 1, m_areaRect.h - 1), false);

    // Calculate rendering offsets
    Vec2 basePos(
        ((float)m_areaRect.x - m_inverseContentOffset.x) + (startItem.screenPos.x - startItem.x),
        ((float)m_areaRect.y - m_inverseContentOffset.y) + (startItem.screenPos.y - startItem.y)
    );

    // Loop through visible grid columns and rows
    for (int row = startItem.y; row <= endItem.y; ++row)
    {
        for (int col = startItem.x; col <= endItem.x; ++col)
        {
            int itemIdx = col + m_columns * row;

            // Bounds check for the computed index
            if (row >= 0 && itemIdx >= 0 && itemIdx < m_itemCount)
            {
                // Check wrapper configurations or standard bounds limit
                if (hasHorizontalWrapping() || (col >= 0 && col < m_columns))
                {
                    GridItem item;
                    {
                        item.id = itemIdx;
                        item.screenPos.x = (float)(m_itemRect.w * col) + basePos.x;
                        item.screenPos.y = (float)(m_itemRect.h * row) + basePos.y;
                        item.x = item.screenPos.x;
                        item.y = item.screenPos.y;
                        item.isSelected = isSelected(itemIdx);
                    }

                    items.push_back(item);
                }
            }
        }
    }

    renderBatch(items, a);
}

void ScrollingPane::renderBatch(std::vector<GridItem>& items, float a)
{
    for (size_t i = 0; i < items.size(); i++)
    {
        renderItem(items[i], a);
    }
}

void ScrollingPane::renderItem(GridItem& item, float a)
{
}
