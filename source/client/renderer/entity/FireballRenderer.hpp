#pragma once

#include "EntityRenderer.hpp"

class FireballRenderer : public EntityRenderer
{
public:
	FireballRenderer();
	~FireballRenderer();
	void render(const Entity& entity, const Vec3& pos, float rot, float a) override;

};

