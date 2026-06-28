#include "GhastRenderer.hpp"
#include "world/entity/Ghast.hpp"
#include "client/model/models/GhastModel.hpp"
#include "renderer/ShaderConstants.hpp"

GhastRenderer::GhastRenderer() : MobRenderer(new GhastModel(), 0.5f)
{
}

GhastRenderer::~GhastRenderer()
{
}

void GhastRenderer::scale(const Mob& mob, Matrix& matrix, float a)
{
	const Ghast& ghast = static_cast<const Ghast&>(mob);
	float ss = (float(ghast.m_oCharge) + float(ghast.m_charge - ghast.m_oCharge) * a) / 20.0f;
	if (ss < 0.0f)
	{
		ss = 0.0f;
	}

	ss = 1.0f / (ss * ss * ss * ss * ss * 2.0f + 1.0f);
	float s = (8.0f + ss) / 2.0f;
	float hs = (8.0f + 1.0f / ss) / 2.0f;
	matrix.scale(Vec3(hs, s, hs));
}
