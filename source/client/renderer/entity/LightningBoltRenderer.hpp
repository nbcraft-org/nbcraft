#pragma once

#include "EntityRenderer.hpp"

class LightningBoltRenderer : public EntityRenderer
{
public:
	LightningBoltRenderer();
	void render(const Entity&, const Vec3&, float a, float b) override;
};
