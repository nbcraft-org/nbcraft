#include "PaintingRenderer.hpp"
#include "EntityRenderDispatcher.hpp"
#include "world/level/Level.hpp"
#include "renderer/ShaderConstants.hpp"

PaintingRenderer::PaintingRenderer()
{
}

void PaintingRenderer::render(const Entity& pEntity, const Vec3& pos, float rot, float b)
{
    m_random.setSeed(187);
    MatrixStack::Ref matrix = MatrixStack::World.push();
    matrix->translate(pos);
    matrix->rotate(rot, Vec3::UNIT_Y);
    bindTexture("art/kz.png");
    const Painting& painting = (const Painting&)pEntity;
    PaintingType& motive = painting.getPaintingType();
    constexpr float s = 0.0625f;
    matrix->scale(s);
    renderPainting(painting, motive.m_width, motive.m_height, motive.m_uo, motive.m_vo);
}

void PaintingRenderer::renderPainting(const Painting& painting, int w, int h, int uo, int vo)
{
    float xx0 = (float)(-w) / 2.0f;
    float yy0 = (float)(-h) / 2.0f;
    float z0 = -0.5f;
    float z1 = 0.5f;

    for (int xs = 0; xs < w / 16; ++xs)
    {
        for (int ys = 0; ys < h / 16; ++ys)
        {
            float x0 = xx0 + (float)((xs + 1) * 16);
            float x1 = xx0 + (float)(xs * 16);
            float y0 = yy0 + (float)((ys + 1) * 16);
            float y1 = yy0 + (float)(ys * 16);
            setBrightness(painting, (x0 + x1) / 2.0f, (y0 + y1) / 2.0f);
            float fu0 = (float)(uo + w - xs * 16) / 256.0f;
            float fu1 = (float)(uo + w - (xs + 1) * 16) / 256.0f;
            float fv0 = (float)(vo + h - ys * 16) / 256.0f;
            float fv1 = (float)(vo + h - (ys + 1) * 16) / 256.0f;
            constexpr float bu0 = 0.75f;
            constexpr float bu1 = 0.8125f;
            constexpr float bv0 = 0.0f;
            constexpr float bv1 = 0.0625f;
            constexpr float uu0 = 0.75f;
            constexpr float uu1 = 0.8125f;
            constexpr float uv0 = 0.001953125f;
            constexpr float uv1 = 0.001953125f;
            constexpr float su0 = 0.7519531f;
            constexpr float su1 = 0.7519531f;
            constexpr float sv0 = 0.0f;
            constexpr float sv1 = 0.0625f;
            Tesselator& t = Tesselator::instance;
            t.begin(24);
            t.normal(0.0f, 0.0f, -1.0f);
            t.vertexUV(x0, y1, z0, fu1, fv0);
            t.vertexUV(x1, y1, z0, fu0, fv0);
            t.vertexUV(x1, y0, z0, fu0, fv1);
            t.vertexUV(x0, y0, z0, fu1, fv1);
            t.normal(0.0f, 0.0f, 1.0f);
            t.vertexUV(x0, y0, z1, bu0, bv0);
            t.vertexUV(x1, y0, z1, bu1, bv0);
            t.vertexUV(x1, y1, z1, bu1, bv1);
            t.vertexUV(x0, y1, z1, bu0, bv1);
            t.normal(0.0f, -1.0f, 0.0f);
            t.vertexUV(x0, y0, z0, uu0, uv0);
            t.vertexUV(x1, y0, z0, uu1, uv0);
            t.vertexUV(x1, y0, z1, uu1, uv1);
            t.vertexUV(x0, y0, z1, uu0, uv1);
            t.normal(0.0f, 1.0f, 0.0f);
            t.vertexUV(x0, y1, z1, uu0, uv0);
            t.vertexUV(x1, y1, z1, uu1, uv0);
            t.vertexUV(x1, y1, z0, uu1, uv1);
            t.vertexUV(x0, y1, z0, uu0, uv1);
            t.normal(-1.0f, 0.0f, 0.0f);
            t.vertexUV(x0, y0, z1, su1, sv0);
            t.vertexUV(x0, y1, z1, su1, sv1);
            t.vertexUV(x0, y1, z0, su0, sv1);
            t.vertexUV(x0, y0, z0, su0, sv0);
            t.normal(1.0f, 0.0f, 0.0f);
            t.vertexUV(x1, y0, z0, su1, sv0);
            t.vertexUV(x1, y1, z0, su1, sv1);
            t.vertexUV(x1, y1, z1, su0, sv1);
            t.vertexUV(x1, y0, z1, su0, sv0);
            t.draw(m_shaderMaterials.entity);
        }
    }
}

void PaintingRenderer::setBrightness(const Painting& painting, float ss, float ya)
{
    TilePos tp(painting.m_pos);
    tp.y += ya / 16.0f;

    switch (painting.m_dir)
    {
    case 0:
        tp.x = Mth::floor(painting.m_pos.x + (ss / 16.0f));
        break;
    case 1:
        tp.z = Mth::floor(painting.m_pos.z - (ss / 16.0f));
        break;
    case 2:
        tp.x = Mth::floor(painting.m_pos.x - (ss / 16.0f));
        break;
    case 3:
        tp.z = Mth::floor(painting.m_pos.z + (ss / 16.0f));
        break;
    default:
        break;
    }

    float br = EntityRenderDispatcher::getInstance()->m_pLevel->getBrightness(tp);
    currentShaderDarkColor = Color(br, br, br);
}
