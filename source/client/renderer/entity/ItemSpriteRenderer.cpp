#include "ItemSpriteRenderer.hpp"
#include "EntityRenderDispatcher.hpp"

ItemSpriteRenderer::ItemSpriteRenderer(int sprite)
	: m_sprite(sprite)
	, m_bCompiled(false)
{
}

void ItemSpriteRenderer::_compile()
{
	Tesselator& t = Tesselator::instance;
	t.begin(4);
	t.normal(Vec3::UNIT_Y);
	t.vertexUV(-0.5f, -0.25f, 0.0f, float(16 * (m_sprite % 16))     / 256.0f, float(16 * (m_sprite / 16 + 1)) / 256.0f);
	t.vertexUV(+0.5f, -0.25f, 0.0f, float(16 * (m_sprite % 16 + 1)) / 256.0f, float(16 * (m_sprite / 16 + 1)) / 256.0f);
	t.vertexUV(+0.5f, +0.75f, 0.0f, float(16 * (m_sprite % 16 + 1)) / 256.0f, float(16 * (m_sprite / 16))     / 256.0f);
	t.vertexUV(-0.5f, +0.75f, 0.0f, float(16 * (m_sprite % 16))     / 256.0f, float(16 * (m_sprite / 16))     / 256.0f);
	m_mesh = t.end();

	m_bCompiled = true;
}

void ItemSpriteRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
	MatrixStack::Ref matrix = MatrixStack::World.push();

	matrix->translate(pos);

	matrix->scale(0.5f);

	bindTexture(C_ITEMS_NAME);

	// face the camera
	matrix->rotate(180.0f - m_pDispatcher->m_rot.yaw, Vec3::UNIT_Y);
	matrix->rotate(-m_pDispatcher->m_rot.pitch, Vec3::UNIT_X);

	if (!m_bCompiled)
		_compile();

	m_mesh.render(m_shaderMaterials.entity_alphatest);
}

void ItemSpriteRenderer::onGraphicsReset()
{
	m_mesh.reset();
	m_bCompiled = false;
}

Color ItemSpriteRenderer::getOverlayColor(const Entity& entity, float a) const
{
	// we don't recolor item sprites under any circumstances, this would lead to fireballs glowing orange in PE
	return Color::NIL;
}
