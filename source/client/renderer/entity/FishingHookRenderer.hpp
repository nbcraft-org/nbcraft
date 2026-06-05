#pragma once

#include "EntityRenderer.hpp"
#include "renderer/Mesh.hpp"

class FishingHookRenderer : public EntityRenderer
{
public:
	FishingHookRenderer();
	~FishingHookRenderer();

private:
	//void _buildMesh();

public:
	void render(const Entity& entity, const Vec3& pos, float rot, float a) override;

private:
	//mce::Mesh m_mesh;
};

