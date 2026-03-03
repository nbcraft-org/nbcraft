#include "FishingHookRenderer.hpp"
#include "EntityRenderDispatcher.hpp"
#include "world/entity/FishingHook.hpp"

FishingHookRenderer::FishingHookRenderer()
{
}

FishingHookRenderer::~FishingHookRenderer()
{
}

void FishingHookRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
	const FishingHook& hook = reinterpret_cast<const FishingHook&>(entity);
	MatrixStack::Ref matrix = MatrixStack::World.push();

	matrix->translate(pos);

#if MCE_GFX_API_OGL && !defined(FEATURE_GFX_SHADERS)
	glEnable(GL_RESCALE_NORMAL);
#endif

	matrix->scale(0.5f);
	int xi = 1;
	int yi = 2;
	bindTexture("particles.png"); // TODO: use C_PARTICLES_NAME when that gets created

	float texU_1 = float(xi * 8 + 0) / 128.0f; // u0
	float texU_2 = float(xi * 8 + 8) / 128.0f; // u1
	float texV_1 = float(yi * 8 + 0) / 128.0f; // v0
	float texV_2 = float(yi * 8 + 8) / 128.0f; // v1
	float r = 1.0f;
	float xo = 0.5f;
	float yo = 0.5f;
	matrix->rotate(180.0f - m_pDispatcher->m_rot.x, Vec3::UNIT_Y);
	matrix->rotate(-m_pDispatcher->m_rot.y, Vec3::UNIT_X);

	Tesselator& t = Tesselator::instance;
	t.begin(4);
	t.color(Color::WHITE);
	t.normal(Vec3::UNIT_Y);
	t.vertexUV(0.0f - xo, 0.0f - yo, 0.0f, texU_1, texV_2);
	t.vertexUV(r - xo, 0.0f - yo, 0.0f, texU_2, texV_2);
	t.vertexUV(r - xo, 1.0f - yo, 0.0f, texU_2, texV_1);
	t.vertexUV(0.0f - xo, 1.0f - yo, 0.0f, texU_1, texV_1);
	t.draw(m_shaderMaterials.entity_alphatest);

#if MCE_GFX_API_OGL && !defined(FEATURE_GFX_SHADERS)
	glDisable(GL_RESCALE_NORMAL);
#endif
	if (hook.m_owner) {
		float rr = (hook.m_owner->m_oRot.y + (hook.m_owner->m_rot.y - hook.m_owner->m_oRot.y) * a) * float(M_PI) / 180.0f;
		float rr2 = (hook.m_owner->m_oRot.x + (hook.m_owner->m_rot.x - hook.m_owner->m_oRot.x) * a) * float(M_PI) / 180.0f;
		float ss = Mth::sin(rr);
		float cc = Mth::cos(rr);
		float ss2 = Mth::sin(rr2);
		float cc2 = Mth::cos(rr2);
		float xp = hook.m_owner->m_oPos.x + (hook.m_owner->m_pos.x - hook.m_owner->m_oPos.x) * a - cc * 0.7f - ss * 0.5f * cc2;
		float yp = hook.m_owner->m_oPos.y + (hook.m_owner->m_pos.y - hook.m_owner->m_oPos.y) * a - ss2 * 0.5f;
		float zp = hook.m_owner->m_oPos.z + (hook.m_owner->m_pos.z - hook.m_owner->m_oPos.z) * a - ss * 0.7f + cc * 0.5f * cc2;
		if (m_pDispatcher->m_pOptions->m_thirdPerson.get()) {
			rr = (hook.m_owner->m_oRot.y + (hook.m_owner->m_rot.y - hook.m_owner->m_oRot.y) * a) * float(M_PI) / 180.0f;
			ss = Mth::sin(rr);
			cc = Mth::cos(rr);
			xp = hook.m_owner->m_oPos.x + (hook.m_owner->m_pos.x - hook.m_owner->m_oPos.x) * a - cc * 0.35f - ss * 0.85f;
			yp = hook.m_owner->m_oPos.y + (hook.m_owner->m_pos.y - hook.m_owner->m_oPos.y) * a - 0.45f;
			zp = hook.m_owner->m_oPos.z + (hook.m_owner->m_pos.z - hook.m_owner->m_oPos.z) * a - ss * 0.35f + cc * 0.85f;
		}

		float xh = hook.m_owner->m_oPos.x + (hook.m_owner->m_pos.x - hook.m_owner->m_oPos.x) * a;
		float yh = hook.m_owner->m_oPos.y + (hook.m_owner->m_pos.y - hook.m_owner->m_oPos.y) * a + 0.25;
		float zh = hook.m_owner->m_oPos.z + (hook.m_owner->m_pos.z - hook.m_owner->m_oPos.z) * a;
		float xa = xp - xh;
		float ya = yp - yh;
		float za = zp - zh;
#if MCE_GFX_API_OGL && !defined(FEATURE_GFX_SHADERS)
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
#endif
		t.begin(3);
		t.color(0);
		int steps = 16;

		for (int i = 0; i <= steps; i++) {
			float aa = float(i) / float(steps);
			t.vertex(pos.x + xa * aa, pos.y + ya * (aa * aa + aa) * 0.5f + 0.25f, pos.z + za * aa);
		}

		t.end();
#if MCE_GFX_API_OGL && !defined(FEATURE_GFX_SHADERS)
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
#endif
	}
}
