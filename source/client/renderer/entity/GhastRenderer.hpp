#pragma once

#include "MobRenderer.hpp"

class GhastRenderer : public MobRenderer
{
public:
	GhastRenderer();
	~GhastRenderer();

	void scale(const Mob& mob, Matrix& matrix, float a) override;
};