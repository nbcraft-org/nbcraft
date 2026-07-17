#include "Ghast.hpp"
#include "world/level/Level.hpp"
#include "world/entity/projectile/Fireball.hpp"

Ghast::Ghast(Level* pLevel) : FlyingMob(pLevel)
{
	m_pDescriptor = &EntityTypeDescriptor::ghast;
	m_renderType = RENDER_GHAST;
	m_texture = "mob/ghast.png";
	setSize(4.0f, 4.0f);
	m_bFireImmune = true;
	m_floatDuration = 0;
	m_oCharge = 0;
	m_charge = 0;
	m_target = nullptr;
	m_retargetTime = 0;
}

void Ghast::updateAi()
{
	if (m_pLevel->m_difficulty == 0)
	{
		remove();
	}

	m_oCharge = m_charge;
	float var1 = m_targetPos.x - m_pos.x;
	float var3 = m_targetPos.y - m_pos.y;
	float var5 = m_targetPos.z - m_pos.z;
	float var7 = float(Mth::sqrt(var1 * var1 + var3 * var3 + var5 * var5));
	if (var7 < 1.0f || var7 > 60.0f)
	{
		m_targetPos.x = m_pos.x + float((m_random.nextFloat() * 2.0f - 1.0f) * 16.0f);
		m_targetPos.y = m_pos.y + float((m_random.nextFloat() * 2.0f - 1.0f) * 16.0f);
		m_targetPos.z = m_pos.z + float((m_random.nextFloat() * 2.0f - 1.0f) * 16.0f);
	}

	if (m_floatDuration-- <= 0)
	{
		m_floatDuration += m_random.nextInt(5) + 2;
		if (_canReach(m_targetPos, var7))
		{
			m_vel.x += var1 / var7 * 0.1f;
			m_vel.y += var3 / var7 * 0.1f;
			m_vel.z += var5 / var7 * 0.1f;
		}
		else
		{
			m_targetPos = m_pos;
		}
	}

	if (m_target && m_target->m_bRemoved)
	{
		m_target = nullptr;
	}

	if (!m_target || m_retargetTime-- <= 0)
	{
		m_target = m_pLevel->getNearestAttackablePlayer(*this, 100.0f);
		if (m_target)
		{
			m_retargetTime = 20;
		}
	}

	float var9 = 64.0f;
	if (m_target && m_target->distanceToSqr(this) < var9 * var9)
	{
		Vec3 fireballPos = m_target->m_pos - m_pos;
		fireballPos.y = m_target->m_hitbox.min.y + float(m_target->m_bbHeight / 2.0f) - (m_pos.y + float(m_bbHeight / 2.0f));
		m_yBodyRot = m_rot.yaw = -(float(Mth::atan2(fireballPos.x, fireballPos.z))) * 180.0f / float(M_PI);
		if (canSee(m_target))
		{
			if (m_charge == 10)
			{
				m_pLevel->playSound(this, "mob.ghast.charge", getSoundVolume(), (m_random.nextFloat() - m_random.nextFloat()) * 0.2f + 1.0f);
			}

			++m_charge;
			if (m_charge == 20)
			{
				m_pLevel->playSound(this, "mob.ghast.fireball", getSoundVolume(), (m_random.nextFloat() - m_random.nextFloat()) * 0.2f + 1.0f);
				Fireball* entity = new Fireball(m_pLevel, this, fireballPos); // var17
				float var18 = 4.0f;
				Vec3 var20 = getViewVector(1.0f);
				entity->m_pos.x = m_pos.x + var20.x * var18;
				entity->m_pos.y = m_pos.y + float(m_bbHeight / 2.0f) + 0.5f;
				entity->m_pos.z = m_pos.z + var20.z * var18;
				m_pLevel->addEntity(entity);
				m_charge = -40;
			}
		}
		else if (m_charge > 0)
		{
			--m_charge;
		}
	}
	else
	{
		m_yBodyRot = m_rot.yaw = -(float(Mth::atan2(m_vel.x, m_vel.y))) * 180.0f / float(M_PI);
		if (m_charge > 0)
		{
			--m_charge;
		}
	}

	m_texture = m_charge > 10 ? "mob/ghast_fire.png" : "mob/ghast.png";
}

bool Ghast::_canReach(const Vec3& travel, float var7)
{
	Vec3 var9 = (m_targetPos - m_pos) / var7;
	AABB aabb = m_hitbox;

	for (int var16 = 1; (float)var16 < var7; ++var16)
	{
		aabb.move(var9);
		if (m_pLevel->getCubes(this, aabb)->size() > 0)
		{
			return false;
		}
	}

	return true;
}
