#include "MinecartRenderer.hpp"
#include "client/model/models/MinecartModel.hpp"
#include "client/renderer/entity/EntityRenderDispatcher.hpp"
#include "world/entity/Minecart.hpp"

MinecartRenderer::MinecartRenderer() : m_pModel(new MinecartModel())
{
	m_shadowRadius = 0.5f;
}

void MinecartRenderer::_renderInnerTile(const Entity& entity, Tile* tile, float a)
{
    MatrixStack::Ref tileMatrix = MatrixStack::World.push();
    bindTexture(C_TERRAIN_NAME);
    constexpr float ss = 0.75f;
    tileMatrix->scale(ss);
    tileMatrix->translate(Vec3(0.0f, 0.3125f, 0.0f));
    tileMatrix->rotate(90.0f, Vec3::UNIT_Y);
    m_pDispatcher->m_tileRenderer->renderTile(
        FullTile(tile, 0),
        m_shaderMaterials.entity,
        entity.getBrightness(a)
    );
}

void MinecartRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
    _setupShaderParameters(entity, a);
    const Minecart& cart = (const Minecart&)entity;
    MatrixStack::Ref matrix = MatrixStack::World.push();
    Vec3 cPos = pos;
    Vec3 smoothPos = cart.m_oPos.interpolateTo(cart.m_pos, a);
    float smoothX = Mth::Lerp(cart.m_oRot.pitch, cart.m_rot.pitch, a);

    Vec3* railPos = cart.getOnRailPos(smoothPos);
    if (railPos)
    {
        constexpr float r = 0.3f;
        Vec3* p0 = cart.getPosOffs(smoothPos, r);
        Vec3* p1 = cart.getPosOffs(smoothPos, -r);
        if (!p0)
            p0 = railPos;

        if (!p1)
            p1 = railPos;

        cPos.x += railPos->x - smoothPos.x;
        cPos.y += (p0->y + p1->y) / 2.0f - smoothPos.y;
        cPos.z += railPos->z - smoothPos.z;
        Vec3 dir = *p1 - *p0;
        if (dir.length() != 0.0f)
        {
            dir = dir.normalize();
            rot = Mth::atan2(dir.z, dir.x) * 180.0f / M_PI;
            smoothX = Mth::atan(dir.y) * 73.0f;
        }

        if (railPos != p0)
            SAFE_DELETE(p0);
        if (railPos != p1)
            SAFE_DELETE(p1);
        SAFE_DELETE(railPos);
    }

    matrix->translate(cPos);
    matrix->rotate(180.0f - rot, Vec3::UNIT_Y);
    matrix->rotate(-smoothX, Vec3::UNIT_Z);

	// bobbing effect
    float hurt = cart.m_hurtTime - a;
    float dmg = Mth::Max(cart.m_damage - a, 0.0f);
    if (hurt > 0.0f)
        matrix->rotate(Mth::sin(hurt) * hurt * dmg / 10.0f * cart.m_hurtDir, Vec3::UNIT_X);

    if (cart.m_type != Minecart::TYPE_DEFAULT)
        _renderInnerTile(entity, cart.m_type == Minecart::TYPE_CHEST ? Tile::chest : Tile::furnace, a);

    bindTexture("item/cart.png");
    matrix->scale(Vec3(-1.0f, -1.0f, 1.0f));
    m_pModel->render(0.0f, 0.0f, -0.1f, 0.0f, 0.0f, 0.0625f);
}
