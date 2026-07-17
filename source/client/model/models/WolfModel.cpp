#include "WolfModel.hpp"
#include "common/Mth.hpp"
#include "world/entity/Wolf.hpp"
#include "renderer/ShaderConstants.hpp"
#include "common/Utils.hpp"

WolfModel::WolfModel() :
	Model(64, 32),
	m_head(this, 0, 0),
	m_body(this, 18, 14),
	m_leg1(this, 0, 18),
	m_leg2(this, 0, 18),
	m_leg3(this, 0, 18),
	m_leg4(this, 0, 18),
	m_rightEar(this, 16, 14),
	m_leftEar(this, 16, 14),
	m_snout(this, 0, 10),
	m_tail(this, 9, 18),
	m_mane(this, 21, 0)
{
	m_yBabyHeadOffset = 8.0f;
	m_zBabyHeadOffset = 4.0f;

	float var1 = 0.0f;
	constexpr float var2 = 13.5f;
	m_head.addBox(-3.0f, -3.0f, -2.0f, 6, 6, 4, var1);
	m_head.setPos(-1.0f, var2, -7.0f);
	m_body.addBox(-4.0f, -2.0f, -3.0f, 6, 9, 6, var1);
	m_body.setPos(0.0f, 14.0f, 2.0f);
	m_mane.addBox(-4.0f, -3.0f, -3.0f, 8, 6, 7, var1);
	m_mane.setPos(-1.0f, 14.0f, 2.0f);
	m_leg1.addBox(-1.0f, 0.0f, -1.0f, 2, 8, 2, var1);
	m_leg1.setPos(-2.5f, 16.0f, 7.0f);
	m_leg2.addBox(-1.0f, 0.0f, -1.0f, 2, 8, 2, var1);
	m_leg2.setPos(0.5f, 16.0f, 7.0f);
	m_leg3.addBox(-1.0f, 0.0f, -1.0f, 2, 8, 2, var1);
	m_leg3.setPos(-2.5f, 16.0f, -4.0f);
	m_leg4.addBox(-1.0f, 0.0f, -1.0f, 2, 8, 2, var1);
	m_leg4.setPos(0.5f, 16.0f, -4.0f);
	m_tail.addBox(-1.0f, 0.0f, -1.0f, 2, 8, 2, var1);
	m_tail.setPos(-1.0f, 12.0f, 8.0f);
	m_rightEar.addBox(-3.0f, -5.0f, 0.0f, 2, 2, 1, var1);
	m_rightEar.setPos(-1.0f, var2, -7.0f);
	m_leftEar.addBox(1.0f, -5.0f, 0.0f, 2, 2, 1, var1);
	m_leftEar.setPos(-1.0f, var2, -7.0f);
	m_snout.addBox(-2.0f, 0.0f, -5.0f, 3, 3, 4, var1);
	m_snout.setPos(-0.5f, var2, -7.0f);
}

WolfModel::~WolfModel()
{
}

void WolfModel::render(float a, float b, float c, float d, float e, float f)
{
	setupAnim(a, b, c, d, e, f);

	m_head.renderRollable(f);
	m_body.render(f);
	m_leg1.render(f);
	m_leg2.render(f);
	m_leg3.render(f);
	m_leg4.render(f);
	m_rightEar.renderRollable(f);
	m_leftEar.renderRollable(f);
	m_snout.renderRollable(f);
	m_tail.renderRollable(f);
	m_mane.render(f);
}

void WolfModel::setupAnim(float a2, float a3, float a4, float a5, float a6, float a7)
{
	m_head.m_rot.x = a6 / (float)(180.0f / M_PI);
	m_head.m_rot.y = a5 / (float)(180.0f / M_PI);
	m_rightEar.m_rot.y = m_head.m_rot.y;
	m_rightEar.m_rot.x = m_head.m_rot.x;
	m_leftEar.m_rot.y = m_head.m_rot.y;
	m_leftEar.m_rot.x = m_head.m_rot.x;
	m_snout.m_rot.y = m_head.m_rot.y;
	m_snout.m_rot.x = m_head.m_rot.x;
	m_tail.m_rot.x = a4;
}

void WolfModel::prepareMobModel(const Mob& mob, float var2, float var3, float var4)
{
	const Wolf& wolf = (const Wolf&) mob;
	if (wolf.isAngry())
		m_tail.m_rot.y = 0.0f;
	else
		m_tail.m_rot.y = Mth::cos(var2 * 0.6662f) * 1.4f * var3;

	if (wolf.isOrderedToSit())
	{
		m_mane.setPos(-1.0f, 16.0f, -3.0f);
		m_mane.m_rot.x = M_PI * 0.4f;
		m_mane.m_rot.y = 0.0f;
		m_body.setPos(0.0f, 18.0f, 0.0f);
		m_body.m_rot.x = M_PI * 0.25f;
		m_tail.setPos(-1.0f, 21.0f, 6.0f);
		m_leg1.setPos(-2.5f, 22.0f, 2.0f);
		m_leg1.m_rot.x = M_PI * 3.0f / 2.0f;
		m_leg2.setPos(0.5f, 22.0f, 2.0f);
		m_leg2.m_rot.x = M_PI * 3.0f / 2.0f;
		m_leg3.m_rot.x = M_PI * 1.85f;
		m_leg3.setPos(-2.49f, 17.0f, -4.0f);
		m_leg4.m_rot.x = M_PI * 1.85f;
		m_leg4.setPos(0.51f, 17.0f, -4.0f);
	}
	else
	{
		m_body.setPos(0.0f, 14.0f, 2.0f);
		m_body.m_rot.x = M_PI * 0.5f;
		m_mane.setPos(-1.0f, 14.0f, -3.0f);
		m_mane.m_rot.x = m_body.m_rot.x;
		m_tail.setPos(-1.0f, 12.0f, 8.0f);
		m_leg1.setPos(-2.5f, 16.0f, 7.0f);
		m_leg2.setPos(0.5f, 16.0f, 7.0f);
		m_leg3.setPos(-2.5f, 16.0f, -4.0f);
		m_leg4.setPos(0.5f, 16.0f, -4.0f);
		m_leg1.m_rot.x = Mth::cos(var2 * 0.6662f) * 1.4f * var3;
		m_leg2.m_rot.x = Mth::cos(var2 * 0.6662f + M_PI) * 1.4f * var3;
		m_leg3.m_rot.x = Mth::cos(var2 * 0.6662f + M_PI) * 1.4f * var3;
		m_leg4.m_rot.x = Mth::cos(var2 * 0.6662f) * 1.4f * var3;
	}

	float var6 = wolf.getHeadRollAngle(var4) + wolf.getBodyRollAngle(var4, 0.0f);
	m_head.m_rot.z = var6;
	m_rightEar.m_rot.z = var6;
	m_leftEar.m_rot.z = var6;
	m_snout.m_rot.z = var6;
	m_mane.m_rot.z = wolf.getBodyRollAngle(var4, -0.08f);
	m_body.m_rot.z = wolf.getBodyRollAngle(var4, -0.16f);
	m_tail.m_rot.z = wolf.getBodyRollAngle(var4, -0.2f);
	if (wolf.isShaking())
	{
		float shade = wolf.getBrightness(var4) * wolf.getWetShade(var4);
		currentShaderColor = Color(shade, shade, shade);
	}
}
