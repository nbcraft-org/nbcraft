#include "WolfRenderer.hpp"
#include "world/entity/Wolf.hpp"

WolfRenderer::WolfRenderer(Model* pModel, float f) : MobRenderer(pModel, f)
{
}

WolfRenderer::~WolfRenderer()
{
}

float WolfRenderer::getBob(const Mob& mob, float time)
{
	return ((const Wolf&)mob).getTailAngle();
}
