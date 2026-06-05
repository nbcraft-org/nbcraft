#pragma once

#include "MobRenderer.hpp"

class GiantMobRenderer : public MobRenderer
{
public:
	GiantMobRenderer(Model*, float, float);
	~GiantMobRenderer();

	void scale(const Mob& mob, Matrix& matrix, float a) override;

private:
	float m_scale;
};
