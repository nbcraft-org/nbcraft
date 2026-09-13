#pragma once

#include "EntityRenderer.hpp"
#include "client/model/models/Model.hpp"

class MinecartRenderer : public EntityRenderer
{
private:
	void _renderInnerTile(const Entity& entity, Tile* tile, float a);

public:
	MinecartRenderer();

	void render(const Entity&, const Vec3&, float, float) override;

protected:
	Model* m_pModel;
};

