#include "FireballRenderer.hpp"
#include "EntityRenderDispatcher.hpp"
#include "world/entity/projectile/Fireball.hpp"

FireballRenderer::FireballRenderer()
{
}

FireballRenderer::~FireballRenderer()
{
}

void FireballRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
	MatrixStack::Ref matrix = MatrixStack::World.push();

	matrix->translate(pos);

	constexpr float s = 2.0f;
	matrix->scale(s / 1.0f);
	int icon = Item::snowBall->getIcon(nullptr);
	bindTexture(C_ITEMS_NAME);

	/*float texU_1 = float(16 * (m_sprite % 16))     / 256.0f;
	float texU_2 = float(16 * (m_sprite % 16 + 1)) / 256.0f;
	float texV_1 = float(16 * (m_sprite / 16))     / 256.0f;
	float texV_2 = float(16 * (m_sprite / 16 + 1)) / 256.0f;*/

	matrix->rotate(180.0f - m_pDispatcher->m_rot.yaw, Vec3::UNIT_Y);
	matrix->rotate(-m_pDispatcher->m_rot.pitch, Vec3::UNIT_X);

	Tesselator& t = Tesselator::instance;
	t.begin(4);
	t.normal(Vec3::UNIT_Y);
	t.vertexUV(-0.5f, -0.25f, 0.0f, float(16 * (icon % 16)) / 256.0f, float(16 * (icon / 16 + 1)) / 256.0f);
	t.vertexUV(+0.5f, -0.25f, 0.0f, float(16 * (icon % 16 + 1)) / 256.0f, float(16 * (icon / 16 + 1)) / 256.0f);
	t.vertexUV(+0.5f, +0.75f, 0.0f, float(16 * (icon % 16 + 1)) / 256.0f, float(16 * (icon / 16)) / 256.0f);
	t.vertexUV(-0.5f, +0.75f, 0.0f, float(16 * (icon % 16)) / 256.0f, float(16 * (icon / 16)) / 256.0f);
	t.draw(m_shaderMaterials.entity_alphatest);
}
