#pragma once

#include "MobRenderer.hpp"

class WolfRenderer : public MobRenderer
{
public:
	WolfRenderer(Model*, float);
	~WolfRenderer();
	float getBob(const Mob&, float) override;
};
