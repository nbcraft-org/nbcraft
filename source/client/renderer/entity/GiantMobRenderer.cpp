#include "GiantMobRenderer.hpp"
#include "world/entity/Giant.hpp"

GiantMobRenderer::GiantMobRenderer(Model* pModel, float shadow, float scale) : MobRenderer(pModel, shadow * scale)
{
	m_scale = scale;
}

GiantMobRenderer::~GiantMobRenderer()
{
}

void GiantMobRenderer::scale(const Mob& mob, Matrix& matrix, float a)
{
	matrix.scale(Vec3(m_scale, m_scale, m_scale));
}