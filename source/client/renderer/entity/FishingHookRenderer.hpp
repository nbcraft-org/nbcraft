#pragma once

#include "EntityRenderer.hpp"

class FishingHookRenderer : public EntityRenderer
{
public:
	FishingHookRenderer();
	~FishingHookRenderer();
	void render(const Entity& entity, const Vec3& pos, float rot, float a) override;
};

