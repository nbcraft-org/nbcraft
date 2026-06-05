#include "Squid.hpp"
#include "world/entity/Player.hpp"

Squid::Squid(Level* pLevel) : WaterAnimal(pLevel)
{
	m_pDescriptor = &EntityTypeDescriptor::squid;
	m_renderType = RENDER_SQUID;
	m_texture = "mob/squid.png";
	setSize(0.95f, 0.95f);
	m_xBodyRot = 0.0f;
	m_xBodyRotO = 0.0f;
	m_zBodyRot = 0.0f;
	m_zBodyRotO = 0.0f;
	m_tentacleMovement = 0.0f;
	m_oldTentacleMovement = 0.0f;
	m_tentacleAngle = 0.0f;
	m_oldTentacleAngle = 0.0f;
	m_speed = 0.0f;
	m_rotateSpeed = 0.0f;
	m_tentacleSpeed = 1.0f / (m_random.nextFloat() + 1.0f) * 0.2f;
}

//ItemStack Squid::inkSack;
void Squid::dropDeathLoot()
{
	//if (inkSack.isEmpty())
	static ItemStack inkSack = ItemStack(Item::dye_powder, 1, 0);

	int var1 = m_random.nextInt(3) + 1;

	for (int var2 = 0; var2 < var1; ++var2)
	{
		spawnAtLocation(inkSack, 0.0f);
	}

}

bool Squid::interact(Player* player)
{
	return false;
}

bool Squid::isInWater()
{
	AABB bb = m_hitbox;
	bb.grow(0.0f, -0.6f, 0.0f);
	return m_pLevel->checkAndHandleWater(bb, Material::water, this);
}

void Squid::aiStep()
{
	WaterAnimal::aiStep();
	m_xBodyRotO = m_xBodyRot;
	m_zBodyRotO = m_zBodyRot;
	m_oldTentacleMovement = m_tentacleMovement;
	m_oldTentacleAngle = m_tentacleAngle;
	m_tentacleMovement += m_tentacleSpeed;
	if (m_tentacleMovement > float(M_PI) * 2.0f)
	{
		m_tentacleMovement -= float(M_PI) * 2.0f;
		if (m_random.nextInt(10) == 0)
		{
			m_tentacleSpeed = 1.0f / (m_random.nextFloat() + 1.0f) * 0.2f;
		}
	}

	if (isInWater())
	{
		float var1;
		if (m_tentacleMovement < float(M_PI))
		{
			var1 = m_tentacleMovement / float(M_PI);
			m_tentacleAngle = Mth::sin(var1 * var1 * float(M_PI)) * float(M_PI) * 0.25f;
			if (var1 > 0.75f)
			{
				m_speed = 1.0f;
				m_rotateSpeed = 1.0f;
			}
			else
			{
				m_rotateSpeed *= 0.8f;
			}
		}
		else
		{
			m_tentacleAngle = 0.0f;
			m_speed *= 0.9f;
			m_rotateSpeed *= 0.99f;
		}

		if (!interpolateOnly())
		{
			m_vel.x = (m_tPos.x * m_speed);
			m_vel.y = (m_tPos.y * m_speed);
			m_vel.z = (m_tPos.z * m_speed);
		}

		var1 = Mth::sqrt(m_vel.x * m_vel.x + m_vel.z * m_vel.z);
		m_yBodyRot += (-(Mth::atan2(m_vel.x, m_vel.z)) * 180.0f / float(M_PI) - m_yBodyRot) * 0.1f; // field_E8 = yBodyRot
		m_rot.x = m_yBodyRot; // m_rot.x is supposed to be m_rot.y but it's inverted?!
		m_zBodyRot += float(M_PI) * m_rotateSpeed * 1.5f;
		m_xBodyRot += (-(Mth::atan2(var1, m_vel.y)) * 180.0f / float(M_PI) - m_xBodyRot) * 0.1f;
	}
	else
	{
		m_tentacleAngle = Mth::abs(Mth::sin(m_tentacleMovement)) * float(M_PI) * 0.25f;
		if (!interpolateOnly())
		{
			m_vel.x = 0.0f;
			m_vel.y -= 0.08f;
			m_vel.y *= 0.98f;
			m_vel.z = 0.0f;
		}

		m_xBodyRot = (m_xBodyRot + (-90.0f - m_xBodyRot) * 0.02f);
	}
}

void Squid::travel(const Vec2&)
{
	move(m_vel);
}

void Squid::updateAi()
{
	if (m_random.nextInt(50) == 0 || !m_bWasInWater || m_tPos == Vec3::ZERO)
	{
		float var1 = m_random.nextFloat() * float(M_PI) * 2.0f;
		m_tPos.x = Mth::cos(var1) * 0.2f;
		m_tPos.y = -0.1f + m_random.nextFloat() * 0.2f;
		m_tPos.z = Mth::sin(var1) * 0.2f;
	}
}
