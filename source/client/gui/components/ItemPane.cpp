#include "ItemPane.hpp"
#include "client/gui/Gui.hpp"

ItemPane::ItemPane(ICallback& callback, Textures& textures, const IntRectangle& areaRect, int itemCount, int bottom, int screenHeight, bool isVertical)
    : ScrollingPane(
        (isVertical ? FLAG_NOSCROLLH : FLAG_NOSCROLLV) | FLAG_SCROLLFADE,
        areaRect,
        isVertical ? IntRectangle(0, 0, areaRect.w, 22) : IntRectangle(0, 0, 32, areaRect.h),
        isVertical ? 1 : itemCount,
        itemCount,
        Gui::GuiScale,
        IntRectangle()
    )
    , m_textures(textures)
    , m_callback(callback)
    , m_screenHeight(screenHeight)
    , m_bVertical(isVertical)
{
    m_area.left -= 4.0f;
    m_area.right += 4.0f;
    m_area.top = 0.0f;
    m_area.bottom = bottom;

    // @TODO: "gui/spritesheet.png" NinePatchFactory

    IntRectangle rect1(20, 32, 8, 8);
    IntRectangle rect2(28, 32, 8, 8);

    //m_pPatchLayer = ninePatchFactory.sub_A9E38(rect1);
    //m_pPatchLayer2 = ninePatchFactory.sub_A9E38(rect2);

    // need a nineslice function that takes u,v, and uvWidth, uvHeight
    //blitNineSlice(m_textures, "gui/spritesheet.png", m_xPos, m_yPos, m_width, m_height, 3);

    /*if ((areaRect.w + 4) != rect1.w || rect1.h != 22.0f)
    {
        m_pPatchLayer->m_size.x = areaRect.w + 4;
        m_pPatchLayer->m_size.y = 22.0f;

        v23 = 0;
        do
        {
            v24 = v23++;
            m_pPatchLayer->buildQuad(v24);
        } while (v23 != 9);
    }

    if ((areaRect.w + 4) != rect2.w || rect2.h != 22.0f)
    {
        m_pPatchLayer2->m_size.x = areaRect.w + 4;
        m_pPatchLayer2->m_size.y = 22.0f;

        v26 = 0;
        do
        {
            v27 = v26++;
            m_pPatchLayer2->buildQuad(v27);
        } while (v26 != 9);
    }*/
}