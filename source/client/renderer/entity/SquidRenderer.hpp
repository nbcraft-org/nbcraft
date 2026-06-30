#pragma once

#include "MobRenderer.hpp"

class SquidRenderer : public MobRenderer
{
public:
	SquidRenderer(Model*, float);
	~SquidRenderer();

	void setupRotations(const Entity&, float, float, Matrix&, float) override;
	float getBob(const Mob&, float) override;
};
