#pragma once

#include "EntityRenderer.hpp"

class ItemSpriteRenderer : public EntityRenderer
{
public:
	ItemSpriteRenderer(int sprite);

private:
	void _compile();

public:
	void render(const Entity& entity, const Vec3& pos, float rot, float a) override;
	void onGraphicsReset() override;
	Color getOverlayColor(const Entity& entity, float a) const override;

private:
	const int m_sprite;
	bool m_bCompiled;
	mce::Mesh m_mesh;
};

