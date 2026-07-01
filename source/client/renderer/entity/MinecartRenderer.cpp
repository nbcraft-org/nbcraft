#include "MinecartRenderer.hpp"
#include "client/model/models/MinecartModel.hpp"
#include "world/entity/vehicle/Minecart.hpp"

MinecartRenderer::MinecartRenderer() : m_pModel(new MinecartModel())
{
	m_shadowRadius = 0.5f;
}

void MinecartRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
    const Minecart& cart = (const Minecart&)entity;
    Vec3 cPos = pos;
    MatrixStack::Ref matrix = MatrixStack::World.push();
    Vec3 smoothPos = cart.m_oPos + (cart.m_pos - cart.m_oPos) * a;
    constexpr float r = 0.3f;
    Vec3* p = cart.getPos(smoothPos);
    float xRot = cart.m_oRot.pitch + (cart.m_rot.pitch - cart.m_oRot.pitch) * a;
    if (p)
    {
        Vec3* p0 = cart.getPosOffs(smoothPos, r);
        Vec3* p1 = cart.getPosOffs(smoothPos, -r);
        if (!p0)
            p0 = p;

        if (!p1)
            p1 = p;

        cPos.x += p->x - smoothPos.x;
        cPos.y += (p0->y + p1->y) / 2.0 - smoothPos.y;
        cPos.z += p->z - smoothPos.z;
        Vec3 dir = *p1 - *p0;
        if (dir.length() != 0.0)
        {
            dir = dir.normalize();
            rot = Mth::atan2(dir.z, dir.x) * 180.0 / M_PI;
            xRot = Mth::atan(dir.y) * 73.0;
        }

        if (p != p0)
            SAFE_DELETE(p0);
        if (p != p1)
            SAFE_DELETE(p1);
        SAFE_DELETE(p);
    }

    matrix->translate(cPos);
    matrix->rotate(180.0f - rot, Vec3::UNIT_Y);
    matrix->rotate(-xRot, Vec3::UNIT_Z);
    float hurt = cart.m_hurtTime - a;
    float dmg = cart.m_damage - a;
    if (dmg < 0.0f)
        dmg = 0.0f;

    if (hurt > 0.0f)
        matrix->rotate(Mth::sin(hurt) * hurt * dmg / 10.0f * cart.m_hurtDir, Vec3::UNIT_X);

    if (cart.m_type != Minecart::DEFAULT)
    {
        MatrixStack::Ref tileMatrix = MatrixStack::World.push();
        bindTexture(C_TERRAIN_NAME);
        float ss = 0.75F;
        tileMatrix->scale(ss);
        tileMatrix->translate(Vec3(0.0f, 0.3125f, 0.0f));
        tileMatrix->rotate(90.0f, Vec3::UNIT_Y);
        if (cart.m_type == 1)
            m_tileRenderer.renderTile(FullTile(Tile::chest, 0), m_shaderMaterials.entity, entity.getBrightness(a));
        else if (cart.m_type == 2)
            m_tileRenderer.renderTile(FullTile(Tile::furnace, 0), m_shaderMaterials.entity, entity.getBrightness(a));
    }

    bindTexture("item/cart.png");
    matrix->scale(Vec3(-1.0f, -1.0f, 1.0f));
    m_pModel->render(0.0f, 0.0f, -0.1f, 0.0f, 0.0f, 0.0625f);
}
