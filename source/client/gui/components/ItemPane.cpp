#include "ItemPane.hpp"
#include "client/app/Minecraft.hpp"
#include "client/gui/Gui.hpp"
#include "client/renderer/entity/ItemRenderer.hpp"
#include "renderer/EnableScissorTest.hpp"

ItemPane::ItemPane(ICallback& callback, Minecraft& mc, const IntRectangle& areaRect, int itemCount, int bottom, int screenHeight, bool isVertical)
    : ScrollingPane(
        (isVertical ? FLAG_NOSCROLLH : FLAG_NOSCROLLV) | FLAG_SCROLLFADE,
        areaRect,
        isVertical ? IntRectangle(0, 0, areaRect.w, 22) : IntRectangle(0, 0, 32, areaRect.h),
        isVertical ? 1 : itemCount,
        itemCount,
        Gui::GuiScale,
        IntRectangle()
    )
    , m_minecraft(mc)
    , m_callback(callback)
    , m_screenHeight(screenHeight)
    , m_bVertical(isVertical)
{
    m_area.left -= 4.0f;
    m_area.right += 4.0f;
    m_area.top = 0.0f;
    m_area.bottom = bottom;

    m_sprite         = TextureAtlasSprite(20, 32, 8, 8, "gui/spritesheet.png", 128, 128);
    m_spriteSelected = TextureAtlasSprite(28, 32, 8, 8, "gui/spritesheet.png", 128, 128);

    //m_pPatchLayer = ninePatchFactory.sub_A9E38(rect1);
    //m_pPatchLayer2 = ninePatchFactory.sub_A9E38(rect2);

    /*if ((areaRect.w + 4) != m_sprite.w || m_sprite.h != 22)
    {
        m_pPatchLayer->m_size.x = areaRect.w + 4;
        m_pPatchLayer->m_size.y = 22;

        v23 = 0;
        do
        {
            v24 = v23++;
            m_pPatchLayer->buildQuad(v24);
        } while (v23 != 9);
    }

    if ((areaRect.w + 4) != m_spriteSelected.w || m_spriteSelected.h != 22)
    {
        m_pPatchLayer2->m_size.x = areaRect.w + 4;
        m_pPatchLayer2->m_size.y = 22;

        v26 = 0;
        do
        {
            v27 = v26++;
            m_pPatchLayer2->buildQuad(v27);
        } while (v26 != 9);
    }*/
}

void ItemPane::_drawScrollBar(ScrollBar& scrollBar)
{
    if (scrollBar.alpha <= 0.0f)
        return;

    fill(
         scrollBar.pos.x + 2.0f,
         scrollBar.pos.y,
        (scrollBar.pos.x + 2.0f) + scrollBar.size.x,
         scrollBar.pos.y + scrollBar.size.y,
        Color(1.0f, 1.0f, 1.0f, scrollBar.alpha)
    );
}

void ItemPane::renderBatch(std::vector<GridItem>& items, float a)
{
    const std::vector<CItem*> cItems = m_callback.getItems(*this);
    if (cItems.empty())
        return;

    Font& font = *m_minecraft.m_pFont;
    Textures& textures = *m_minecraft.m_pTextures;
    Tesselator& t = Tesselator::instance;

    {
        mce::EnableScissorTest scissor(
            m_scale * m_xPos,
            m_screenHeight - (m_scale * (m_height + m_yPos)),
            m_scale * m_width,
            m_scale * m_height);

        t.begin(0);
        t.voidBeginAndEndCalls(true);

        for (size_t i = 0; i < items.size(); i++)
        {
            const GridItem& item = items[i];

            TextureAtlasSprite* pSprite;
            if (item.isSelected)
                pSprite = &m_spriteSelected;
            else
                pSprite = &m_sprite;

            float x = Gui::FloorAlignToScreenPixel(item.screenPos.x - 1.0f);
            float y = Gui::FloorAlignToScreenPixel(item.screenPos.y);
            blitNineSlice(textures, "gui/spritesheet.png", x, y, m_width + 4, 22, 3);
        }

        t.voidBeginAndEndCalls(false);
        t.draw(m_materials.ui_textured);


        // 0.12.1 got rid of the batching
        // in our case I don't know if we can do it since renderGuiItem does matrix translations
        //t.begin();
        //t.voidBeginAndEndCalls(true);

        for (size_t i = 0; i < items.size(); i++)
        {
            const GridItem& item = items[i];
            const CItem& cItem = *cItems[item.id];

            float x = Gui::FloorAlignToScreenPixel((m_itemRect.w + item.screenPos.x) - 16.0f);
            float y = Gui::FloorAlignToScreenPixel(item.screenPos.y + 2.0f);
            ItemRenderer::singleton().renderGuiItem(m_minecraft, *cItem.m_pItemStack, x, y);
        }

        //t.voidBeginAndEndCalls(false);
        //t.draw();


        t.begin(0);
        t.voidBeginAndEndCalls(true);

        for (size_t i = 0; i < items.size(); i++)
        {
            const GridItem& item = items[i];
            const CItem& cItem = *cItems[item.id];
            std::string countStr = Util::toString(cItem.m_count);
            int xOffs = item.screenPos.x - 1.0f;
            int yOffs = item.screenPos.y;
            int x, y;

            if (cItem.m_unknown > 0)
            {
                x = Gui::FloorAlignToScreenPixel(xOffs + 2.0f);
                y = Gui::FloorAlignToScreenPixel(yOffs + 6.0f);
                font.drawShadow(cItem.m_subtext, x, y, 0xFFF0F0F0);

                t.scale2d(0.6667f, 0.6667f);

                x = Gui::FloorAlignToScreenPixel((((float)m_itemRect.w + xOffs) - (float)(4 * countStr.size())) * 1.5f);
                y = Gui::FloorAlignToScreenPixel((((float)m_itemRect.h + yOffs) - 8.0f) * 1.5f);
                font.drawShadow(countStr, x, y, 0xFFF0F0F0);
            }
            else
            {
                x = Gui::FloorAlignToScreenPixel(xOffs + 3.0f);
                y = Gui::FloorAlignToScreenPixel(yOffs + 7.0f);
                font.draw(cItem.m_subtext, x, y, 0xC0AAAAAA);

                x = Gui::FloorAlignToScreenPixel(xOffs + 2.0f);
                y = Gui::FloorAlignToScreenPixel(yOffs + 6.0f);
                font.draw(cItem.m_subtext, x, y, 0xC0635558);

                t.scale2d(0.6667f, 0.6667f);

                x = Gui::FloorAlignToScreenPixel((((float)m_itemRect.w + xOffs) - (float)(4 * countStr.size())) * 1.5f);
                y = Gui::FloorAlignToScreenPixel((((float)m_itemRect.h + yOffs) - 8.0f) * 1.5f);
                font.draw(countStr, x, y, 0xC0635558);
            }
            t.resetScale();
        }

        t.voidBeginAndEndCalls(false);
        t.draw(m_materials.ui_texture_and_color);

        if (m_bVertical)
        {
            fillGradient(
                m_xPos,
                m_yPos,
                m_xPos + m_width,
                m_yPos + 28,
                0xBB000000,
                Color::NIL);
            fillGradient(
                m_xPos,
                m_yPos + m_height - 28,
                m_xPos + m_width,
                m_yPos + m_height,
                Color::NIL,
                0xBB000000);
        }
        else
        {
            fillHorizontalGradient(
                m_xPos,
                m_yPos,
                m_xPos + 28,
                m_yPos + m_height,
                0xBB000000,
                Color::NIL);
            fillHorizontalGradient(
                m_width + m_xPos - 28,
                m_yPos,
                m_xPos + m_width,
                m_yPos + m_height,
                Color::NIL,
                0xBB000000);
        }
    }

    _drawScrollBar(m_scrollBarH);
    _drawScrollBar(m_scrollBarV);
}

void ItemPane::onSelect(int id, bool isSelected)
{
    if (isSelected)
        m_callback.onItemSelected(*this, id);
}
