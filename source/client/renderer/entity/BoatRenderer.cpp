#include "BoatRenderer.hpp"
#include "client/model/models/BoatModel.hpp"
#include "world/entity/vehicle/Boat.hpp"

BoatRenderer::BoatRenderer() : m_pModel(new BoatModel())
{
	m_shadowRadius = 0.5f;
}

void BoatRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
    const Boat& boat = (const Boat&)entity;
    MatrixStack::Ref matrix = MatrixStack::World.push();
    matrix->translate(pos);
    matrix->rotate(180.0f - rot, Vec3::UNIT_Y);
    float hurt = boat.m_hurtTime - a;
    float dmg = boat.m_damage - a;
    if (dmg < 0.0f)
        dmg = 0.0f;

    if (hurt > 0.0f)
        matrix->rotate(Mth::sin(hurt) * hurt * dmg / 10.0f * boat.m_hurtDir, Vec3::UNIT_X);

    bindTexture("item/boat.png");
    matrix->scale(Vec3(-1.0f, -1.0f, 1.0f));
    m_pModel->render(0.0f, 0.0f, -0.1f, 0.0f, 0.0f, 0.0625f);
}
