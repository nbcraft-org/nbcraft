#pragma once

#include "EntityRenderer.hpp"
#include "client/model/models/Model.hpp"

class MinecartRenderer : public EntityRenderer
{
public:
	MinecartRenderer();

	void render(const Entity&, const Vec3&, float, float) override;

protected:
	Model* m_pModel;
};

