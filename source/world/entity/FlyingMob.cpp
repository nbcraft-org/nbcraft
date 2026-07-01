#include "FlyingMob.hpp"
#include "world/level/TileSource.hpp"

FlyingMob::FlyingMob(TileSource& source) : Mob(source)
{
}

void FlyingMob::travel(const Vec2& pos)
{
	if (isInWater())
	{
		moveRelative(Vec3(pos.x, 0.02f, pos.y));
		move(m_vel);
		m_vel *= 0.8f;
	}
	else if (isInLava())
	{
		moveRelative(Vec3(pos.x, 0.02f, pos.y));
		move(m_vel);
		m_vel *= 0.5f;
	}
	else
	{
		float var3 = 0.91f;
		if (m_bOnGround)
		{
			var3 = 546.0f * 0.1f * 0.1f * 0.1f;
			int var4 = m_tileSource->getTile(TilePos(Mth::floor(m_pos.x), Mth::floor(m_hitbox.min.y) - 1, Mth::floor(m_pos.z)));
			if (var4 > 0)
			{
				var3 = Tile::tiles[var4]->m_friction * 0.91f;
			}
		}

		float var8 = 0.16277136f / (var3 * var3 * var3);
		moveRelative(Vec3(pos.x, m_bOnGround ? 0.1f * var8 : 0.02f, pos.y));
		var3 = 0.91f;
		if (m_bOnGround)
		{
			var3 = 546.0f * 0.1f * 0.1f * 0.1f;
			int var5 = m_tileSource->getTile(TilePos(Mth::floor(m_pos.x), Mth::floor(m_hitbox.min.y) - 1, Mth::floor(m_pos.z)));
			if (var5 > 0)
			{
				var3 = Tile::tiles[var5]->m_friction * 0.91f;
			}
		}

		move(m_vel);
		m_vel *= var3;
	}

	m_walkAnimSpeedO = m_walkAnimSpeed;
	float var10 = m_pos.x - m_oPos.x;
	float var9 = m_pos.z - m_oPos.z;
	float var7 = Mth::sqrt(var10 * var10 + var9 * var9) * 4.0f;
	if (var7 > 1.0f)
	{
		var7 = 1.0f;
	}

	m_walkAnimSpeed += (var7 - m_walkAnimSpeed) * 0.4f;
	m_walkAnimPos += m_walkAnimSpeed;
}

