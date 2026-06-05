#include "SquidRenderer.hpp"
#include "world/entity/Squid.hpp"

SquidRenderer::SquidRenderer(Model* pModel, float f) : MobRenderer(pModel, f)
{
}

SquidRenderer::~SquidRenderer()
{
}

void SquidRenderer::setupRotations(const Entity& mob, float bob, float bodyRot, Matrix& matrix, float a)
{
    const Squid& squid = (const Squid&)mob;
    float bodyXRot = squid.m_xBodyRotO + (squid.m_xBodyRot - squid.m_xBodyRotO) * a;
    float bodyZRot = squid.m_zBodyRotO + (squid.m_zBodyRot - squid.m_zBodyRotO) * a;
    matrix.translate(Vec3(0.0f, 0.5f, 0.0f));
    matrix.rotate(180.0f - bodyRot, Vec3::UNIT_Y);
    matrix.rotate(bodyXRot, Vec3::UNIT_X);
    matrix.rotate(bodyZRot, Vec3::UNIT_Y);
    matrix.translate(Vec3(0.0f, -1.2f, 0.0f));
}

float SquidRenderer::getBob(const Mob& mob, float a)
{
    const Squid& squid = (const Squid&)mob;
    return squid.m_oldTentacleAngle + (squid.m_tentacleAngle - squid.m_oldTentacleAngle) * a;
}
