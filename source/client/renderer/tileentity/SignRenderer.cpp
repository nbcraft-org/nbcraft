#include "SignRenderer.hpp"
#include "renderer/ShaderConstants.hpp"

void SignRenderer::renderTyped(SignTileEntity* sign, const Vec3& pos, float partialTicks)
{
    Tile* tile = sign->getTile();
    MatrixStack::Ref mtx = MatrixStack::World.push();
    constexpr float size = 0.6666667f;
    mtx->translate(Vec3(pos.x + 0.5f, pos.y + 0.75f * size, pos.z + 0.5f));
    float rot;
    if (tile == Tile::sign)
    {
        float rot = float(sign->getData() * 360) / 16.0f;
        mtx->rotate(-rot, Vec3::UNIT_Y);
        m_signModel.m_cube2.m_bVisible = true;
    }
    else
    {
        TileData face = sign->getData();
        rot = 0.0f;
        if (face == 2)
            rot = 180.0f;

        if (face == 4)
            rot = 90.0f;

        if (face == 5)
            rot = -90.0f;

        mtx->rotate(-rot, Vec3::UNIT_Y);
        mtx->translate(Vec3(0.0f, -0.3125F, -0.4375F));
        m_signModel.m_cube2.m_bVisible = false;
    }

    bindTexture("item/sign.png");
    MatrixStack::Ref mtx2 = MatrixStack::World.push();
    mtx2->scale(Vec3(size, -size, -size));
    m_signModel.render();
    mtx2.release();
    Font* font = getFont();
    rot = 0.016666668F * size;
    mtx->translate(Vec3(0.0f, 0.5F * size, 0.07F * size));
    mtx->scale(Vec3(rot, -rot, rot));
    //glNormal3f(0.0f, 0.0f, -1.0f * rot);
    int col = 0;

    constexpr int messagesSize = 4;

    for (int i = 0; i < messagesSize; ++i)
    {
        const std::string& msg = sign->m_messages[i];
        if (i == sign->m_selectedLine)
        {
            std::string select = "> " + msg + " <";
            font->draw(select, -font->width(select) / 2, i * 10 - messagesSize * 5, col, false, &m_signModel.m_materials.entity_alphatest);
        }
        else
            font->draw(msg, -font->width(msg) / 2, i * 10 - messagesSize * 5, col, false, &m_signModel.m_materials.entity_alphatest);
    }

    currentShaderDarkColor = Color::WHITE;
}
