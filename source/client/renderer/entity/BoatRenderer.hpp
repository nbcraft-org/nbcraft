#pragma once

#include "EntityRenderer.hpp"
#include "client/model/models/Model.hpp"

class BoatRenderer : public EntityRenderer
{
public:
	BoatRenderer();

	void render(const Entity&, const Vec3&, float, float) override;

protected:
	Model* m_pModel;
};

