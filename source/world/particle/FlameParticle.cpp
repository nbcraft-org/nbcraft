/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Particle.hpp"

FlameParticle::FlameParticle(Level* level, const Vec3& pos, const Vec3& dir) :
	Particle(level, pos, dir)
{
	m_oSize = 0.0f;

	m_vel = m_vel * 0.01f + dir;

	m_oSize = m_size;
	m_rCol = m_gCol = m_bCol = 1.0f;
	m_lifetime = int(8.0f / (0.2f + 0.8f * Mth::random())) + 4;
	m_bNoPhysics = true;
	m_tex = PTI_FLAME;
}

float FlameParticle::getBrightness(float unused) const
{
	// Unused flame burn out code from b1.2_02-20110517
	/*float life = float(m_age + unused) / float(m_lifetime);
	if (life < 0.0f) life = 0.0f;
	if (life > 1.0f) life = 1.0f;
	float br = Particle::getBrightness(unused);
	return br * life + 1.0f - life;*/

	return 1.0f;
}

void FlameParticle::tick()
{
	m_oPos = m_pos;

	m_age++;
	if (m_age > m_lifetime)
		remove();

	move(m_vel);

	m_vel *= 0.96f;

	if (m_bOnGround)
	{
		m_vel.x *= 0.7f;
		m_vel.z *= 0.7f;
	}
}

void FlameParticle::render(Tesselator& t, float f, float a, float b, float c, float d, float e)
{
	float mult = float(m_age + f) / float(m_lifetime);
	m_size = m_oSize * (1.0f - 0.5f * mult * mult);
	Particle::render(t, f, a, b, c, d, e);
}
