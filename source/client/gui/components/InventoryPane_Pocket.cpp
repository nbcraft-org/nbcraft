#include "InventoryPane_Pocket.hpp"
#include "client/app/Minecraft.hpp"
#include "client/gui/Gui.hpp"
#include "client/renderer/entity/ItemRenderer.hpp"
#include "client/renderer/renderer/RenderMaterialGroup.hpp"
#include "renderer/EnableScissorTest.hpp"
#include "renderer/ShaderConstants.hpp"

const Color InventoryPane_Pocket::BACKGROUND_COLOR = Color(0xFF333333);

InventoryPane_Pocket::Materials::Materials()
{
    MATERIAL_PTR(common, ui_item);
    MATERIAL_PTR(common, ui_item_glint);
}

InventoryPane_Pocket::InventoryPane_Pocket(ICallback& callback, Minecraft& mc, const IntRectangle& areaRect, int width, float paddingH, int itemCount)
    : ScrollingPane(FLAG_NOSCROLLH | FLAG_SCROLLFADE, areaRect, IntRectangle(0, 0, 32, 32), 0, itemCount, Gui::GuiScale * Minecraft::GetRenderScaleMultiplier(), IntRectangle::ZERO)
    , m_callback(callback)
    , m_minecraft(mc)
{
    m_widthOrSomething = width;
    m_pRectArea = new RectangleArea(0, 0, 0, 0);
    m_paddingX = 2;
    m_paddingY = 4;
    m_selectedId = -1;
    m_clickAnimTick = -1;

    m_area.left = (float)areaRect.x - paddingH;
    m_area.right = (float)(areaRect.w + areaRect.x) + paddingH;
    m_area.top -= 6.0f;
    m_area.bottom += 6.0;
}

InventoryPane_Pocket::~InventoryPane_Pocket()
{
    SAFE_DELETE(m_pRectArea);
}

void InventoryPane_Pocket::_drawScrollBar(ScrollBar& scrollBar)
{
    if (scrollBar.alpha <= 0.0f)
        return;

    fill(
        float(m_xPos + m_width) - scrollBar.size.x,
        scrollBar.pos.y,
        float(m_xPos + m_width),
        scrollBar.pos.y + scrollBar.size.y,
        Color(0xAAAAAA, scrollBar.alpha)
    );
}

void InventoryPane_Pocket::tick()
{
    --m_clickAnimTick;
    ScrollingPane::tick();
}

void InventoryPane_Pocket::renderBatch(std::vector<GridItem>& items, float a)
{
    fill(
        m_xPos - 1 - m_paddingX,
        m_yPos - m_paddingY,
        m_xPos + 1 + m_paddingX + m_width,
        m_yPos + m_paddingY + m_height,
        BACKGROUND_COLOR);

    currentShaderColor = Color::WHITE;

    const std::vector<const ItemStack*>& itemStacks = m_callback.getItems(*this);
    {
        mce::EnableScissorTest scissor(
            m_scale * m_xPos,
            Minecraft::GetHeightP() - (m_scale * (m_height + m_yPos)),
            m_scale * m_width,
            m_scale * m_height
        );

        Tesselator& t = Tesselator::instance;
        Textures& textures = *m_minecraft.m_pTextures;

        t.begin(0);
        t.voidBeginAndEndCalls(true);

        for (size_t i = 0; i < items.size(); i++)
        {
            const GridItem& item = items[i];
            blit(
                item.screenPos.x, item.screenPos.y,
                200, 46,
                m_itemRect.w, m_itemRect.h,
                16, 16
            );
        }

        textures.loadAndBindTexture("gui/gui.png");

        t.voidBeginAndEndCalls(false);
        t.draw(m_materials.ui_textured);

        /*t.begin(0);
        t.voidBeginAndEndCalls(true);*/

        for (size_t i = 0; i < items.size(); i++)
        {
            const GridItem& item = items[i];
            const ItemStack* pItemStack = itemStacks[item.id];
            if (!pItemStack)
                continue;

            t.enableColor();
            if (m_callback.isAllowed(item.id))
            {
                if (m_clickAnimTick > 0 && m_selectedId == item.id)
                {
                    int v27 = 0xFFFFFFF1 * m_clickAnimTick;
                    t.color(v27 + 255, v27 + 255, v27 + 255, pItemStack->m_count > 0 ? 255 : 96);
                }
                else
                {
                    t.color(255, 255, 255, pItemStack->m_count > 0 ? 255 : 96);
                }
            }
            else
            {
                t.color(64, 64, 64);
            }
            t.noColor();

            float x = Gui::FloorAlignToScreenPixel(item.screenPos.x + 4.0f + 4.0f);
            float y = Gui::FloorAlignToScreenPixel(item.screenPos.y + 4.0f + 4.0f);
            ItemRenderer::singleton().renderGuiItem(m_minecraft, *pItemStack, x, y);
            ItemRenderer::singleton().renderGuiItemOverlay(m_minecraft, *pItemStack, x, y);
        }

        /*t.voidBeginAndEndCalls(false);
        t.draw(m_itemMaterials.ui_item);*/

        const GameMode* pGameMode = m_minecraft.getLocalPlayerGameMode();
        if (pGameMode && !pGameMode->isCreativeType())
        {
            float mtxScale = Gui::InvGuiScale + Gui::InvGuiScale;
            float posScale = Gui::GuiScale * 0.5f;

            MatrixStack::Ref matrix = MatrixStack::World.push();
            matrix->scale(Vec3(mtxScale, mtxScale, 1.0f));

            for (size_t i = 0; i < items.size(); i++)
            {
                const GridItem& item = items[i];
                const ItemStack* pItemStack = itemStacks[item.id];
                if (!pItemStack)
                    continue;

                float x = Gui::FloorAlignToScreenPixel((item.screenPos.x + 4.0f + 3.0f) * posScale);
                float y = Gui::FloorAlignToScreenPixel((item.screenPos.y + 4.0f + 3.0f) * posScale);
                ItemRenderer::singleton().renderGuiItemText(m_minecraft, *pItemStack, x, y);
            }
        }
    }

    fillGradient(
        m_xPos - m_paddingX,
        m_yPos,
        m_xPos + m_width + m_paddingX,
        m_yPos + 20,
        0x99000000,
        Color::NIL);
    fillGradient(
        m_xPos - m_paddingX,
        m_yPos + m_height - 20,
        m_xPos + m_width + m_paddingX,
        m_yPos + m_height,
        Color::NIL,
        0x99000000);

    _drawScrollBar(m_scrollBarH);
    _drawScrollBar(m_scrollBarV);
}

void InventoryPane_Pocket::onSelect(int id, bool isSelected)
{
    if (!m_callback.isAllowed(id) || !isSelected)
        return;

    bool success = m_callback.addItem(*this, id);
    if (success)
    {
        m_selectedId = id;
        m_clickAnimTick = 7;
    }
}