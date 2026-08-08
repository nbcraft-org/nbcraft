#include "FishingHookRenderer.hpp"
#include "EntityRenderDispatcher.hpp"
#include "client/renderer/renderer/RenderMaterialGroup.hpp"
#include "renderer/ShaderConstants.hpp"
#include "world/entity/projectile/FishingHook.hpp"

FishingHookRenderer::FishingHookRenderer()
{
}

FishingHookRenderer::~FishingHookRenderer()
{
}

void FishingHookRenderer::render(const Entity& entity, const Vec3& pos, float rot, float a)
{
	const FishingHook& hook = reinterpret_cast<const FishingHook&>(entity);
	Tesselator& t = Tesselator::instance;

	{
		MatrixStack::Ref matrix = MatrixStack::World.push();

		matrix->translate(pos);

		matrix->scale(0.5f);
		constexpr int xi = 1;
		constexpr int yi = 2;
		bindTexture(C_PARTICLES_NAME);

		constexpr float texU_1 = float(xi * 8 + 0) / 128.0f; // u0
		constexpr float texU_2 = float(xi * 8 + 8) / 128.0f; // u1
		constexpr float texV_1 = float(yi * 8 + 0) / 128.0f; // v0
		constexpr float texV_2 = float(yi * 8 + 8) / 128.0f; // v1
		constexpr float r = 1.0f;
		constexpr float xo = 0.5f;
		constexpr float yo = 0.5f;
		matrix->rotate(180.0f - m_pDispatcher->m_rot.yaw, Vec3::UNIT_Y);
		matrix->rotate(-m_pDispatcher->m_rot.pitch, Vec3::UNIT_X);

		t.begin(4);
		t.normal(Vec3::UNIT_Y);
		t.vertexUV(0.0f - xo, 0.0f - yo, 0.0f, texU_1, texV_2);
		t.vertexUV(r - xo, 0.0f - yo, 0.0f, texU_2, texV_2);
		t.vertexUV(r - xo, 1.0f - yo, 0.0f, texU_2, texV_1);
		t.vertexUV(0.0f - xo, 1.0f - yo, 0.0f, texU_1, texV_1);
		t.draw(m_shaderMaterials.entity_alphatest);
	}

	if (hook.m_owner)
	{
		Rot2 rr = (hook.m_owner->m_oRot + (hook.m_owner->m_rot - hook.m_owner->m_oRot) * a) * M_PI / 180.0f;
		float ss = Mth::sin(rr.yaw);
		float cc = Mth::cos(rr.yaw);
		float ss2 = Mth::sin(rr.pitch);
		float cc2 = Mth::cos(rr.pitch);

		
		// player pos
		Vec3 p = hook.m_owner->m_oPos + (hook.m_owner->m_pos - hook.m_owner->m_oPos) * a;

		// rod pos
		Vec3 r(p.x - cc * 0.7f - ss * 0.5f * cc2,
			   p.y - ss2 * 0.5f,
			   p.z - ss * 0.7f + cc * 0.5f * cc2);

		if (m_pDispatcher->m_pOptions->m_thirdPerson.get())
		{
			rr.yaw = (hook.m_owner->m_yBodyRotO + (hook.m_owner->m_yBodyRot - hook.m_owner->m_yBodyRotO) * a) * M_PI / 180.0f;
			ss = Mth::sin(rr.yaw);
			cc = Mth::cos(rr.yaw);
			r.x = p.x - cc * 0.35f - ss * 0.85f;
			r.y = p.y - 0.45f;
			r.z = p.z - ss * 0.35f + cc * 0.85f;
		}

		// hook pos
		Vec3 h = hook.m_oPos + (hook.m_pos - hook.m_oPos) * a;
		h.y += 0.25f;
		
		Vec3 delta = r - h;

		constexpr int steps = 16;
		t.begin(mce::PRIMITIVE_MODE_LINE_STRIP, steps);

		for (int i = 0; i <= steps; i++)
		{
			float aa = float(i) / float(steps);
			t.vertex(pos.x + delta.x * aa, pos.y + delta.y * (aa * aa + aa) * 0.5f + 0.25f, pos.z + delta.z * aa);
		}

		currentShaderColor = Color::BLACK;
		t.draw(m_materials.entity_flat_color);
	}
}
